//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GrainGBEvolution.h"

registerMooseObject("multigrainmergeApp", GrainGBEvolution);
registerMooseObject("multigrainmergeApp", ADGrainGBEvolution);

template <bool is_ad>
InputParameters
GrainGBEvolutionTempl<is_ad>::validParams()
{
  InputParameters params = GrainGBEvolutionBaseTempl<is_ad>::validParams();
  params.addRequiredParam<Real>("GBenergy", "Grain boundary energy in J/m^2");
  return params;
}

template <bool is_ad>
GrainGBEvolutionTempl<is_ad>::GrainGBEvolutionTempl(const InputParameters & parameters)
  : GrainGBEvolutionBaseTempl<is_ad>(parameters), _GBEnergy(this->template getParam<Real>("GBenergy"))
{
}

template <bool is_ad>
void
GrainGBEvolutionTempl<is_ad>::computeQpProperties()
{
  // eV/nm^2
  this->_sigma[this->_qp] = _GBEnergy * this->_JtoeV * (this->_length_scale * this->_length_scale);

  GrainGBEvolutionBaseTempl<is_ad>::computeQpProperties();
}

template class GrainGBEvolutionTempl<false>;
template class GrainGBEvolutionTempl<true>;
