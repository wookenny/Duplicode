#pragma once
#include "AbstractTetComparator.h"
#include <string>

class ComparatorIdentical : public AbstractTetComparator{
    public:
        double operator()(const std::string& c1, const std::string& c2) const{
            return c1==c2;
        } 
}; 
