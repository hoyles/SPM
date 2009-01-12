//============================================================================
// Name        : CPenaltyFactory.cpp
// Author      : S.Rasmussen
// Date        : 12/01/2009
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local headers
#include "CPenaltyFactory.h"
#include "../../Translations/Translations.h"
#include "../CPenalty.h"

//**********************************************************************
// CPenalty* CPenaltyFactory::buildPenalty(string type)
// Build our penalty based on type
//**********************************************************************
CPenalty* CPenaltyFactory::buildPenalty(string type) {

  if (type == PARAM_BASIC)
    return new CPenalty();
  else
    throw string("Unknown type: " + type); // TODO: FIX THIS

  return 0;
}