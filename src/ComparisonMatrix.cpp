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
#include <thread>

static const int NUM_THREADS = std::thread::hardware_concurrency()*2;

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
    calculated_ = false;
}

void ComparisonMatrix::compute_via_thread_(int tid) {
    for(size_t i=tid; i<pairs_.size();i+=NUM_THREADS){
        std::string group1 = std::get<0>(pairs_[i]);
        std::string group2 = std::get<1>(pairs_[i]);
        //std::cout<< "comparing: "<<group1<<" and "<<group2<<std::endl;
        std::string hint1, hint2;
        double sim = compare_groups(codeFiles_[group1], codeFiles_[group2],
                                    &hint1, &hint2);
        Similarity simil;
        simil.val = sim;
        simil.hint1 = hint1;
        simil.hint2 = hint2;                               
        comparisonResult_[group1][group2] = simil;   
        if(tid==0){
            std::cout<<"\r "<<100*i/pairs_.size()<<"%  ";
            std::cout.flush();
        }
          
    }
}

void ComparisonMatrix::calculateComparisionMatrix(){
    //how to get group names
    if(calculated_) return;
    
    filterCodes();
    
    std::vector<std::string> keys;
    pairs_.clear();
    for(auto kv : codeFiles_) 
        keys.push_back(kv.first);
    std::sort(std::begin(keys),std::end(keys));    
    for(size_t i=0; i<keys.size();++i)
        for(size_t j=i+1; j<keys.size();++j){
            pairs_.push_back(std::make_tuple(keys[i],keys[j]));
            Similarity simil;
            comparisonResult_[keys[i]][keys[j]] = simil;    
        }
        

    
    std::cout<<"comparisons in total: "<<pairs_.size()<<std::endl;
    std::cout<<"using "<<NUM_THREADS<<" threads for processing" <<std::endl;
    //c++11 thread version!:
    std::thread t[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        t[i] = std::thread(&ComparisonMatrix::compute_via_thread_, this, i);
    }

    //Join the threads with the main thread
    for (int i = 0; i < NUM_THREADS; ++i) {
        t[i].join();
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
                                    const std::vector<CodeFile>& g2, 
                                    std::string* h1, std::string* h2) const
{
    double match = 0;
    switch(comp_mode_){
        case SingleBestMatch:
            //First implementation
            for(const CodeFile& f1: g1)
                for(const CodeFile& f2: g2){
                     double m = comparator_->compare(f1.filtered_content,
                                                     f2.filtered_content);
                     if(m > match){
                           match = m; *h1 = f1.filename; *h2 = f2.filename; 
                     }  
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
                     if(m > match){
                           match = m; *h1 = f1.filename; *h2 = f2.filename; 
                     }
                     //std::cout<<f1.filename<<" vs "<<f2.filename<<"   "<<m <<std::endl;    
                }
            break;
        default: std::cout<<"WARNING: NO VALID MODE FOR COMPARISON"<<std::endl;
    } 
                           
    return match;                               
}

std::vector<CodeMatch> ComparisonMatrix::get_sorted_matches()
{   
    calculateComparisionMatrix();
    std::vector<CodeMatch> matches;

    for(const auto &i: comparisonResult_){
        for(const auto &j: comparisonResult_[i.first]){
            CodeMatch match;
            auto &comp = comparisonResult_[i.first][j.first];
            match.similarity = comp.val;
            match.group1 = i.first;
            match.group2 = j.first;
            match.file1  = comp.hint1;
            match.file2  = comp.hint2;
            matches.push_back(match);
        }
    }
    
    //sort them
    auto order = [](const CodeMatch& a, const CodeMatch& b) { 
        return a.similarity > b.similarity; 
    };
    std::sort(begin(matches),end(matches),order);
    return matches;
} 
