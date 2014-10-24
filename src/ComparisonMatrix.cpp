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

#include "ComparisonMatrix.h"
#include <algorithm>
#include <thread>
#include <limits>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

static const int NUM_THREADS = std::thread::hardware_concurrency()*2;

std::string get_group(const std::string &s){ 
    std::vector<std::string> vec = split(s, '/');
    return (s[0]=='/')?vec[1]:vec[0]; 
}

void ComparisonMatrix::addCodes(std::vector<std::string> &codes){
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
    std::cout<<"Collected "<<codes.size()<<" files"<<std::endl;
}

void ComparisonMatrix::compute_via_thread_(int tid) {
    for(size_t i=tid; i<pairs_.size();i+=NUM_THREADS){
        std::string group1 = std::get<0>(pairs_[i]);
        std::string group2 = std::get<1>(pairs_[i]);
        //std::cout<< "comparing: "<<group1<<" and "<<group2<<std::endl;
        std::string hint1, hint2;
        double sim = compare_groups_(codeFiles_[group1], codeFiles_[group2],
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

    max_ = - std::numeric_limits<double>::infinity();
    min_ =   std::numeric_limits<double>::infinity();
    filterCodes();
    
    pairs_.clear();
    auto keys = get_keys();
    for(size_t i=0; i<keys.size();++i)
        for(size_t j=i+1; j<keys.size();++j){
            pairs_.push_back(std::make_tuple(keys[i],keys[j]));
            Similarity simil;
            comparisonResult_[keys[i]][keys[j]] = simil;    
        }

    std::cout<<"groups: "<<keys.size()<<"\t#of comparisons: "
                                      <<pairs_.size()<<"\n";
    std::cout<<"using "<<NUM_THREADS<<" threads for processing" <<std::endl;

    std::vector<std::thread> t;
    for (int i = 0; i < NUM_THREADS; ++i) {
        t.push_back(std::thread(&ComparisonMatrix::compute_via_thread_, this, i));
    }
    //Join the threads with the main thread
    for (int i = 0; i < NUM_THREADS; ++i) {
        t[i].join();
    }
    
    std::cout<<"\r  Done  "<<std::endl;
    //std::cout<<"\r";
    //std::cout<<"100 %        "<<std::endl;
    
    //collect max/min
    for(const auto &i: comparisonResult_){
        for(const auto &j: comparisonResult_[i.first]){
            auto &comp = comparisonResult_[i.first][j.first];
            min_ = std::min(min_, comp.val);
            max_ = std::max(max_, comp.val);
        }
    }
    calculated_ = true;
}

std::vector<std::vector<double>> ComparisonMatrix::get_comp_matrix(){
    calculateComparisionMatrix();
    
    auto keys = get_keys(); 
    //hashmap to matrix
    std::vector<std::vector<double>> result;
    for(uint i = 0; i < keys.size();++i)
        result.push_back(std::vector<double>(keys.size(),0));
    //fill it properly
    for(uint i = 0; i < keys.size();i++)
         for(uint j = i+1; j < keys.size();j++){
            result[j][i]=result[i][j] =  comparisonResult_[keys[i]][keys[j]].val;
         } 
    return result;
}

void ComparisonMatrix::filterCodes(){
    for ( auto &group : codeFiles_)
        for(CodeFile& f: group.second)
            f.filtered_content = comparator_->filter(f.content);

    comparator_->initialize_comparators( codeFiles_ );
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

double ComparisonMatrix::compare_groups_(const std::vector<CodeFile>& g1,
                                    const std::vector<CodeFile>& g2, 
                                    std::string* h1, std::string* h2) const
{
    double match = -std::numeric_limits<double>::infinity();
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

std::vector<std::string> ComparisonMatrix::get_keys() const{
    std::vector<std::string> keys;
    for(auto kv : codeFiles_) 
        keys.push_back(kv.first);
    std::sort(std::begin(keys),std::end(keys)); 
    return keys;
}

Similarity ComparisonMatrix::operator()(const std::string& s1, 
                                        const std::string& s2)
{
    if(s1<s2)
        return comparisonResult_[s1][s2];
    else
        return comparisonResult_[s2][s1];    
}


void ComparisonMatrix::visual_diff(const std::string& s1, const std::string& s2){
    if(s1==s2) return;
    std::string difftool = "setsid ";
    difftool += find_difftool();
    std::string syscall = difftool;
    Similarity sim = operator()(s1,s2);

    syscall += " "+root_+sim.hint1;
    syscall += " "+root_+sim.hint2;

    syscall+= " 2>&1 >/dev/null | grep 'something' &";
    //catch return value
    if(system(syscall.c_str())){};
}

void ComparisonMatrix::write_results_(std::ostream &os )
{
   
    using boost::property_tree::ptree;
    ptree pt;

    std::vector<CodeMatch> matches = get_sorted_matches();     
    int counter = 0;
    for(auto & m: matches ) {
        ptree & node = pt.add("comparision", ++counter);
        node.put("similarity",  m.similarity);
        node.put("group1",      m.group1);
        node.put("group2",      m.group2);
        node.put("file1",       m.file1);
        node.put("file2",       m.file2);
    }
 
    write_xml( os, pt );
}

void ComparisonMatrix::write_results(std::string &file){
    std::ofstream ofs;
    ofs.open(file.c_str(), std::ofstream::out | std::ofstream::trunc);
    write_results_(ofs);
    ofs.close();
}

