//============================================================================
// Name        : CMCMC.cpp
// Author      : S.Rasmussen
// Date        : 2/03/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date$
//============================================================================

// Headers
#include <limits>
#include <numeric>
#include <iostream>
#include <boost/random.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include "CMCMC.h"
#include "../Estimates/CEstimate.h"
#include "../Estimates/CEstimateManager.h"
#include "../Helpers/CComparer.h"
#include "../Helpers/CError.h"
#include "../Helpers/CMath.h"
#include "../Helpers/ForEach.h"
#include "../Minimizers/CMinimizer.h"
#include "../Minimizers/CMinimizerManager.h"
#include "../ObjectiveFunction/CObjectiveFunction.h"
#include "../RandomNumberGenerator/CRandomNumberGenerator.h"
#include "../Reports/CReportManager.h"
#include "../RuntimeThread/CRuntimeThread.h"

// Namespaces
using namespace boost::numeric;
using std::cout;
using std::endl;

//**********************************************************************
// CMCMC::CMCMC()
// Default Constructor
//**********************************************************************
CMCMC::CMCMC() {
  // default variables
  bLastItem = false;

  pParameterList->registerAllowed(PARAM_TYPE);
}

//**********************************************************************
// CMCMC::~CMCMC()
// Default De-Constructor
//**********************************************************************
CMCMC::~CMCMC() {
}



