//============================================================================
// Name        : CBaseObject.h
// Author      : S.Rasmussen
// Date        : 30/04/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description : This class is the very BASE of all classes. Every class
//    in the application must inherit from this in some form.
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================
#ifndef CBASEOBJECT_H_
#define CBASEOBJECT_H_

// Global Headers
#include <map>
#include <boost/foreach.hpp>
#include <boost/thread/tss.hpp>

// Local Headers
#include "../CConfiguration.h"
#include "../CRuntimeController.h"
#include "../Translations/Translations.h"

// Namespaces
using std::map;

// Defines
#define foreach BOOST_FOREACH // Make the boost foreach a bit nicer.

//**********************************************************************
//
//
//**********************************************************************
class CBaseObject {
public:
	CBaseObject(CBaseObject *Object = 0);
	virtual                    ~CBaseObject();
	void                       setLabel(string value) { sLabel = value; }
	string                     getLabel() { return sLabel; }
	double*                    getEstimableVariable(string name);

protected:
  // Functions
  void                       print(string output);
  void                       println(string output);
  string                     convertDouble(double value);
  string                     convertInt(int value);
  void                       registerEstimable(string name, double *variable);
  void                       registerEstimable(string name, int index, double *variable);
  void                       errorMissing(string value);
  void                       errorLessThan(string less, string more);
  void                       errorLessThanEqualTo(string less, string more);
  void                       errorGreaterThan(string more, string less);
  void                       errorGreaterThanEqualTo(string more, string less);
  void                       errorNotEqual(string value1, string value2);
  void                       errorEqualTo(string value1, string value2);
  void                       errorSupported(string value);

  // Variables
  CConfiguration             *pConfig;
  CRuntimeController         *pRuntimeController;
  bool                       bVerbose;
  string                     sLabel;
  map<string, double*>       mEstimables;
};

#endif /*CBASEOBJECT_H_*/