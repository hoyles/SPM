//============================================================================
// Name        : CCategoricalMonotonicPreferenceFunction.cpp
// Author      : S.Rasmussen
// Date        : 16/03/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local Headers
#include "CCategoricalMonotonicPreferenceFunction.h"
#include "../../Layers/CLayerManager.h"
#include "../../Layers/String/CStringLayer.h"
#include "../../Helpers/CMath.h"
#include "../../Helpers/CError.h"


//TODO: Check that this class/functions work correctly

//**********************************************************************
// CCategoricalMonotonicPreferenceFunction::CCategoricalMonotonicPreferenceFunction()
// Default Constructor
//**********************************************************************
CCategoricalMonotonicPreferenceFunction::CCategoricalMonotonicPreferenceFunction() {

  // Register user allowed parameters
  pParameterList->registerAllowed(PARAM_CATEGORY_VALUES);
  pParameterList->registerAllowed(PARAM_CATEGORY_LABELS);
  pParameterList->registerAllowed(PARAM_CATEGORIES);
  pParameterList->registerAllowed(PARAM_SELECTIVITIES);

}

//**********************************************************************
// void CCategoricalMonotonicPreferenceFunction::validate()
// Validate
//**********************************************************************
void CCategoricalMonotonicPreferenceFunction::validate() {
  try {
    // Base Validation
    CPreferenceFunction::validate();

    // Assign local variables
    pParameterList->fillVector(vLabels, PARAM_CATEGORY_LABELS);
    pParameterList->fillVector(vValues, PARAM_CATEGORY_VALUES);
    for (int i = 1; i < (int)vValues.size(); i++) {
      if(vValues[i] < 0) {
        CError::errorLessThan(PARAM_CATEGORY_VALUES, PARAM_ZERO); // TODO: Not a helpful error message: Should report that these values are not monotonically increasing.
      }
    }
    // TODO: Validate that the length of VALUES is the same as the length LABELS
    //       Validate that all VALUES are numeric
    //       Validate that the layer has a number of discrete character values that exactly match CATEGORY_LABELS
    //       Validate that the layer is a string layer
    pParameterList->fillVector(vCategories, PARAM_CATEGORIES);
    pParameterList->fillVector(vSelectivities, PARAM_SELECTIVITIES);

  // Register estimables
    for (int i = 0; i < (int)vValues.size(); ++i)
      registerEstimable(PARAM_CATEGORY_VALUES, i, &vValues[i]);

  } catch (string &Ex) {
    Ex = "CCategoricalMonotonicPreferenceFunction.validate(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CCategoricalMonotonicPreferenceFunction::build()
// Build our Object
//**********************************************************************
void CCategoricalMonotonicPreferenceFunction::build() {
  try {

    // Get our Layer Pointer
    CLayerManager *pLayerManager = CLayerManager::Instance();
    pLayer = pLayerManager->getStringLayer(sLayerName);

  } catch (string &Ex) {
    Ex = "CCategoricalMonotonicPreferenceFunction.build(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// double CCategoricalMonotonicPreferenceFunction::getResult(int RIndex, int CIndex, int TRIndex, int TCIndex)
// Get Result
//**********************************************************************
double CCategoricalMonotonicPreferenceFunction::getResult(int RIndex, int CIndex, int TRIndex, int TCIndex) {

  dRet = 0.0;

#ifndef OPTIMIZE
  try {
#endif
    //TODO: Scott to check code for efficiency
    //Function should identify the label, and then return the corresponding value
    sLayerValue = pLayer->getValue(RIndex, CIndex);

    for (int i = 0; i < (int)vLabels.size(); i++) {
      if(vLabels[i] == sLayerValue) {
        dRet = vValues[i];
        break;
      }
    }

#ifndef OPTIMIZE
  } catch (string &Ex) {
    Ex = "CCategoricalMonotonicPreferenceFunction.getResult(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
#endif

  return CMath::zeroFun(pow(dRet,dAlpha),ZERO);
}
//**********************************************************************
// CCategoricalMonotonicPreferenceFunction::~CCategoricalMonotonicPreferenceFunction()
// Default De-Constructor
//**********************************************************************
CCategoricalMonotonicPreferenceFunction::~CCategoricalMonotonicPreferenceFunction() {
}