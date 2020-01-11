//============================================================================
// Name        : CMCMC.h
// Author      : S.Rasmussen
// Date        : 2/03/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date$
//============================================================================
#ifndef CINDEPENDENCEMETROPOLIS_H_
#define CINDEPENDENCEMETROPOLIS_H_

// Global Headers
#include <boost/numeric/ublas/matrix.hpp>

// Local Headers
#include "../CMCMC.h"
#include "../../Minimizers/CMinimizerManager.h"
#include "../../Minimizers/CMinimizer.h"

// Classes
class CRuntimeThread;

using namespace boost::numeric;

//**********************************************************************
//
//
//**********************************************************************
class CIndependenceMetropolis : public CMCMC {
public:
  // Methods
  CIndependenceMetropolis();
  virtual                    ~CIndependenceMetropolis();
  void                       validate() final;
  void                       build() final;
  void                       execute() final;

protected:
  // Functions
  void                       buildCovarianceMatrix();
  void                       generateEstimates();
  void                       generateRandomStart();
  void                       generateNewCandidate();
  void                       fillMultivariateNormal(double stepsize);
  void                       fillMultivariatet(double stepsize);
  bool                       choleskyDecomposition();
  void                       updateStepSize(int iteration);

  // Variables
  double                     dStart;
  int                        iKeep;
  int                        iEstimateCount;
  int                        iJumps;
  int                        iSuccessfulJumps;
  int                        iJumpsSinceAdapt;
  int                        iSuccessfulJumpsSinceAdapt;
  double                     dMaxCorrelation;
  string                     sCorrelationMethod;
  double                     dCorrelationDiff;
  double                     dStepSize;
  double                     dAcceptanceRatio;
  string                     sProposalDistribution;
  int                        iDF;
  vector<double>             vCandidates;
  vector<bool>               vbIsEnabledEstimate;
  vector<int>                vAdaptStepSize;
  CMinimizer                 *pMinimizer;
};

#endif /*CINDEPENDENCEMETROPOLIS_H_*/
