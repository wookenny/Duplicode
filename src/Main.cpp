#include "CompareAlgo.h"
#include "FilterIdentity.hpp"
#include "FilterDelete.hpp"
#include "AbstractTetFilter.h"
#include "ComparatorIdentical.hpp" 
#include "ComparatorLongestMatch.h" 
#include <string>
#include <memory>
#include <iostream>

using namespace std;

int main(){

    CompareAlgo comp;
    string c1 = "este er rewrd w";
    string c2 = "asta ar lkewrd";

    std::unique_ptr<AbstractTetFilter> filter{new FilterIdentity()};
    comp.addFilter(filter);

    cout << "Comparision without comp.: "<< comp.compare(c1,c2) <<endl;
    

    std::unique_ptr<AbstractTetComparator> comparator{new ComparatorLongestMatch()};
    comp.setComparator(comparator);

    cout << "Comparision (identical): "<< comp.compare(c1,c2) <<endl;

    filter = move( unique_ptr<AbstractTetFilter>{new FilterDelete()} );
    comp.addFilter(filter);
    cout << "Comparision (identical, filter: delete): "<< comp.compare(c1,c2) <<endl;


    return 0;
}
