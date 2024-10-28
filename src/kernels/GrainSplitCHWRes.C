#include "GrainSplitCHWRes.h"

registerMooseObject("multigrainmergeApp", GrainSplitCHWRes);

InputParameters
GrainSplitCHWRes::validParams()
{
  auto params = ADKernelGrad::validParams();
  params.addClassDescription("SplitCHWres with mobility from auxkernel var");
  params.addRequiredCoupledVar("grainboundary", "The variable representing gb");
  params.addRequiredParam<Real>("M0", "Mobility_0");
  params.addRequiredParam<Real>("k", "$M = M_0e^{-bnd/k}$");
  return params;
}

GrainSplitCHWRes::GrainSplitCHWRes(const InputParameters & parameters) : ADKernelGrad(parameters), 
 _bnd(adCoupledValue("grainboundary")),
 _m0(getParam<Real>("M0")),
 _k(getParam<Real>("k"))
{}

ADRealVectorValue
GrainSplitCHWRes::precomputeQpResidual()
{ 

  return _grad_u[_qp] * _m0 * std::exp(- _k/_bnd[_qp]);
}
