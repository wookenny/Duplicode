#pragma once
#include "AbstractTetFilter.h"
class FilterDeleteComments : public AbstractTetFilter{

     public:
        std::string operator()(const std::string& s) const;
};
