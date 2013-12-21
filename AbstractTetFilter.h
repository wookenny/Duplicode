#pragma once
#include <string>

class AbstractTetFilter{
    
    public:
        virtual std::string operator()(const std::string&) const = 0; 
};
