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

#include "CompareAlgo.h"
#include <iostream>

using namespace std;

string CompareAlgo::filter(const string& code) const{
    string s = code;
    for(const auto& f: filters_)
        s = (*f)(s);
    return s;
}

double CompareAlgo::compare(const string& code1,const string& code2, string& hint) const{
    if(comparator_!=nullptr)
        return (*comparator_)(code1,code2, hint);
    else
        std::cerr<<"Warning: No operator to compare code given!"<<std::endl;
    return 0;
}

double CompareAlgo::filterAndCompare(const std::string& c1,
                                     const std::string& c2, std::string& hint) const{

    string filtered1 = filter(c1);
    string filtered2 = filter(c2);

    return compare(filtered1, filtered2,hint);
}

void CompareAlgo::addFilter(unique_ptr<AbstractFilter>& filter_ptr) {
    filters_.push_back(move(filter_ptr));
}

void CompareAlgo::addFilter(std::unique_ptr<AbstractFilter>&& filter_ptr){
    filters_.push_back(move(filter_ptr));
}


void CompareAlgo::setComparator(unique_ptr<AbstractComparator>& c){
    comparator_ = move(c);
}


void CompareAlgo::initialize_comparators(  const CodeMap& codes  ) const{
    comparator_->init( codes ); 
}
