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

#include "ComparatorLevenshteinDistance.h"
#include <vector>
using namespace std;

double ComparatorLevenshteinDistance::operator()
            (const std::string& c1, const std::string& c2, std::string&) const
{
    uint dist = 0;
    if(c1.size() >= c2.size())
        dist = levenshtein_distance(c1,c2);
    else 
        dist = levenshtein_distance(c2,c1);
    double n = std::min(c1.size(),c2.size());
    double N = std::max(c1.size(),c2.size());       
    double sim = (dist-(N-n))/std::max(1.0*dist,1.);

    return std::max(sim,0.);            
}
