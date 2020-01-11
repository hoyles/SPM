//============================================================================
// Name        : CEstimateFactory.cpp
// Author      : S.Rasmussen
// Date        : 12/01/2009
// Copyright   : Copyright NIWA Science ©2008 - www.niwa.co.nz
// Description : << See CEstimateFactory.h >>
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local headers
#include "CMCMCFactory.h"
#include "../CMCMCManager.h"
#include "../../Helpers/CError.h"
#include "../Children/CIndependenceMetropolis.h"


/**
 *
 */
CMCMC* CMCMCFactory::buildMCMC(const string& type, bool registerWithManager) {
  CMCMC *mcmc = 0;

  if (type == "" || type == PARAM_METROPOLIS_HASTINGS)
    mcmc = new CIndependenceMetropolis();
  else
    CError::errorUnknown(PARAM_MCMC, type);

  if (registerWithManager)
    CMCMCManager::Instance()->addMCMC(mcmc);

  return mcmc;
}
