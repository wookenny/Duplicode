#include "ComparatorFactory.h"


#include "ComparatorDiffLib.hpp"
#include "ComparatorIdentical.hpp"
#include "ComparatorLongestMatch.h"
#include "ComparatorLevenshteinDistance.h"
#include "ComparatorUncommonStrings.h"
//TODO: add a function to map substrings to comparator if only one comp. is matching

void ComparatorFactory::generate_comparator(std::unique_ptr<AbstractComparator> &comp,
                                    const std::string& f)
{
    std::string c;
    if (f=="")
        c=="DiffLib";
    else
        c = fuzzy_match(f, comparator_list());

    if(c=="Identical"){
        std::unique_ptr<AbstractComparator> c{new ComparatorIdentical()};
        comp = std::move(c);
        return;
    }
    if(c=="LongestMatch"){
        std::unique_ptr<AbstractComparator> c{new ComparatorLongestMatch()};
        comp = std::move(c);
        return;
    }
    if(c=="DiffLib"){
        std::unique_ptr<AbstractComparator> c{new ComparatorDiffLib()};
        comp = std::move(c);
        return;
    }
    if(c=="Levenshtein"){
        std::unique_ptr<AbstractComparator> c{new ComparatorLevenshteinDistance()};
        comp = std::move(c);
        return; 
    }
    if(c=="UncommonStrings"){
        std::unique_ptr<AbstractComparator> c{new ComparatorUncommonStrings()};
        comp = std::move(c);
        return; 
    }
    return;
}
   
std::vector<std::string> ComparatorFactory::comparator_list(){
    return {
            "Identical",    
            "LongestMatch",
            "DiffLib",
            "Levenshtein",
            "UncommonStrings"
            };
}
   
std::vector<std::tuple<std::string,std::string>> ComparatorFactory::comparatordescription_list()
{
    std::vector<std::tuple<std::string,std::string>> desc;
    desc.push_back(std::make_tuple(
                    "Identical",
                    "checks whether two files are identical."));
    desc.push_back(std::make_tuple(
                    "LongestMatch",
                    "finds the longest common sequence in two files."));
    desc.push_back(std::make_tuple(
                    "DiffLib",
                    "uses a method similar to Pythons Difflib to measure"
                    " the difference between two files."));
    desc.push_back(std::make_tuple(
                    "Levenshtein",
                    "calculates the Levenshtein distance between two files.")); 
     desc.push_back(std::make_tuple(
                    "UncommonStrings",
                    "counts the number of strings used in both files "
                    " but no other group. Strings might appear"
                    " in other files of both groups."));                      
           
    return desc;       
}


