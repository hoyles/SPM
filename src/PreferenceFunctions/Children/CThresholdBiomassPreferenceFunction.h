//============================================================================
// Name        : CThresholdBiomassPreferenceFunction.h
// Author      : S.Rasmussen
// Date        : 16/01/2009
// Copyright   : Copyright NIWA Science �2009 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================
#ifndef CTHRESHOLDBIOMASSPREFERENCEFUNCTION_H_
#define CTHRESHOLDBIOMASSPREFERENCEFUNCTION_H_

// Local headers
#include "../CPreferenceFunction.h"

// Forward-Decs
class CSelectivity;

//**********************************************************************
//
//
//**********************************************************************
class CThresholdBiomassPreferenceFunction : public CPreferenceFunction {
public:
  // Functions
  CThresholdBiomassPreferenceFunction();
  virtual                     ~CThresholdBiomassPreferenceFunction();
  double                     getResult(int RIndex, int CIndex, int TRIndex, int TCIndex);
  void                       validate();

protected:
  // Variables
  vector<string>             vCategoryNames;
  vector<int>                vCategories;
  vector<string>             vSelectivityNames;
  vector<CSelectivity*>      vSelectivities;
  double                     dSizeWeight;
  double                     dBiomass;
  double                     dLambda;
};

#endif /* CTHRESHOLDBIOMASSPREFERENCEFUNCTION_H_ */