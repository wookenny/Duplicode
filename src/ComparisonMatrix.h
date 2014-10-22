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
#pragma once
#include "CompareAlgo.h"
#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <fstream>
#include <string>
#include <memory>
#include "Common.h"
#include "ComparatorIdentical.hpp"
#include "ComparatorLongestMatch.h"
#include "ComparatorLevenshteinDistance.h"
#include "ComparatorDiffLib.hpp"
#include "FilterIdentity.hpp"

std::string get_group(const std::string &s);

struct CodeMatch{
    std::string group1;
    std::string group2;
    double similarity;
    std::string file1;
    std::string file2;
};

struct Similarity{
    double val;
    std::string hint1;
    std::string hint2;
};
        
//todo Groupnumer is a template argument
class ComparisonMatrix{

    struct CodeFile{
           std::string filename;  
           std::string content;
           std::string filtered_content; 
           
           CodeFile(const std::string f,const std::string root){
                std::ifstream ifs(root+f);
                std::string c( (std::istreambuf_iterator<char>(ifs) ),
                                      (std::istreambuf_iterator<char>() ) );
                assert(c!="");
                filename = f;
                content  = c;
                filtered_content = "";                       
 
           }
    };

    enum MultipleFileComp{SameNameMax, SameNameMultiply, SingleBestMatch}; 
   
    public:
        ComparisonMatrix() = delete;
        ComparisonMatrix(std::string root){
            std::unique_ptr<CompareAlgo> comp{new CompareAlgo()};
            /*
            std::unique_ptr<AbstractTestComparator> comparator
                                          {new ComparatorDiffLib()};
            comp->setComparator(comparator);
            std::unique_ptr<AbstractTestFilter> filter{new FilterIdentity()};
            comp->addFilter(filter);
            */
            comparator_ = std::move(comp);
            calculated_ = false;
            root_ = root;
        }
        
        
        
        void addCodes(std::vector<std::string> &codes);
        
        
        void print_files() const;
        
        void setCompareAlgorithm( std::unique_ptr<CompareAlgo> comp){
            comparator_ = std::move(comp);
            calculated_ = false;
        }        

        void addFilter(std::unique_ptr<AbstractTestFilter> &f){
            comparator_->addFilter(f);  
        }
    
        void setComparator(std::unique_ptr<AbstractTestComparator> &c){
            comparator_->setComparator(c);  
        }

        void calculateComparisionMatrix();

        void filterCodes();
        std::vector<CodeMatch> get_sorted_matches(); 
        std::vector<std::vector<double>> get_comp_matrix();
        std::vector<std::string> get_keys() const;
        
        Similarity operator()(const std::string&, const std::string& );
        
        double get_max(){ return max_;}
        double get_min(){ return min_;}
        void visual_diff(const std::string&, const std::string&);
    private:
        std::unordered_map<std::string,std::vector<CodeFile>> codeFiles_;
        std::unique_ptr<CompareAlgo> comparator_;
        bool calculated_;
        std::string root_;
        

        std::unordered_map<std::string,std::unordered_map<std::string,Similarity>> comparisonResult_;
        std::vector<std::tuple<std::string,std::string>> pairs_;
       
        MultipleFileComp comp_mode_ = SingleBestMatch;        
        double compare_groups(const std::vector<CodeFile>& g1,
                              const std::vector<CodeFile>& g2,
                              std::string *hint1 = nullptr,
                              std::string *hint2 = nullptr ) const;
                              
        void compute_via_thread_(int tid);
        double max_;
        double min_;
};
