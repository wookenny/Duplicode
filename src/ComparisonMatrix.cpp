/**
Copyright (c) 2014 Torsten J. Gellert, Jan-Phillip W. Kappmeier

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

#include "ComparisonMatrix.h"

std::string get_group(const std::string &s){ 
    std::vector<std::string> vec = split(s, '/');
    return (s[0]=='/')?vec[1]:vec[0]; 
}

void ComparisonMatrix::addCodes(std::vector<std::string> &codes){
    //TODO find group numbers!
    //TODO: use codefiles instead of strings!
    codeFiles_.clear();
    for(auto s: codes){
        std::string group = get_group(s);
        if(codeFiles_.find(group)==codeFiles_.end()){
            std::vector<std::string> files = {s};
            codeFiles_[group] = files;
        }else{
            codeFiles_[group].push_back(s);
        }              
    }    
    calculated_ = false;
}

void ComparisonMatrix::print_files() const{
    std::cout<<"files in comparison matrix:"<<std::endl;
    for ( auto it = codeFiles_.begin(); it != codeFiles_.end(); ++it ){
        std::cout << " " << it->first << ":\n";
        for(const std::string& f: it->second)
            std::cout<<"\t"<<f<<"\n";   
    }     
    std::cout << std::endl;
}
