// Name        : CRuntimeController.cpp
// Author      : S.Rasmussen
// Date        : 29/04/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Global Headers
#include <boost/program_options.hpp>
#include <sstream>
#include <iostream>
#include <time.h>

// Local Headers
#include "Translations/Translations.h"
#include "CRuntimeController.h"
#include "CConfiguration.h"
#include "Estimates/CEstimateManager.h"
#include "Profiles/CProfileManager.h"
#include "RuntimeThread/CRuntimeThread.h"
#include "MCMC/CMCMC.h"
#include "Minimizers/CMinimizerManager.h"
#include "ConfigurationLoaders/CConfigurationLoader.h"
#include "Helpers/CError.h"

// Singleton Variable
CRuntimeController* CRuntimeController::clInstance = 0;

// Typedefs
typedef boost::mutex::scoped_lock lock;

// Namespaces
using namespace boost::program_options;
using std::cout;
using std::endl;
using std::ostringstream;

//**********************************************************************
// CRuntimeController::CRuntimeController()
// Default Constructor
//**********************************************************************
CRuntimeController::CRuntimeController() {
  // Setup Variables
  eRunMode                = RUN_MODE_INVALID;
  iCurrentEstimateValue   = 0;
  runtimeBarrier          = new boost::barrier(2);
  sCommandLineOptions     = "Undefined";
  pBaseThread             = 0;
}

//**********************************************************************
// CRuntimeController* CRuntimeController::Instance()
// Instance Method - Singleton
//**********************************************************************
CRuntimeController* CRuntimeController::Instance() {
  if (clInstance == 0)
    clInstance = new CRuntimeController();

  return clInstance;
}

//**********************************************************************
// void CRuntimeController::Destroy()
// Destroy Method - Singleton
//**********************************************************************
void CRuntimeController::Destroy() {
  if (clInstance != 0) {
    delete clInstance;
    clInstance = 0;
  }
}

//**********************************************************************
// bool CRuntimeController::parseCommandLine(int argc, char* argv[])
// Parse out command line
//**********************************************************************
void CRuntimeController::parseCommandLine(int argc, char* argv[]) {

  // Build some hidden options
  options_description oHidden("Hidden");
  oHidden.add_options()
      ("disable_reports,d", "Disable all reports");

  // Build Options menu
  options_description oDesc("Usage");
  oDesc.add_options()
      ("help,h", "Print help")
      ("license,l", "Display SPM license")
      ("version,v", "Display version information")
      ("config,c", value<string>(), "Configuration file")
      ("run,r", "Basic model run")
      ("estimate,e", "Point estimation")
      ("profile,p", "Likelihood profiles")
      ("mcmc,m", "MCMC")
      ("forward,f", "Forward projections")
      ("simulate,s", value<int>(), "Simulate observations")
      ("input,i", value<string>(), "Load free parameter values from file")
      ("threads,t", value<int>(), "Number of threads to spawn")
      ("quiet,q", "Run in quiet mode")
      ("genseed,g", value<int>(), "Random number seed");

  options_description oAllOptions("All");
  oAllOptions.add(oDesc);
  oAllOptions.add(oHidden);

  ostringstream o;
  o << oDesc;
  sCommandLineOptions = o.str();

  // Read our Parameters
  variables_map vmParams;
  try {
    store(parse_command_line(argc, argv, oAllOptions), vmParams);
    notify(vmParams);
  } catch (std::exception &ex) {
    throw string(ex.what());
  }

  // Check for Help Command
  if ( (vmParams.count("help")) || (vmParams.size() == 0) ) {
    setRunMode(RUN_MODE_HELP);
    return;
  }

  // Check for Version
  if (vmParams.count("version")) {
    setRunMode(RUN_MODE_VERSION);
    return;
  }

  // Check For License
  if (vmParams.count("license")) {
    setRunMode(RUN_MODE_LICENSE);
    return;
  }

  // Count how many modes are present.
  int iCount = 0;
  iCount += vmParams.count("run");
  iCount += vmParams.count("estimate");
  iCount += vmParams.count("profile");
  iCount += vmParams.count("mcmc");
  iCount += vmParams.count("forward");
  iCount += vmParams.count("simulate");

  if (iCount == 0)
    throw string("Missing run mode command line option");
  if (iCount > 1)
    throw string("More than one run mode parameter supplied");

  if (vmParams.count("run"))
    setRunMode(RUN_MODE_BASIC);
  else if (vmParams.count("estimate"))
    setRunMode(RUN_MODE_ESTIMATION);
  else if (vmParams.count("profile"))
    setRunMode(RUN_MODE_PROFILE);
  else if (vmParams.count("mcmc"))
    setRunMode(RUN_MODE_MARKOV_CHAIN_MONTE_CARLO);
  else if (vmParams.count("forward"))
    setRunMode(RUN_MODE_FORWARD_PROJECTION);
  else if (vmParams.count("simulate"))
    setRunMode(RUN_MODE_SIMULATION);

  // Variables
  CConfiguration *pConfig = CConfiguration::Instance();

  // Estimates
  if (vmParams.count("input"))
    pConfig->setEstimateValuesFile(vmParams["input"].as<string>());

  // Threads
  if (vmParams.count("threads"))
    pConfig->setNumberOfThreads(vmParams["threads"].as<int>());

  // Quiet
  if (vmParams.count("quiet"))
    pConfig->setQuietMode(true);

  // Random Seed
  if (vmParams.count("genseed")) {
    pConfig->setRandomSeed(vmParams["genseed"].as<int>());
  } else {
    long iSeed = time (0);
    iSeed = (long)(iSeed-(floor((double)iSeed/100000)*100000));
    pConfig->setRandomSeed(iSeed);
  }

  // Configuration File
  if (vmParams.count("config"))
    pConfig->setConfigFile(vmParams["config"].as<string>());

  // Set number of simulation candidates to generate
  if (vmParams.count("simulate"))
    pConfig->setSimulationCandidates(vmParams["simulate"].as<int>());
}

//**********************************************************************
// EState CRuntimeController::getCurrentState()
// Get the current state from the base thread
//**********************************************************************
EState CRuntimeController::getCurrentState() {
  return pBaseThread->getCurrentState();
}

//**********************************************************************
// CRuntimeThread* CRuntimeController::getCurrentThread()
// Return the Current Thread
//**********************************************************************
CRuntimeThread* CRuntimeController::getCurrentThread() {
  return pBaseThread;
}

//**********************************************************************
// void CRuntimeController::loadConfiguration()
// Load our Configurations
//**********************************************************************
void CRuntimeController::loadConfiguration() {
  try {
    // Estimation
    CConfigurationLoader clLoader = CConfigurationLoader();

    clLoader.loadConfigFile();
    clLoader.loadEstimateValuesFile();

  } catch (string Ex) {
    Ex = "CRuntimeController.loadConfiguration()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CRuntimeController::run()
// Run our Model. Break up Flow into Correct Functions
//**********************************************************************
void CRuntimeController::run() {
  try {
    CConfiguration    *pConfig          = CConfiguration::Instance();
    CEstimateManager  *pEstimateManager = CEstimateManager::Instance();

    // Create our Base Thread
    pBaseThread = new CRuntimeThread();
    pBaseThread->validate();
    pBaseThread->build();

    // Set a default number of estimate values we want to run
    // the model for. This defaults to 1, because we always
    // want to run atleast 1 iteration of the model
    int iEstimateValueCount = 1;

    // If we are using pre-loaded estimate values, how many?
    if (pConfig->getUseEstimateValues())
      iEstimateValueCount = pEstimateManager->getEstimateValueCount();

    for (int i = 0; i < iEstimateValueCount; ++i) {
      // If we are using loaded estimate values, set them now.
      if (pConfig->getUseEstimateValues()) {
        pEstimateManager->loadEstimateValues(i);
        pBaseThread->rebuild();
      }

      // Run the model.
      switch(eRunMode) {
        case RUN_MODE_BASIC:
          pBaseThread->executeBasicRun();
          break;

        case RUN_MODE_ESTIMATION:
          pBaseThread->executeEstimationRun();
          break;

        case RUN_MODE_PROFILE:
          pBaseThread->executeProfileRun();
          break;
        case RUN_MODE_MARKOV_CHAIN_MONTE_CARLO:
          startEstimation();
          startMCMC();
          break;
        case RUN_MODE_SIMULATION:
          pBaseThread->executeSimulationRun();
          break;
        default:
          CError::errorUnknown(PARAM_RUN_MODE, "");
      }
    }

  } catch (string Ex) {
    Ex = "CRuntimeController.run()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CRuntimeController::startEstimation()
// Start Estimation Run
//**********************************************************************
void CRuntimeController::startEstimation() {
  try {
    // Get Minimizer Manager
    CMinimizerManager *pMinimizer = CMinimizerManager::Instance();

    // Get Configuration
    // CConfiguration *pConfig = CConfiguration::Instance();
    // int iNumberOfThreads = pConfig->getNumberOfThreads();

    // Create Threads
   /* boost::thread_group threads;
    for (int i = 0; i < iNumberOfThreads; ++i)
      threads.create_thread(&CRuntimeController::initEstimationThread);*/

    // Start the Minimizer.
    pMinimizer->execute();

    // Wait for all threads to finish.
//    threads.join_all();

  } catch (string Ex) {
    Ex = "CRuntimeController.startEstimation()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// static void CRuntimeController::initEstimationThread()
// Initialise an Estimation Thread
//**********************************************************************
void CRuntimeController::initEstimationThread() {

  CRuntimeController  *pThis    = CRuntimeController::Instance();
  CRuntimeThread      *pThread  = 0;
  pThread             = new CRuntimeThread();

  if (true) {
    // Clone
    lock lk(pThis->runtimeLock);
    pThread->clone(pThis->pBaseThread);

    // Register with Publisher (CMinimizerManager)
    CMinimizerManager *pMinimizer = CMinimizerManager::Instance();
    pMinimizer->addThread(pThread);
  }

  // Validate and Build
  pThread->validate();
  pThread->build();

  // Setup Vars
  pThread->setWaiting(true);
  pThread->setTerminate(false);

  // Execute
  // ToDo: Complete Minimizer Pool
  //pThread->executeEstimation();

  delete pThread;
}

//**********************************************************************
// void CRuntimeController::startMCMC()
// Start MCMC
//**********************************************************************
void CRuntimeController::startMCMC() {
  try {
    // Setup the MCMC Controller.
    CMCMC *pMCMC = CMCMC::Instance();
    pMCMC->validate();
    pMCMC->build();

    // Get Configuration
    CConfiguration *pConfig = CConfiguration::Instance();
    int iNumberOfThreads = pConfig->getNumberOfThreads();

    // Create Threads
    boost::thread_group threads;
    for (int i = 0; i < iNumberOfThreads; ++i)
        threads.create_thread(&CRuntimeController::initMCMCThread);

    // Start the MCMC
    pMCMC->execute();

    // Wait for Threads to finish.
    threads.join_all();

  } catch (string Ex) {
    Ex = "CRuntimeController.startMCMC()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// static void CRuntimeController::initMCMCThread()
// Init an MCMC Thread
//**********************************************************************
void CRuntimeController::initMCMCThread() {

  CRuntimeController  *pThis    = CRuntimeController::Instance();
  CRuntimeThread      *pThread  = 0;
  pThread             = new CRuntimeThread();

  if (true) {
    // Clone
    lock lk(pThis->runtimeLock);
    pThread->clone(pThis->pBaseThread);

    // Register with Publisher (MCMC)
    CMCMC *pMCMC = CMCMC::Instance();
    pMCMC->addThread(pThread);
  }

  // Validate and Build
  pThread->validate();
  pThread->build();

  // Setup Vars
  pThread->setWaiting(true);
  pThread->setTerminate(false);

  // Execute
  pThread->executeMCMC();

  delete pThread;
}

//**********************************************************************
// CRuntimeController::~CRuntimeController()
// Default De-Constructor
//**********************************************************************
CRuntimeController::~CRuntimeController() {
  if (pBaseThread != 0)
    delete pBaseThread;

  CConfiguration::Destroy();
}
