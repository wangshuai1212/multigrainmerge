#include "LoopACGrGrPolyAction.h"
#include "Factory.h"
#include "Parser.h"
#include "Conversion.h"
#include "FEProblem.h"

registerMooseAction("multigrainmergeApp", LoopACGrGrPolyAction, "add_kernel");

InputParameters
LoopACGrGrPolyAction::validParams()
{
  InputParameters params = Action::validParams();

  params.addClassDescription("Set up ACGrGrPoly kernels for all dofs");
  params.addRequiredParam<unsigned int>("op_num", "specifies the total number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addRequiredParam<std::string>("liquid", "argument of mobility");
  //params.addRequiredParam<std::string>("c", "concentration");
  params.addParam<bool>("variable_mobility", true, "The mobility is a function of any MOOSE variable");
  params.addCoupledVar("args", "Vector of nonlinear variable arguments that L depends on");
  params.deprecateCoupledVar("args", "coupled_variables", "02/27/2024");
////  params.addRequiredCoupledVar("liquid", "the liquid around the multigrain");

  return params;
}


LoopACGrGrPolyAction::LoopACGrGrPolyAction(const InputParameters & params)
  : Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
    //,_liquid(getParam<std::string>("liquid"))
    ,_liquid(getParam<std::string>("liquid"))
    //,_c(getParam<std::string>("c"))
{
}

void
LoopACGrGrPolyAction::act()
{
  for (unsigned int op = 0; op < _op_num; ++op)
  {
     std::string var_name = _var_name_base + Moose::stringify(op);   //gr0 gr1 ...
     std::vector<VariableName> v;
     v.resize(_op_num - 1);                                         // v: coupled gr

       unsigned int ind = 0;
        for (unsigned int j = 0; j < _op_num; ++j)
          if (j != op)
          v[ind++] = _var_name_base + Moose::stringify(j);       // gr5; v: gr0,gr1,gr2,gr3,gr4,gr6...
       
//  create kernels

     {
      
      InputParameters params = _factory.getValidParams("GrainACGrGrPoly");  //create kernel from ACGrGrPoly
      params.set<NonlinearVariableName>("variable") = var_name;        //kernel var
      params.set<std::vector<VariableName>>("v") = v;                  //coupled var  to v
      //params.set<std::vector<VariableName>>("rho") = _liquid;           //coupled with CH op
      params.set<NonlinearVariableName>("rho") = "rho";
      
      params.applyParameters(parameters());

      std::string kernel_name = "GrainACBulk_" + var_name;                  //set kernel name to ACBulk_gri
      _problem->addKernel("GrainACGrGrPoly", kernel_name, params);
     }
     

     {
      InputParameters params = _factory.getValidParams("GrainACInterface");
      params.set<NonlinearVariableName>("variable") = var_name;
      params.applyParameters(parameters());

      std::string kernel_name = "GrainACInt_" + var_name;
      _problem->addKernel("GrainACInterface", kernel_name, params);
     }
      
     { 
      InputParameters params = _factory.getValidParams("TimeDerivative");
      params.set<NonlinearVariableName>("variable") = var_name;
      params.set<bool>("implicit") = true;
      params.applyParameters(parameters());

      std::string kernel_name = "IE_" + var_name;
      _problem->addKernel("TimeDerivative", kernel_name, params);
    }


  }
}

