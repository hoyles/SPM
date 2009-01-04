//============================================================================
// Name        : CEventMortalityProcess.h
// Author      : S.Rasmussen
// Date        : 9/03/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================
#ifndef CEVENTMORTALITYPROCESS_H_
#define CEVENTMORTALITYPROCESS_H_

// Local Headers
#include "../CProcess.h"

// Classes
class CTimeStepManager;

//**********************************************************************
//
//
//**********************************************************************
class CEventMortalityProcess : public CProcess {
public:
  // Functions
  CEventMortalityProcess(CEventMortalityProcess *Process = 0);
	virtual                    ~CEventMortalityProcess();
	void                       setUMax(double value) { dUMax = value; }
	double                     getUMax() { return dUMax; }
	void                       addYears(int value);
	int                        getYearsCount() { return vYearsList.size(); }
	int                        getYears(int index);
	void                       addLayers(string value);
	int                        getLayersCount() { return vLayersList.size(); }
	string                     getLayers(int index);
	CProcess*                  clone() { return (new CEventMortalityProcess(this)); }
	void                       validate();
	void                       build();
	void                       execute();

protected:
  // Variables
  double                     dUMax;
  double                     dCatch;
  double                     dVulnerable;
  double                     dExploitation;
  CWorldSquare               *pWorldSquare;
  vector<int>                vYearsList;
  vector<string>             vLayersList;
  vector<CNumericLayer*>     vLayersIndex;
  int                        iCurrentYear;
  bool                       bYearMatch;
  CTimeStepManager           *pTimeStepManager;
};

#endif /*CEVENTMORTALITYPROCESS_H_*/