#include "ComparatorFactory.h"

//Further Compators can be added like this:
// * include the needed header
// * add the name and the class to the map below
// e.g.,   {"MyComp", &createInstance<MyComparator>},
//
// important: implement  name() and description() method properly


#include "ComparatorDiffLib.hpp"
#include "ComparatorIdentical.hpp"
#include "ComparatorLongestMatch.h"
#include "ComparatorLevenshteinDistance.h"
#include "ComparatorUncommonStrings.h"

std::map<std::string, AbstractComparator*(*)()> ComparatorFactory::map = 
{
    {"DiffLib",         &createInstance<ComparatorDiffLib>},
    {"Identical",       &createInstance<ComparatorIdentical>},
    {"LongestMatch",    &createInstance<ComparatorLongestMatch>},
    {"Levenshtein",     &createInstance<ComparatorLevenshteinDistance>},
    {"UncommonStrings", &createInstance<ComparatorUncommonStrings>}
};

/* ---------------------  HINT!! ----------------------------------
  Generic part below, no modification for new comparators needed!
-------------------------------------------------------------------*/

void ComparatorFactory::generate_comparator(std::unique_ptr<AbstractComparator> &comp,
                                    const std::string& f)
{
    std::string c;
    if (f=="")
        c==map.begin()->first;
    else
        c = fuzzy_match(f, comparator_list());
           
    std::unique_ptr<AbstractComparator> co{ map[c]() };
    comp = std::move(co);
    return;


}
    
std::vector<std::string> ComparatorFactory::comparator_list(){
    std::vector<std::string> list;
    for(const auto &kv: map)
        list.push_back(kv.first);
    return list;
}
   
std::vector<std::tuple<std::string,std::string>> ComparatorFactory::comparatordescription_list()
{
    std::vector<std::tuple<std::string,std::string>> desc;
    for(const auto &kv: map){
        std::unique_ptr<AbstractComparator> comp{ map[kv.first]() };
        desc.push_back( std::make_tuple( kv.first,comp->name() ) );
    }
    return desc;       
}


