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

#include "CompareAlgo.h"
#include <vector>
#include <memory>
#include <tuple>

//todo Groupnumer is a template argument
class ComparisonMatrix{

    struct CodeFile{
           std::string filename;  
           std::string content;
           std::string filtered_content; 
    };


    enum MultipleFileComp{SameName, SingleBestMatch, BestMatching}; 
   
    public:
        
        void addCodes(std::vector<std::string> &codes){
            codeList_.reset();
            for(auto s: codes)
                codeList_.push_back(s);
            calculated_ = false;
        }
        void setCompareAlgorithm( std::unique_ptr<compareAlgo> comp){
            comparator_ = std::move(comp);
            calculated_ = false;
        }        

        //TODO: every string filtered 'n' times!! Once is enough!
        //NOW doable
        void calculateComparisionMatrix(){
            if(calculated_) return;
            #pragma omp parallel for
            for(uint i=0; i<codeList_;++i)
                for(uint j=i+1; j<codeList_; ++j)
                    comparisonResult_[i][j] = comparator_(codeList_[i],codeList[j]); 

            calculated_=true;
        }
        
        void filterCodes(){
            for(CodeFile file: codeFiles_)
                    file.filtered_content = comparator_.filter(file.content);
        }
    
    private:
        std::unordered_map<int,CodeFile> codeFiles_;
        std::unordered_set<int, std::unordered_map<int, double>> compResults_;
        std::unique_ptr<compareAlgo> comparator_;
        bool calculated = false;
};
