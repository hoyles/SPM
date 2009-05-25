//============================================================================
// Name        : CEstimate.h
// Author      : S.Rasmussen
// Date        : 27/02/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date$
//============================================================================
#ifndef CESTIMATE_H_
#define CESTIMATE_H_

// Local Headers
#include "../BaseClasses/CBaseBuild.h"

// Classes
class CPrior;

//**********************************************************************
//
//
//**********************************************************************
class CEstimate : public CBaseBuild {
public:
	CEstimate();
	virtual                    ~CEstimate();
	CEstimate*                 clone() { return new CEstimate(*this); }
	string                     getParameter() { return sParameter; }
	double                     getLowerBound() { return dLowerBound; }
	double                     getUpperBound() { return dUpperBound; }
	string                     getPrior() { return sPrior; }
  void                       setValue(double value);
	double                     getValue();
	void                       setEnabled(bool value) { bEnabled = value; }
  bool                       getEnabled() { return bEnabled; }
  void                       addValue(double value);
  int                        getValueCount() { return (int)vValueList.size(); }
  void                       loadValue(int index);
  double                     getPriorScore();
  void                       fillSameVector(vector<string> &sames);
  void                       validate();
	void                       build();

protected:
  // Variables
  string                     sParameter;
  double                     dLowerBound;
  double                     dUpperBound;
  string                     sPrior;
  CPrior                     *pPrior;
  double                     *pTarget;
  bool                       bEnabled;
  vector<string>             vSameList;
  vector<double*>            vSameIndex;
  vector<double>             vValueList;
};

#endif /*CESTIMATE_H_*/
