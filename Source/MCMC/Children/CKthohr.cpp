/*
 * CKthohr.cpp
 *
 *  Created on: 11/01/2020
 *      Author: Scott Rasmussen (Zaita)
 */

#include "CKthohr.h"

#include <limits>
#include <numeric>
#include <iostream>
#include <boost/random.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include "kthohr/mcmc/mcmc.hpp"
#include "../../Estimates/CEstimate.h"
#include "../../Estimates/CEstimateManager.h"
#include "../../Helpers/CComparer.h"
#include "../../Helpers/CError.h"
#include "../../Helpers/CMath.h"
#include "../../Helpers/ForEach.h"
#include "../../Minimizers/CMinimizer.h"
#include "../../Minimizers/CMinimizerManager.h"
#include "../../ObjectiveFunction/CObjectiveFunction.h"
#include "../../RandomNumberGenerator/CRandomNumberGenerator.h"
#include "../../Reports/CReportManager.h"
#include "../../RuntimeThread/CRuntimeThread.h"

static int callback_count = 0;
double kthohr_callback(const arma::vec &vals_inp, void *ll_data) {
  cout << " ------- " << endl;
  // Update our Components with the New Parameters
  auto pEstimateManager = CEstimateManager::Instance();
  int iCount = pEstimateManager->getEnabledEstimateCount();
  for (int i = 0; i < iCount; ++i) {
    cout << "Setting " << pEstimateManager->getEnabledEstimate(i)->getLabel() << " to " << vals_inp(i) << endl;
    pEstimateManager->getEnabledEstimate(i)->setValue(vals_inp(i));
  }

  CObjectiveFunction *pObjectiveFunction = CObjectiveFunction::Instance();
  // Re-Run
  CRuntimeThread *pThread = CRuntimeController::Instance()->getCurrentThread();
  pThread->rebuild();
  pThread->startModel();

  // Workout our Objective Score
  pObjectiveFunction->execute();

  // Return Objective Score to Minimiser
  double score =pObjectiveFunction->getScore();
  cout << "XX: " << callback_count++ << " (" << score << ")" << endl;
  return score;
}

/**
 *
 */
CKthohr::CKthohr() {
  pParameterList->registerAllowed(PARAM_TYPE);
  pParameterList->registerAllowed(PARAM_ALGORITHM);
//  pParameterList->registerAllowed(PARAM_LENGTH);
  pParameterList->registerAllowed(PARAM_KEEP);
  pParameterList->registerAllowed(PARAM_COVARIANCE_ADJUSTMENT_METHOD);
  pParameterList->registerAllowed(PARAM_CORRELATION_ADJUSTMENT_DIFF);
  pParameterList->registerAllowed(PARAM_MAX_CORRELATION);
  pParameterList->registerAllowed(PARAM_BURN_IN);
}

/**
 *
 */
void CKthohr::validate() {
  sType = pParameterList->getString(PARAM_TYPE, true, PARAM_KTHOHR);
//  iLength = pParameterList->getInt(PARAM_LENGTH);
  iKeep   = pParameterList->getInt(PARAM_KEEP);
  dMaxCorrelation         = pParameterList->getDouble(PARAM_MAX_CORRELATION, true, 0.8);
  sCorrelationMethod      = pParameterList->getString(PARAM_COVARIANCE_ADJUSTMENT_METHOD, true, PARAM_COVARIANCE);
  dCorrelationDiff        = pParameterList->getDouble(PARAM_CORRELATION_ADJUSTMENT_DIFF, true, 0.0001);
  iBurnIn                 = pParameterList->getInt(PARAM_BURN_IN);

//  if (iLength < 1)
//    CError::errorLessThan(PARAM_LENGTH, PARAM_ONE);
}

/**
 *
 */
void CKthohr::build() {
  try {
    // Link to our minimiser
    pMinimizer = CMinimizerManager::Instance()->getMinimizer();

    // Get the names of our parameters
    vector<CEstimate*> vEstimates;
    CEstimateManager::Instance()->fillVector(vEstimates);
    foreach(CEstimate *Estimate, vEstimates) {
      vEstimateNames.push_back(Estimate->getParameter());
    }

    // Build default stepsize
    iEstimateCount = CEstimateManager::Instance()->getEnabledEstimateCount();

    int iIgnorableEstimates = 0;
    for (int i = 0; i < iEstimateCount; ++i) {
      CEstimate *estimate = CEstimateManager::Instance()->getEnabledEstimate(i);
      if ((estimate->getLowerBound() == estimate->getUpperBound())
          || estimate->getMCMCFixed())
        iIgnorableEstimates++;
    }
    if ((iEstimateCount - iIgnorableEstimates) < 1)
      CError::error("The number of estimated parameters must be at least one");

  } catch (string &Ex) {
    Ex = "CMCMC.build()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CMCMC::buildCovarianceMatrix()
// build our Covariance Matrix
//**********************************************************************
void CKthohr::buildCovarianceMatrix() {
  try {

    // Obtain the covariance matrix to use for the proposal distribution
    mxOriginalCovariance = pMinimizer->getCovarianceMatrix();
    mxCovariance = mxOriginalCovariance;

    if ( sCorrelationMethod == PARAM_NONE )
      return;

    // Adjust the covariance matrix for the proposal distribution
    for (int i=0; i < (int)mxCovariance.size1(); ++i) {
      for (int j=i+1; j < (int)mxCovariance.size2(); ++j) {
        if (mxCovariance(i,j) / sqrt(mxCovariance(i,i) * mxCovariance(j,j)) > dMaxCorrelation) {
          mxCovariance(i,j) = dMaxCorrelation * sqrt(mxCovariance(i,i) * mxCovariance(j,j));
        }
        if (mxCovariance(i,j) / sqrt(mxCovariance(i,i) * mxCovariance(j,j)) < -dMaxCorrelation){
          mxCovariance(i,j) = -dMaxCorrelation * sqrt(mxCovariance(i,i) * mxCovariance(j,j));
        }
      }
    }

    // Adjust any nonzero variances less than min_diff * the difference between the bounds on the parameter.
    // Obtain the estimation bounds to use to modify the covariance matrix
    vector<double> vDiffBounds;
    for (int i = 0; i < CEstimateManager::Instance()->getEnabledEstimateCount(); ++i) {
      CEstimate *estimate = CEstimateManager::Instance()->getEnabledEstimate(i);
      vDiffBounds.push_back( estimate->getUpperBound() - estimate->getLowerBound() );
    }

    for (int i=0; i < (int)mxCovariance.size1(); ++i) {
      if (mxCovariance(i,i) < (dCorrelationDiff * vDiffBounds[i]) && mxCovariance(i,i) != 0) {
        if (sCorrelationMethod == PARAM_COVARIANCE) {
          double dMultiplyCovariance = (sqrt(dCorrelationDiff) * vDiffBounds[i]) / sqrt(mxCovariance(i,i));
          for (int j=0 ; j < (int)mxCovariance.size1(); ++j) {
            mxCovariance(i,j) = mxCovariance(i,j) * dMultiplyCovariance;
            mxCovariance(j,i) = mxCovariance(j,i) * dMultiplyCovariance;
          }
        } else if(sCorrelationMethod == PARAM_CORRELATION) {
          mxCovariance(i,i) = dCorrelationDiff * vDiffBounds[i];
        }
      }
    }

  } catch (string &Ex) {
    Ex = "CKthohr.buildCovarianceMatrix()->" + Ex;
    throw Ex;
  }
}

/**
 *
 */
void CKthohr::execute() {
  execute_rwmh();
}



void CKthohr::execute_rwmh() {
  arma::vec initial_vals(iEstimateCount);
  arma::vec lower_bounds(iEstimateCount);
  arma::vec upper_bounds(iEstimateCount);
  for (int i = 0; i < iEstimateCount; ++i) {
    CEstimate *estimate = CEstimateManager::Instance()->getEnabledEstimate(i);
    initial_vals(i) = estimate->getValue();
    lower_bounds(i) = estimate->getLowerBound();
    upper_bounds(i) = estimate->getUpperBound();
  }

  arma::mat chain;
  mcmc::algo_settings_t settings;
  settings.rwmh_n_burnin = iBurnIn;
  settings.rwmh_n_draws = iKeep;
  settings.vals_bound = true;
  settings.lower_bounds = lower_bounds;
  settings.upper_bounds = upper_bounds;

  buildCovarianceMatrix();
  arma::mat covariance_matrix(mxCovariance.size1(), mxCovariance.size1());
  for (int i=0; i < (int)mxCovariance.size1(); ++i) {
    for (int j=0 ; j < (int)mxCovariance.size1(); ++j) {
      covariance_matrix(i,j) = mxCovariance(i,j);
    }
  }
  settings.rwmh_cov_mat = covariance_matrix;
  // settings.rwmh_par_scale - DONT KNOW HOW TO SET THIS

//  bool rwmh(const arma::vec& initial_vals, arma::mat& draws_out, std::function<double (const arma::vec& vals_inp, void* target_data)> target_log_kernel, void* target_data, algo_settings_t& settings);
  mcmc::rwmh(initial_vals, chain, kthohr_callback, 0, settings);

  for (int i = 0; i < iKeep; ++i) {
    vector<double> values;
    for (int j = 0; j < iEstimateCount; ++j) {
      values.push_back(chain.row(i)(j));
    }
    SChainItem newItem;
    newItem.iIteration                = i;
    newItem.dPenalty                  = 0.0; // We don't get this
    newItem.dScore                    = 0.0; // We don't get this
    newItem.dPrior                    = 0.0; // We don't get this
    newItem.dLikelihood               = 0.0; // We don't get this
    newItem.dAcceptanceRate           = 0; // We don't get this
    newItem.dAcceptanceRateSinceAdapt = 0; // We don't get this
    newItem.dStepSize                 = 0.0; // We don't get this
    newItem.vValues                   = values;
    vChain.push_back(newItem);
  }

}

