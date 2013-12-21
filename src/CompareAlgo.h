#pragma once
#include "AbstractTetComparator.h"
#include "AbstractTetFilter.h"
#include <vector>
#include <memory>
#include <string>
//x
class CompareAlgo{

    
    private:
        std::vector<std::unique_ptr<AbstractTetFilter>> filters_;
        std::unique_ptr<AbstractTetComparator> comparator_;

    public:
        CompareAlgo() = default;      

        void addFilter(std::unique_ptr<AbstractTetFilter>& );
        //TODO: GCC Bug! Try Clang or find out whats wronmg with this!
       // void addFilter(std::unique_ptr<AbstractTetFilter>&& );

        void setComparator(std::unique_ptr<AbstractTetComparator>& );        
        double compare(std::string, std::string);  

};
