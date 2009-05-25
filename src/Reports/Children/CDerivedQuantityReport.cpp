//============================================================================
// Name        : CDerivedQuantityReport.cpp
// Author      : S.Rasmussen
// Date        : 26/03/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local headers
#include "CDerivedQuantityReport.h"

//**********************************************************************
// CDerivedQuantityReport::CDerivedQuantityReport()
// Constructor
//**********************************************************************
CDerivedQuantityReport::CDerivedQuantityReport() {
  // Variables
  eExecutionState   = STATE_FINALIZATION;
}

//**********************************************************************
// void CDerivedQuantityReport::execute()
// Execute
//**********************************************************************
void CDerivedQuantityReport::execute() {
  // Check for correct state
  if (pRuntimeController->getRunMode() != RUN_MODE_BASIC)
    if (pRuntimeController->getRunMode() != RUN_MODE_PROFILE)
      return;

  this->start();

  // Write Information
  cout << CONFIG_ARRAY_START << sLabel << CONFIG_ARRAY_END << "\n";
  cout << PARAM_REPORT << "." << PARAM_TYPE << CONFIG_RATIO_SEPARATOR << " " << pParameterList->getString(PARAM_TYPE) << "\n";
  cout << "# Not Yet Implemented" << "\n";
  cout << "*end\n" << endl;

  this->end();
}

//**********************************************************************
// CDerivedQuantityReport::~CDerivedQuantityReport()
// Destructor
//**********************************************************************
CDerivedQuantityReport::~CDerivedQuantityReport() {
}
