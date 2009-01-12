//============================================================================
// Name        : CNormalDirectedProcess.cpp
// Author      : S.Rasmussen
// Date        : 16/03/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local Headers
#include "CNormalDirectedProcess.h"
#include "../../Layers/Numeric/Base/CNumericLayer.h"
#include "../../Helpers/CError.h"
#include "../../Helpers/CMath.h"

//**********************************************************************
// CNormalDirectedProcess::CNormalDirectedProcess()
// Default Constructor
//**********************************************************************
CNormalDirectedProcess::CNormalDirectedProcess(CNormalDirectedProcess *Process)
: CDirectedProcess(Process) {

  // Vars
  dMu     = -1.0;
  dSigma  = -1.0;

  // Register Estimables
  registerEstimable(PARAM_MU, &dMu);
  registerEstimable(PARAM_SIGMA, &dSigma);

  // Register user allowed variables
  pParameterList->registerAllowed(PARAM_MU);
  pParameterList->registerAllowed(PARAM_SIGMA);

  // Copy Constructor
  if (Process != 0) {
    dMu     = Process->getMu();
    dSigma  = Process->getSigma();
  }
}

//**********************************************************************
// void CNormalDirectedProcess::validate()
// Validate
//**********************************************************************
void CNormalDirectedProcess::validate() {
  try {
    // Base Validation
    CDirectedProcess::validate();

    // Assign our variables
    dMu       = pParameterList->getDouble(PARAM_MU);
    dSigma    = pParameterList->getDouble(PARAM_SIGMA);

  } catch (string Ex) {
    Ex = "CNormalDirectedProcess.validate(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// double CNormalDirectedProcess::getResult(int RIndex, int CIndex, int TRIndex, int TCIndex)
// get Result
//**********************************************************************
double CNormalDirectedProcess::getResult(int RIndex, int CIndex, int TRIndex, int TCIndex) {

  dRet = 0.0;

#ifndef OPTIMISE
  try {
#endif

    dLayerValue = pLayer->getValue(TRIndex, TCIndex, RIndex, CIndex);
    dRet = pow(2.0,-((dLayerValue-dMu)/dSigma * (dLayerValue-dMu)/dSigma));

#ifndef OPTIMISE
  } catch (string Ex) {
    Ex = "CNormalDirectedProcess.getResult(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
#endif

  //return (dAlpha * dRet);
  return CMath::zeroFun(pow(dRet,dAlpha),ZERO);
}
//**********************************************************************
// CNormalDirectedProcess::~CNormalDirectedProcess()
// Default De-Constructor
//**********************************************************************
CNormalDirectedProcess::~CNormalDirectedProcess() {
}