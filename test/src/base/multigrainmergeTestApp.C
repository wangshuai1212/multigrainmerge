//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "multigrainmergeTestApp.h"
#include "multigrainmergeApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
multigrainmergeTestApp::validParams()
{
  InputParameters params = multigrainmergeApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

multigrainmergeTestApp::multigrainmergeTestApp(InputParameters parameters) : MooseApp(parameters)
{
  multigrainmergeTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

multigrainmergeTestApp::~multigrainmergeTestApp() {}

void
multigrainmergeTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  multigrainmergeApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"multigrainmergeTestApp"});
    Registry::registerActionsTo(af, {"multigrainmergeTestApp"});
  }
}

void
multigrainmergeTestApp::registerApps()
{
  registerApp(multigrainmergeApp);
  registerApp(multigrainmergeTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
multigrainmergeTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  multigrainmergeTestApp::registerAll(f, af, s);
}
extern "C" void
multigrainmergeTestApp__registerApps()
{
  multigrainmergeTestApp::registerApps();
}
