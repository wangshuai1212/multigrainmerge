#include "multigrainmergeApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
multigrainmergeApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

multigrainmergeApp::multigrainmergeApp(InputParameters parameters) : MooseApp(parameters)
{
  multigrainmergeApp::registerAll(_factory, _action_factory, _syntax);
}

multigrainmergeApp::~multigrainmergeApp() {}

void
multigrainmergeApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<multigrainmergeApp>(f, af, s);
  Registry::registerObjectsTo(f, {"multigrainmergeApp"});
  Registry::registerActionsTo(af, {"multigrainmergeApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
multigrainmergeApp::registerApps()
{
  registerApp(multigrainmergeApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
multigrainmergeApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  multigrainmergeApp::registerAll(f, af, s);
}
extern "C" void
multigrainmergeApp__registerApps()
{
  multigrainmergeApp::registerApps();
}
