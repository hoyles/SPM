//============================================================================
// Name        : CEstimateManager.h
// Author      : S.Rasmussen
// Date        : 25/02/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description : This class is responsible for maintaining a list of our
//       estimated variables. This Manager is responsible for the validate,
//       build and execute calls to our children.
//
//       Estimates are variables that will be estimated by our minimiser
//       (ADOLC) before each run of the model. The Minimiser will adjust
//       the value of these variables to try and find a model run that best
//       fits what we have supplied in our observations.
//
// $Date$
//============================================================================
#ifndef CESTIMATEMANAGER_H_
#define CESTIMATEMANAGER_H_

// Local Headers
#include "BaseClasses/CBaseBuildableObject.h"

// Classes
class CEstimate;

//**********************************************************************
//
//
//**********************************************************************
class CEstimateManager : public CBaseBuildableObject {
public:
  static CEstimateManager*  Instance();
  static void               Destroy();

  // Functions
  void                       addEstimate(CEstimate *Estimate);
  int                        getEstimateCount() { return vEstimateList.size(); }
  int                        getEnabledEstimateCount();
  CEstimate*                 getEnabledEstimate(int Index);
  CEstimate*                 getEstimate(int Index);
  CEstimate*                 getEstimate(string Parameter);
  void                       addEstimateValue(string estimate, double value);
  int                        getEstimateValueCount();
  void                       loadEstimateValues(int index);
  void                       clone(CEstimateManager *Manager);
  void                       validate();
  void                       build();
  virtual                    ~CEstimateManager();

protected:
  // Functions
  CEstimateManager();

	// Variables
	vector<CEstimate*>           vEstimateList;

private:
  static boost::thread_specific_ptr<CEstimateManager> clInstance;
};

#endif /*CESTIMATEDPARAMETERMANAGER_H_*/