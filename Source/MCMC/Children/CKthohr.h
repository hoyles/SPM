/*
 * CKthohr.h
 *
 *  Created on: 11/01/2020
 *      Author: zaita
 */

#ifndef SOURCE_MCMC_CHILDREN_CKTHOHR_H_
#define SOURCE_MCMC_CHILDREN_CKTHOHR_H_

// headers
#include "../CMCMC.h"

/**
 * Class definition
 */
class CKthohr : public CMCMC {
public:
  CKthohr();
  virtual                   ~CKthohr() = default;
  void                       validate();
  void                       build();
  void                       execute();

private:
  string                    sAlgorithm = "";
};

#endif /* SOURCE_MCMC_CHILDREN_CKTHOHR_H_ */
