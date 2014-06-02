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
#include <algorithm>
#include <omp.h>

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
            CodeFile code = CodeFile(s,root_);
            std::vector<CodeFile> files = {code};
            codeFiles_[group] = files;
        }else{
            codeFiles_[group].push_back(CodeFile(s,root_));
        }              
    }
    comparisonResult_.clear() ;
    filterCodes();
    calculated_ = false;
}

void ComparisonMatrix::calculateComparisionMatrix(){
    //how to get group names
    if(calculated_) return;
    
    std::vector<std::string> keys;
    std::vector<std::tuple<std::string,std::string>> pairs;
    for(auto kv : codeFiles_) 
        keys.push_back(kv.first);
    std::sort(std::begin(keys),std::end(keys));    
    for(size_t i=0; i<keys.size();++i)
        for(size_t j=i+1; j<keys.size();++j){
            pairs.push_back(std::make_tuple(keys[i],keys[j]));
            comparisonResult_[keys[i]][keys[j]] = 0;    
        }
        

    
    std::cout<<"comparisons in total: "<<pairs.size()<<std::endl;

    int counter = 0;
    //48 threads is much faster than 6: 10.3s vs 18.3s
    //no omp at all vs 1 thread:        40s vs 39s        
    #pragma omp parallel for schedule(dynamic,8) num_threads(48)
    for(size_t i=0; i<pairs.size();++i){
        counter+=1;
        std::string group1 = std::get<0>(pairs[i]);
        std::string group2 = std::get<1>(pairs[i]);
        //std::cout<< "comparing: "<<group1<<" and "<<group2<<std::endl;
        double sim = compare_groups(codeFiles_[group1], codeFiles_[group2]);
        comparisonResult_[group1][group2] = sim;   
        if( omp_get_thread_num() == 0){
            std::cout<<"\r  "<< 100*counter/pairs.size()<<"%     ";
            std::cout.flush();
        }
    }
    std::cout<<"\r  Done  "<<std::endl;
    //std::cout<<"\r";
    //std::cout<<"100 %        "<<std::endl;
    calculated_ = true;
}

void ComparisonMatrix::filterCodes(){
    for ( auto &group : codeFiles_)
        for(CodeFile& f: group.second)
            f.filtered_content = comparator_->filter(f.content);
}

void ComparisonMatrix::print_files() const{
    std::cout<<"files in comparison matrix:"<<std::endl;
    for ( auto &group : codeFiles_){
        std::cout << " " << group.first << ":\n";
             for(const CodeFile& f: group.second)
                    std::cout<<"\t"<<f.filename<<"\n";  
   } 
    std::cout << std::endl;
}

double ComparisonMatrix::compare_groups(const std::vector<CodeFile>& g1,
                                    const std::vector<CodeFile>& g2) const
{
    double match = 0;
    switch(comp_mode_){
        case SingleBestMatch:
            //First implementation
            for(const CodeFile& f1: g1)
                for(const CodeFile& f2: g2){
                     double m = comparator_->compare(f1.filtered_content,
                                                     f2.filtered_content);
                     match = std::max(match,m);
                     //std::cout<<f1.filename<<" vs "<<f2.filename<<"   "<<m <<std::endl;    
                }
            break;
        case SameNameMultiply:
            //First implementation
            match = 1.0;
            for(const CodeFile& f1: g1)
                for(const CodeFile& f2: g2){
                    if(split(f1.filename,'/').back()!=
                       split(f2.filename,'/').back())
                        continue;     
                     double m = comparator_->compare(f1.filtered_content,
                                                     f2.filtered_content);
                     match *= m;
                     //std::cout<<f1.filename<<" vs "<<f2.filename<<"   "<<m <<std::endl;    
                }
            break;
        case SameNameMax:
            //First implementation
            for(const CodeFile& f1: g1)
                for(const CodeFile& f2: g2){
                    if(split(f1.filename,'/').back()!=
                       split(f2.filename,'/').back())
                        continue;     
                     double m = comparator_->compare(f1.filtered_content,
                                                     f2.filtered_content);
                     match = std::max(match,m);
                     //std::cout<<f1.filename<<" vs "<<f2.filename<<"   "<<m <<std::endl;    
                }
            break;
        default: std::cout<<"WARNING: NO VALID MODE FOR COMPARISON"<<std::endl;
    } 
                           
    return match;                               
}

std::vector<std::tuple<std::string,std::string,double>> 
                               ComparisonMatrix::get_sorted_matches()
{   
    calculateComparisionMatrix();
    std::vector<std::tuple<std::string,std::string,double>> matches;

    for(const auto &i: comparisonResult_){
        for(const auto &j: comparisonResult_[i.first]){
            double sim = comparisonResult_[i.first][j.first];     
            matches.push_back(std::make_tuple(i.first,j.first,sim));
        }
    }
    
    typedef std::tuple<std::string,std::string,double> Elem;
    //sort them
    auto order = [](const Elem& a, const Elem& b) { 
        return std::get<2>(a) > std::get<2>(b); 
    };
    std::sort(begin(matches),end(matches),order);
    return matches;
} 
