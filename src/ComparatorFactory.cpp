#include "ComparatorFactory.h"

#include "ComparatorDiffLib.hpp"
#include "ComparatorLongestMatch.h"
#include "ComparatorLevenshteinDistance.h"

void ComparatorFactory::generate_comparator(std::unique_ptr<AbstractTestComparator> &comp,
                                    const std::string& f)
{
    if(f=="LongestMatch"){
        std::unique_ptr<AbstractTestComparator> c{new ComparatorLongestMatch()};
        comp = std::move(c);
        return;
    }
    if(f=="DiffLib"){
        std::unique_ptr<AbstractTestComparator> c{new ComparatorDiffLib()};
        comp = std::move(c);
        return;
    }
    if(f=="Levenshtein"){
        std::unique_ptr<AbstractTestComparator> c{new ComparatorLevenshteinDistance()};
        comp = std::move(c);
        return; 
    }
    return;
}
   
std::vector<std::string> ComparatorFactory::comparator_list(){
    return {    
            "LongestMatch",
            "DiffLib",
            "Levenshtein"
            };
}
   
   
std::vector<std::tuple<std::string,std::string>> ComparatorFactory::comparatordescription_list()
{
    std::vector<std::tuple<std::string,std::string>> desc;
    desc.push_back(std::make_tuple(
                    "LongestMatch",
                    "finds the longest common sequence in two files."));
    desc.push_back(std::make_tuple(
                    "DiffLib",
                    "uses a method similar to Pythons Difflib to measure"
                    " the difference between two files."));
    desc.push_back(std::make_tuple(
                    "Levenshtein",
                    "calculated the Levenshtein distance between two files."));      
           
    return desc;       
}


