//============================================================================
// Name        : CAbundanceDensityLayer.cpp
// Author      : S.Rasmussen
// Date        : 16/01/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================

// Local headers
#include "CAbundanceDensityLayer.h"
#include "../../Selectivities/CSelectivityManager.h"
#include "../../World/CWorld.h"
#include "../../Selectivities/CSelectivity.h"
#include "../../Helpers/CError.h"
#include "../../Helpers/ForEach.h"

//**********************************************************************
// CAbundanceDensityLayer::CAbundanceDensityLayer()
// Default Constructor
//**********************************************************************
CAbundanceDensityLayer::CAbundanceDensityLayer() {
  // Variables
  pWorld = CWorld::Instance();

  // Register User allowed parameters
  pParameterList->registerAllowed(PARAM_CATEGORIES);
  pParameterList->registerAllowed(PARAM_SELECTIVITIES);
}

//**********************************************************************
// void CAbundanceDensityLayer::addCategory(string value)
// Add A Category to our Abundance Layer
//**********************************************************************
void CAbundanceDensityLayer::addCategory(string value) {
  vCategoryNames.push_back(value);
}

//**********************************************************************
// void CAbundanceDensityLayer::addSelectivity(string value)
// Add a selectivity to our list.
//**********************************************************************
void CAbundanceDensityLayer::addSelectivity(string value) {
  vSelectivityNames.push_back(value);
}

//**********************************************************************
// void CAbundanceDensityLayer::validate()
// Validate the layer
//**********************************************************************
void CAbundanceDensityLayer::validate() {
 try {
   // Base validate
   CNumericLayer::validate();

   // Populate our Parameters
   pParameterList->fillVector(vCategoryNames, PARAM_CATEGORIES);
   pParameterList->fillVector(vSelectivityNames, PARAM_SELECTIVITIES);

   // Check For Duplicate Categories.
   map<string, int> mList;
   foreach(string Category, vCategoryNames) {
     mList[Category] += 1;
     if (mList[Category] > 1)
       CError::errorDuplicate(PARAM_CATEGORY, Category);
   }

   if(vCategoryNames.size() != vSelectivityNames.size())
     CError::errorListSameSize(PARAM_CATEGORIES,PARAM_SELECTIVITIES);

  } catch (string &Ex) {
    Ex = "CAbundanceDensityLayer.validate(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// void CAbundanceDensityLayer::build()
// Build the layer
//**********************************************************************
void CAbundanceDensityLayer::build() {
  try {
    // Build Selectivities
    CSelectivityManager *pSelectivityManager = CSelectivityManager::Instance();
    pSelectivityManager->fillVector(vSelectivities, vSelectivityNames);

    // Build Categories
    pWorld->fillCategoryVector(vCategories, vCategoryNames);

  } catch (string &Ex) {
    Ex = "CAbundanceDensityLayer.build(" + getLabel() + ")->" + Ex;
    throw Ex;
  }
}

//**********************************************************************
// double CAbundanceDensityLayer::getValue(int RowIndex, int ColIndex, int TargetRow, int TargetCol)
// get Value
//**********************************************************************
double CAbundanceDensityLayer::getValue(int RowIndex, int ColIndex, int TargetRow=0, int TargetCol=0) {
#ifndef OPTIMIZE
  try {
#endif

    double  dResult = 0;

    for (int i = 0; i < pWorld->getAgeSpread(); ++i) {
      for (int j = 0; j < (int)vCategories.size(); ++j) {
         dResult += vSelectivities[j]->getResult(i) * pWorld->getBaseSquare(RowIndex, ColIndex)->getAbundanceInCategoryForAge(i, vCategories[j]);
      }
    }
    //TODO: Scott - Get the value of the cell in the base layer (i.e., the area)
    //              Divide the abundance by the base layer value to get density

    return dResult;

#ifndef OPTIMIZE
  } catch (string &Ex) {
    Ex = "CAbundanceDensityLayer.getValue(" + getLabel() + ")->" + Ex;
    throw Ex;
  }

  return 0.0;
#endif
}

//**********************************************************************
// CAbundanceDensityLayer::~CAbundanceDensityLayer()
// Destructor
//**********************************************************************
CAbundanceDensityLayer::~CAbundanceDensityLayer() {
}
