#pragma once

#include "Action.h"

class LoopACGrGrPolyAction : public Action
{
    public:

        static InputParameters validParams();

        LoopACGrGrPolyAction(const InputParameters & params);

        virtual void act();

    protected:
        
        const unsigned int _op_num;
        const std::string _var_name_base;
        const std::string _liquid;
        //const std::string _c;

};

