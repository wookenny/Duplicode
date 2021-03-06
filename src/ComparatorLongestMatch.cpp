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

#include "ComparatorLongestMatch.h"
#include <iostream>
double ComparatorLongestMatch::operator()(const std::string& c1,
                                          const std::string& c2,
                                          std::string& h) const
{
    double max_match = 0;    
    for(uint i = 0; i < c1.size(); ++i )
        for(uint j = 0; j < c2.size();++j){
            double match = sizeOfMatch(c1,i,c2,j); 
            if(match > max_match){
                max_match = match;
                h = c1.substr(i,match);
            }
                
        }

    return max_match;
}


double ComparatorLongestMatch::sizeOfMatch(const std::string& c1, uint i1,
                                          const std::string& c2, uint i2) const
{
    uint pos = 0;
    while(i1+pos < c1.size() and i2+pos<c2.size() and c1[i1+pos]==c2[i2+pos])
        ++pos;
    return static_cast<double>(pos);
}
