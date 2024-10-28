#include "DarcyVelocity.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("multigrainmergeApp", DarcyVelocity);

InputParameters
DarcyVelocity::validParams()
{
   InputParameters params  = VectorAuxKernel::validParams();

   params.addRequiredCoupledVar("pressure","the pressure field");

   return params;

}


DarcyVelocity::DarcyVelocity(const InputParameters & parameters):
       VectorAuxKernel(parameters),
       _p_grad(coupledGradient("pressure")),
       _permeability(getADMaterialProperty<Real>("permeability")),
       _viscosity(getADMaterialProperty<Real>("viscosity"))
       {}


RealVectorValue
DarcyVelocity::computeValue()
{
    return -MetaPhysicL::raw_value(_permeability[_qp] / _viscosity[_qp]) * _p_grad[_qp];
}
