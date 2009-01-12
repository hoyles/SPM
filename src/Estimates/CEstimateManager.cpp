//============================================================================
// Name        : CEstimateManager.cpp
// Author      : S.Rasmussen
// Date        : 25/02/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description : << See CEstimateManager.h >>
// $Date$
//============================================================================

// Local Headers
#include "CEstimateManager.h"
#include "CEstimate.h"
#include "../ParameterParser/CParamParser.h"
#include "../Helpers/CError.h"
#include "../Helpers/CConvertor.h"
#include "../Helpers/ForEach.h"

// Singleton Variable
boost::thread_specific_ptr<CEstimateManager> CEstimateManager::clInstance;

//**********************************************************************
// CEstimateManager::CEstimateManager()
// Default Constructor
//**********************************************************************
CEstimateManager::CEstimateManager() {
}

//**********************************************************************
// CEstimateManager* CEstimateManager::Instance()
// Instance Method - Singleton
//**********************************************************************
CEstimateManager* CEstimateManager::Instance() {
  if (clInstance.get() == 0)
    clInstance.reset(new CEstimateManager());
  return clInstance.get();
}

//**********************************************************************
// void CEstimateManager::Destroy()
// Destroy Method - Singleton
//**********************************************************************
void CEstimateManager::Destroy() {
  if (clInstance.get() != 0) {
    clInstance.reset();
  }
}

//**********************************************************************
// void CEstimateManager::addEstimate(CEstimate *Estimate)
// Add Estimate to our List
//**********************************************************************
void CEstimateManager::addEstimate(CEstimate *Estimate) {
  vEstimateList.push_back(Estimate);
}

//**********************************************************************
// int CEstimateManager::getEnabledEstimateCount()
// Get the count for enabled Estimates
//**********************************************************************
int CEstimateManager::getEnabledEstimateCount() {
  try {
    // vars
    int     iEnabled = 0;
    vector<CEstimate*>::iterator vPtr = vEstimateList.begin();

    // Loop
    while (vPtr != vEstimateList.end()) {
      if ((*vPtr)->getEnabled())
        iEnabled++;
      vPtr++;
    }


    return iEnabled;

  } catch (string Ex) {
    Ex = "CEstimateManager.getEnabledEstimateCount()->" + Ex;
    throw Ex;
  }

  return 0;
}

//**********************************************************************
// CEstimate* CEstimateManager::getEnabledEstimate(int Index)
// Get Estimate
//**********************************************************************
CEstimate* CEstimateManager::getEnabledEstimate(int Index) {
  try {
    if ((Index+1) > (int)vEstimateList.size())
      CError::errorGreaterThan(PARAM_INDEX, PARAM_ESTIMATES);
    if (Index < 0)
      CError::errorLessThan(PARAM_INDEX, PARAM_ZERO);
    if (Index > getEnabledEstimateCount())
      CError::errorGreaterThan(PARAM_INDEX, PARAM_ENABLED_ESTIMATES);

    int iCurrentIndex = -1;

    // Loop through incrementing our index based on the enabled ones
    for (int i = 0; i < (int)vEstimateList.size(); ++i) {
      if (vEstimateList[i]->getEnabled())
        iCurrentIndex++;

      if (iCurrentIndex == Index)
        return vEstimateList[i];
    }

    throw string(ERROR_INVALID_IDX + CConvertor::intToString(Index));

  } catch (string Ex) {
    Ex = "CEstimateManager.getEnabledEstimate()->" + Ex;
    throw Ex;
  }

  return 0;
}

//**********************************************************************
// CEstimate* CEstimateManager::getEstimate(int Index)
// Get Estimate Index
//**********************************************************************
CEstimate* CEstimateManager::getEstimate(int Index) {
  try {
    if ((Index+1) > (int)vEstimateList.size())
      CError::errorGreaterThan(PARAM_INDEX, PARAM_ESTIMATES);
    if (Index < 0)
      CError::errorLessThan(PARAM_INDEX, PARAM_ZERO);

    return vEstimateList[Index];

  } catch (string Ex) {
    Ex = "CEstimateManager.getEnabledEstimate()->" + Ex;
    throw Ex;
  }

  return 0;
}

//**********************************************************************
// CEstimate* CEstimateManager::getEstimate(string Parameter)
// Get Estimate
//**********************************************************************
CEstimate* CEstimateManager::getEstimate(string Parameter) {
  try {
    // Variables
    vector<CEstimate*>::iterator vPtr           = vEstimateList.begin();
    string                       sParam         = Parameter;
    string                       sParam2        = "";

    // Loop Through
    while (vPtr != vEstimateList.end()) {
      // Make sure both commands are relative
      sParam2 = (*vPtr)->getParameter();

      // Check For Match
      if (sParam == sParam2)
        return (*vPtr);

      vPtr++;
    }

    throw string(ERROR_UNKNOWN_ESTIMATE + Parameter);

  } catch (string Ex) {
    Ex = "CEstimateManager.getEstimate()->" + Ex;
    throw Ex;
  }

  return 0;
}

//**********************************************************************
// void CEstimateManager::addEstimateValue(string estimate, double value)
// Add An Estimate Value to Be Used
//**********************************************************************
void CEstimateManager::addEstimateValue(string estimate, double value) {
  try {
    foreach(CEstimate *Estimate, vEstimateList) {
      if (Estimate->getParameter() == estimate) {
        Estimate->addValue(value);
        return;
      }
    }

    throw string(ERROR_UNKNOWN_ESTIMATE + estimate);

  } catch (string Ex) {
    Ex = "CEstimateManager.addEstimateValue()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// int CEstimateManager::getEstimateValueCount()
// Get The Number of Estimates Loaded
//**********************************************************************
int CEstimateManager::getEstimateValueCount() {
  try {
    if (vEstimateList.size() == 0)
      CError::errorMissing(PARAM_ESTIMATES);

    return vEstimateList[0]->getValueCount();

  } catch (string Ex) {
    Ex = "CEstimateManager.getEstimateValueCount()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CEstimateManager::loadEstimateValues(int index)
// Load The Estimate Values
//**********************************************************************
void CEstimateManager::loadEstimateValues(int index) {
  try {
    foreach(CEstimate *Estimate, vEstimateList) {
      Estimate->loadValue(index);
    }

  } catch (string Ex) {
    Ex = "CEstimateManager.loadEstimateValues()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CEstimateManager::clone(CEstimateManager *Manager)
// Clone the Estimate Manager for a new Thread
//**********************************************************************
void CEstimateManager::clone(CEstimateManager *Manager) {
  try {

    // Copy from Manager into out vector
    for (int i = 0; i < Manager->getEstimateCount(); ++i) {
      CEstimate *pEstimate = Manager->getEstimate(i);
      vEstimateList.push_back(pEstimate->clone());
    }

  } catch (string Ex) {
    Ex = "CEstimateManager.clone()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CEstimateManager::validate()
// Validate
//**********************************************************************
void CEstimateManager::validate() {
  try {
    // Variables
    vector<CEstimate*>::iterator  vPtr;
    map<string, int>              mParameterList;

    // Check for Duplicate Estimates
    vPtr = vEstimateList.begin();
    while (vPtr != vEstimateList.end()) {
      string sParam = (*vPtr)->getParameter();
      mParameterList[sParam] += 1;

      if (mParameterList[sParam] > 1)
        throw string(ERROR_DUPLICATE_PARAMETER + (*vPtr)->getParameter());
      vPtr++;
    }

    // Validate Each Estimate
    vPtr = vEstimateList.begin();
    while (vPtr != vEstimateList.end()) {
      (*vPtr)->validate();
      vPtr++;
    }

  } catch (string Ex) {
    Ex = "CEstimateManager.validate()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CEstimateManager::build()
// Build
//**********************************************************************
void CEstimateManager::build() {
  try {
    // Build Estimates
    vector<CEstimate*>::iterator vPtr = vEstimateList.begin();
    while (vPtr != vEstimateList.end()) {
      (*vPtr)->build();
      vPtr++;
    }

  } catch (string Ex) {
    Ex = "CEstimateManager.build()->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// CEstimateManager::~CEstimateManager()
// Default De-Constructor
//**********************************************************************
CEstimateManager::~CEstimateManager() {
  vector<CEstimate*>::iterator vPtr;
  vPtr = vEstimateList.begin();
  while (vPtr != vEstimateList.end()) {
    delete (*vPtr);
    vPtr++;
  }
  vEstimateList.clear();
}
