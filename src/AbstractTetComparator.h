#pragma once
#include <string>

class AbstractTetComparator{
    public:
        virtual double operator()(const std::string&, const std::string&) const = 0; 
}; 
