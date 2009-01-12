//============================================================================
// Name        : CLayerManager.h
// Author      : S.Rasmussen
// Date        : 3/03/2008
// Copyright   : Copyright NIWA Science �2008 - www.niwa.co.nz
// Description : This class is responsible for maintaining a list of our
//       Layers. Like all classes it is responsible for the validate and
//       build calls to the children.
//
//       Layers are a very dynamic concept. During the execution of a process
//       a layer can be defined. This layer is overlayed against the World
//       grid and a check against it's parameters will determine if this
//       World-Square is a valid one for this process or not.
//
//       i.e. We have a layer called Depth. This tells us the depth of water
//       at each World-Square. We know that the fish we are looking for
//       only hang out in Water Depth 250-300M. So we can put a layer_min
//       and layer_max of 250 and 300. Then we can apply this layer to a
//       process. We know this process (e.g Recruitment) will only affect the
//       World-Squares with a depth of 250-300 (Inclusive).
//
//       Different types of layers have different functionalities. You can
//       check individual functionalites in "Layers/*.h"
//
// $Date$
//============================================================================
#ifndef CLAYERMANAGER_H_
#define CLAYERMANAGER_H_

// Local Headers
#include "../BaseClasses/CBaseBuildableObject.h"
#include "CLayerManager.h"

// Classes
class CLayer;
class CNumericLayer;
class CStringLayer;

//**********************************************************************
//
//
//**********************************************************************
class CLayerManager : public CBaseBuildableObject {
public:
  static CLayerManager*     Instance();
  static void               Destroy();

  // Functions
  void                      addLayer(CLayer* Layer);
  CLayer*                   getLayer(int index);
  CNumericLayer*            getNumericLayer(string Label);
  CStringLayer*             getStringLayer(string Label);
  int                       getLayerCount() { return (int)vLayerList.size(); }
  void                      clone(CLayerManager *Manager);
  void                      validate();
  void                      build();
  virtual                   ~CLayerManager();

protected:
  // Functions
  CLayerManager();

  // Variables
  vector<CLayer*>           vLayerList;

private:
  static boost::thread_specific_ptr<CLayerManager> clInstance;
};

#endif /*CLAYERMANAGER_H_*/