#pragma once
#include "AbstractTetFilter.h"
class FilterIdentity : public AbstractTetFilter{

     public:
        std::string operator()(const std::string& s) const {return s;} 
};
