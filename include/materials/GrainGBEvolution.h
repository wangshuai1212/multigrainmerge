//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GrainGBEvolutionBase.h"

/**
 * Grain boundary energy parameters for isotropic uniform grain boundary energies
 */
template <bool is_ad>
class GrainGBEvolutionTempl : public GrainGBEvolutionBaseTempl<is_ad>
{
public:
  static InputParameters validParams();

  GrainGBEvolutionTempl(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  Real _GBEnergy;
};

typedef GrainGBEvolutionTempl<false> GrainGBEvolution;
typedef GrainGBEvolutionTempl<true> ADGrainGBEvolution;
