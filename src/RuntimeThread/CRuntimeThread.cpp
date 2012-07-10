//============================================================================
// Name        : CRuntimeThread.cpp
// Author      : S.Rasmussen
// Date        : 18/08/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Global Headers
#include <iostream>
#include <boost/lexical_cast.hpp>

// Local Headers
#include "CRuntimeThread.h"
#include "../BaseClasses/CBaseManager.h"
#include "../AgeingError/CAgeingErrorManager.h"
#include "../Catchabilities/CCatchabilityManager.h"
#include "../DerivedQuantities/CDerivedQuantityManager.h"
#include "../Estimates/CEstimateManager.h"
#include "../InitializationPhases/CInitializationPhaseManager.h"
#include "../Layers/CLayerManager.h"
#include "../Minimizers/CMinimizerManager.h"
#include "../MCMC/CMCMC.h"
#include "../ObjectiveFunction/CObjectiveFunction.h"
#include "../Observations/CObservationManager.h"
#include "../Penalties/CPenaltyManager.h"
#include "../PreferenceFunctions/CPreferenceFunctionManager.h"
#include "../Priors/CPriorManager.h"
#include "../Processes/CProcessManager.h"
#include "../Profiles/CProfileManager.h"
#include "../RandomNumberGenerator/CRandomNumberGenerator.h"
#include "../Reports/CReportManager.h"
#include "../Selectivities/CSelectivityManager.h"
#include "../SizeWeight/CSizeWeightManager.h"
#include "../AgeSize/CAgeSizeManager.h"
#include "../TimeSteps/CTimeStepManager.h"

//#include "../TimeSteps/CTimeStep.h"
//#include "../InitializationPhases/CInitializationPhase.h"
#include "../Helpers/ForEach.h"

// Using
using std::cout;
using std::endl;

//**********************************************************************
// CRuntimeThread::CRuntimeThread()
// Default Constructor
//**********************************************************************
CRuntimeThread::CRuntimeThread() {

  // Add our managers to the Vector
  vManagers.push_back(CWorld::Instance());
  vManagers.push_back(CProcessManager::Instance());
  vManagers.push_back(CAgeingErrorManager::Instance());
  vManagers.push_back(CSizeWeightManager::Instance());
  vManagers.push_back(CAgeSizeManager::Instance());
  vManagers.push_back(CCatchabilityManager::Instance());
  vManagers.push_back(CDerivedQuantityManager::Instance());
  vManagers.push_back(CInitializationPhaseManager::Instance());
  vManagers.push_back(CLayerManager::Instance());
  vManagers.push_back(CMinimizerManager::Instance());
  vManagers.push_back(CObservationManager::Instance());
  vManagers.push_back(CPenaltyManager::Instance());
  vManagers.push_back(CPreferenceFunctionManager::Instance());
  vManagers.push_back(CPriorManager::Instance());
  vManagers.push_back(CProfileManager::Instance());
  vManagers.push_back(CRandomNumberGenerator::Instance());
  vManagers.push_back(CReportManager::Instance());
  vManagers.push_back(CSelectivityManager::Instance());
  vManagers.push_back(CTimeStepManager::Instance());

  /**
   * EstimateManager goes last, because we want it to be validated
   * after everything else. As part of the validation the EstimateManager
   * will look for estimables, if they don't exist the system out error.
   *
   * All estimables must exist by the end of each Manager's validate
   * phase.
   */
  vManagers.push_back(CEstimateManager::Instance());

  // Variables
  bWaiting            = false;
  pEstimateManager    = CEstimateManager::Instance(); // Use for MCMC
  pObjectiveFunction  = CObjectiveFunction::Instance();
}

//**********************************************************************
// void CRuntimeThread::setWaiting(bool value)
// Set our Waiting Variable
//**********************************************************************
void CRuntimeThread::setWaiting(bool value) {
  lock lk(mutWaiting);
  bWaiting = value;
}

//**********************************************************************
// bool CRuntimeThread::getWaiting()
// Return our Waiting Variable.
//**********************************************************************
bool CRuntimeThread::getWaiting() {
  lock lk(mutWaiting);
  return bWaiting;
}

//**********************************************************************
// void CRuntimeThread::setTerminate(bool value)
// Set our Terminate Value
//**********************************************************************
void CRuntimeThread::setTerminate(bool value) {
  lock lk(mutTerminate);
  bTerminate = value;
}

//**********************************************************************
// bool CRuntimeThread::getTerminate()
// Return out Terminate Variable
//**********************************************************************
bool CRuntimeThread::getTerminate() {
  lock lk(mutTerminate);
  return bTerminate;
}

//**********************************************************************
// void CRuntimeThread::validate()
// Validate This Thread
//**********************************************************************
void CRuntimeThread::validate() {
  eCurrentState = STATE_VALIDATION;

  // Validate our Managers
  foreach(CBaseManager *Manager, vManagers) {
    Manager->validate();
  }
}

//**********************************************************************
// void CRuntimeThread::build()
// Build the thread's relationships
//**********************************************************************
void CRuntimeThread::build() {
  eCurrentState = STATE_CONSTRUCTION;

  // Validate our Managers
  foreach(CBaseManager *Manager, vManagers) {
    Manager->build();
  }

  // Build our Own Locals
  iNumberOfYears = pWorld->getCurrentYear() - pWorld->getInitialYear();
}

//**********************************************************************
// void CRuntimeThread::rebuild()
// Re-Build the components with caches.
//**********************************************************************
void CRuntimeThread::rebuild() {
  // Validate our Managers
  foreach(CBaseManager *Manager, vManagers) {
    Manager->rebuild();
  }
}

//**********************************************************************
// void CRuntimeThread::executeBasicRun()
// Start our Basic Run
//**********************************************************************
void CRuntimeThread::executeBasicRun() {
  // Validate, Build, Start
  startModel();

  // Execute Objective
  pObjectiveFunction->execute();
  dScore = pObjectiveFunction->getScore();

  // Change State
  eCurrentState = STATE_FINALIZATION;
  CReportManager::Instance()->execute(eCurrentState);
}

//**********************************************************************
// void CRuntimeThread::executeEstimationRun()
// Execute Estimations
//**********************************************************************
void CRuntimeThread::executeEstimationRun() {
  CMinimizerManager *pMinimizerManager = CMinimizerManager::Instance();
  pMinimizerManager->execute();

  // Now Execute a Basic Run
  pRuntimeController->setRunMode(RUN_MODE_BASIC);
  rebuild();
  executeBasicRun();

}

//**********************************************************************
// void CRuntimeThread:executeProfileRun()
// Execute a Profile Run
//**********************************************************************
void CRuntimeThread::executeProfileRun() {
  CProfileManager *pProfileManager = CProfileManager::Instance();
  pProfileManager->execute();
}

//**********************************************************************
// void CRuntimeThread::executeMCMC()
// Do a point estimate, then start our MCMC
//**********************************************************************
void CRuntimeThread::executeMCMC() {
  // Find a pointer estimate to start our chain from
  CMinimizerManager *pMinimizerManager = CMinimizerManager::Instance();
  pMinimizerManager->execute();

  // Rebuild our managers to ensure everything is back to default state
  rebuild();

  // Start our MCMC run
  CMCMC *mcmc = CMCMC::Instance();
  mcmc->validate();
  mcmc->build();
  mcmc->execute();
}

//**********************************************************************
// void CRuntimeThread::executeSimulationRun()
// Execute a Simulation Run
//**********************************************************************
void CRuntimeThread::executeSimulationRun() {

  int iSuffixIterationWidth = (int) floor(log10((double) CConfiguration::Instance()->getSimulationCandidates()+1)) +1;

  for (int i=0; i < CConfiguration::Instance()->getSimulationCandidates(); ++i) {
    string sReportSuffix = "." ;
    int iThisIterationWidth = floor(log10(i+1))+1;
    int iCount = iSuffixIterationWidth - iThisIterationWidth;
    if (iCount > 0) {
      for (int j=0; j < iCount; ++j) {
        sReportSuffix += "0";
      }
    }
    sReportSuffix += boost::lexical_cast<string>( i+1 );
    CReportManager::Instance()->setReportSuffix(sReportSuffix);

    // Validate, Build, Start
    startModel();

    // Change State
    eCurrentState = STATE_FINALIZATION;
    CReportManager::Instance()->execute(eCurrentState);
    rebuild();
  }
}

//**********************************************************************
// void CRuntimeThread::startModel()
// Start The Model
//**********************************************************************
void CRuntimeThread::startModel() {

  // Set State To Burn-In (Initialisation) & Execute
  eCurrentState = STATE_INITIALIZATION;
  CInitializationPhaseManager::Instance()->execute();
  CReportManager::Instance()->execute(eCurrentState);

  // Flag and start modelling
  eCurrentState = STATE_MODELLING;
  CTimeStepManager::Instance()->execute();

  pWorld->debugToScreen();
}

//**********************************************************************
// CRuntimeThread::~CRuntimeThread()
// Destructor
//**********************************************************************
CRuntimeThread::~CRuntimeThread() {

  // Destroy Singleton Classes in reverse Order
  CTimeStepManager::Destroy();
  CSelectivityManager::Destroy();
  CReportManager::Destroy();
  CRandomNumberGenerator::Destroy();
  CProfileManager::Destroy();
  CProcessManager::Destroy();
  CPriorManager::Destroy();
  CPreferenceFunctionManager::Destroy();
  CPenaltyManager::Destroy();
  CObservationManager::Destroy();
  CObjectiveFunction::Destroy();
  CMinimizerManager::Destroy();
  CLayerManager::Destroy();
  CInitializationPhaseManager::Destroy();
  CEstimateManager::Destroy();
  CDerivedQuantityManager::Destroy();
  CCatchabilityManager::Destroy();
  CAgeSizeManager::Destroy();
  CSizeWeightManager::Destroy();
  CAgeingErrorManager::Destroy();
  CWorld::Destroy();
}
