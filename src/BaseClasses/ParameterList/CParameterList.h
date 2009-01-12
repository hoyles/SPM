//============================================================================
// Name        : CParameterList.h
// Author      : S.Rasmussen
// Date        : 8/01/2009
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description :
// $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
//============================================================================
#ifndef CPARAMETERLIST_H_
#define CPARAMETERLIST_H_

// Global Headers
#include <map>
#include <vector>
#include <string>

// Using
using std::map;
using std::vector;
using std::string;

//**********************************************************************
//
//
//**********************************************************************
class CParameterList {
public:
  // Functions
  CParameterList(CParameterList *Object = 0);
  virtual                     ~CParameterList();
  void                        addParameter(string name, string value);
  void                        registerAllowed(string name);
  string                      getString(string name);
  double                      getDouble(string name);
  int                         getInt(string name);
  bool                        getBool(string name);
  int                         countMatches(string name);
  int                         countParameterValues(string name);
  string                      getMatchFullName(string name, int matchNumber);
  void                        fillVector(vector<string> &list, string name, int offset = 0);
  void                        fillVector(vector<double> &list, string name, int offset = 0);
  void                        fillArray(double *array, int length, string name, int offset = 0);
  void                        fillArray(string *array, int length, string name, int offset = 0);

protected:
  // Functions
  bool                        hasParameter(string name);

  // Variables
  map<string, vector<string> >  mParameters;
  vector<string>                vAllowedParameters;
};

#endif /* CPARAMETERLIST_H_ */