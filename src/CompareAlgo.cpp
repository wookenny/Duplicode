#include "CompareAlgo.h"
#include <iostream>

double CompareAlgo::compare(std::string code1, std::string code2){
    for(const auto& f: filters_){
        code1 = (*f)(code1);
        code2 = (*f)(code2);
    }

    if(comparator_!=nullptr)    
        return (*comparator_)(code1,code2);
    else
        std::cerr<<"Warning: No operator to compare code given!"<<std::endl;
    return 0;
}

void CompareAlgo::addFilter(std::unique_ptr<AbstractTetFilter>& filter_ptr){
    filters_.push_back(std::move(filter_ptr));
}
/*
void CompareAlgo::addFilter(std::unique_ptr<AbstractTetFilter>&& filter_ptr){
    filters_.push_back(filter_ptr);
}
*/

void CompareAlgo::setComparator(std::unique_ptr<AbstractTetComparator>& c){
    comparator_ = std::move(c);
}   
