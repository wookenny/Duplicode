#pragma once
#include "AbstractTetComparator.h"
#include <string>

class ComparatorLongestMatch : public AbstractTetComparator{
 
public:
        double operator()(const std::string& c1, const std::string& c2) const;

private:
       double sizeOfMatch(const std::string&, uint,
                          const std::string&, uint) const;
}; 
