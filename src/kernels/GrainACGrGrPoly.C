//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GrainACGrGrPoly.h"

registerMooseObject("multigrainmergeApp", GrainACGrGrPoly);

InputParameters
GrainACGrGrPoly::validParams()
{
  InputParameters params = ACGrGrBase::validParams();
  params.addClassDescription("Grain-Boundary model poly-crystalline interface Allen-Cahn Kernel");

  params.addRequiredCoupledVar("rho","coupled var");
  return params;
}

GrainACGrGrPoly::GrainACGrGrPoly(const InputParameters & parameters)
  : ACGrGrBase(parameters), _gamma(getMaterialProperty<Real>("gamma_asymm")),
  _mo(getMaterialProperty<Real>("Mo"))
  ,_rho(coupledValue("rho"))
{
}

Real
GrainACGrGrPoly::computeDFDOP(PFFunctionType type)
{
  // Sum all other order parameters
  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _op_num; ++i)
    SumEtaj += (*_vals[i])[_qp] * (*_vals[i])[_qp];

  //Sum all other order parameters ^3
  Real SumEtajrd = 0.0;
  for (unsigned int c = 0; c < _op_num; ++c)
    SumEtajrd += (*_vals[c])[_qp] * (*_vals[c])[_qp] * (*_vals[c])[_qp];

  // Calculate either the residual or Jacobian of the grain growth free energy
  switch (type)
  {
    case Residual:
    {
      return _rho[_qp]*_rho[_qp]*(1-_rho[_qp])*(1-_rho[_qp]) + 2*(_rho[_qp]*_rho[_qp] + 6*(1-_rho[_qp])*SumEtaj - 4*(2-_rho[_qp])*SumEtajrd + 3*SumEtaj*SumEtaj);
    }

    case Jacobian:
    {
      return _mo[_qp] * _mu[_qp] *
             (_phi[_j][_qp] * (3.0 * _u[_qp] * _u[_qp] - 1.0 + 2.0 * _gamma[_qp] * SumEtaj));
    }

    default:
      mooseError("Invalid type passed in");
  }
}

Real
GrainACGrGrPoly::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i = 0; i < _op_num; ++i)
    if (jvar == _vals_var[i])
    {
      // Derivative of SumEtaj
      const Real dSumEtaj = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp];
      const Real dDFDOP = _mo[_qp] *_mu[_qp] * 2.0 * _gamma[_qp] * _u[_qp] * dSumEtaj;

      return _L[_qp] * _test[_i][_qp] * dDFDOP;
    }

  return 0.0;
}
