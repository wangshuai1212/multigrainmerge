#pragma once

#include "ADKernelGrad.h"

class GrainSplitCHWRes : public ADKernelGrad
{
public:
  static InputParameters validParams();

  GrainSplitCHWRes(const InputParameters & parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;
   

  const ADVariableValue & _bnd;
  const Real & _m0;
  const Real & _k;

};