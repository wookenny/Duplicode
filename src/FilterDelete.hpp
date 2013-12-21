#pragma once
#include "AbstractTetFilter.h"
class FilterDelete : public AbstractTetFilter{

     public:
        std::string operator()(const std::string& s) const {return "";} 
};
