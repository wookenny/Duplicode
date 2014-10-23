/**
Copyright (c) 2014 Torsten J. Gellert

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/

#include "ComparatorUncommonStrings.h"
#include <algorithm>
#include "difflib.h"

double ComparatorUncommonStrings::operator()(const std::string& c1, 
                                             const std::string& c2) const
{
    //find strings used both groups
    std::set<std::string> s1 = strings_from_file_(c1);
    std::set<std::string> s2 = strings_from_file_(c2);
    std::set<std::string> intersect;
    std::set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),
                          std::inserter(intersect,intersect.begin()));
    
    //find uncommon strings
    std::set<std::string> uncommon;
    std::set_intersection(intersect.begin(),intersect.end(),
                          uncommon_strings_.begin(),uncommon_strings_.end(),
                          std::inserter(uncommon,uncommon.begin()));
    //return number of uncommon strings used in both code files                     
    return uncommon.size();
} 
 
void ComparatorUncommonStrings::init( const CodeMap& codes){

    //count number of matches
    std::unordered_map<std::string,int> word_count;
    
    //for each group
    for (const auto& kv : codes) {
        std::set<std::string> strings_used;
        //for each contained file:
        for(const auto& code: kv.second){
            //add all used strings to set
            for(const std::string& s: split(code.filtered_content,' ') ){
                strings_used.insert(s);
            }
        }
        //add words to wordcount
        for(const std::string& s: strings_used){
            if( word_count.find(s)!=word_count.end()){
                word_count[s] += 1; 
            }else{
                word_count[s]  = 1;
            }
        }     
    }
        
     
    
    // build set for stringsd used by twp groups
    uncommon_strings_.clear();
    for(const auto& kv : word_count)
        if(kv.second == 2)
        uncommon_strings_.insert(kv.first);
    
    std::cout<<"Found "<<uncommon_strings_.size()<<" uncommon strings.\n";
}
 
std::set<std::string> ComparatorUncommonStrings::strings_from_file_(const std::string& c){
    std::set<std::string> strings_found;
    for(auto s: split(c,' ')){
        strings_found.insert(s);
    }  
    return strings_found;
}
