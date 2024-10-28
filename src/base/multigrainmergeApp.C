#include "multigrainmergeApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
multigrainmergeApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  return params;
}

multigrainmergeApp::multigrainmergeApp(InputParameters parameters) : MooseApp(parameters)
{
  multigrainmergeApp::registerAll(_factory, _action_factory, _syntax);
}

multigrainmergeApp::~multigrainmergeApp() {}

void
multigrainmergeApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"multigrainmergeApp"});
  Registry::registerActionsTo(af, {"multigrainmergeApp"});

  /* register custom execute flags, action syntax, etc. here */
  
registerSyntax("LoopACGrGrPolyAction", "Kernels/LoopACGrGrPoly");
registerSyntax("PolycrystalColoringIC", "ICs/lycrystalColoringIC");

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
