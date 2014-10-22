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

uint ComparatorLevenshteinDistance::levenshteinDistance(const string &s1, 
                                                      const string &s2) const
{
  const uint m(s1.size());
  const uint n(s2.size());
 
  if( m==0 ) return n;
  if( n==0 ) return m;
 
  vector<uint> costs(n + 1, 0);
 
  uint i = 0;
  for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
  {
    costs[0] = i+1;
    uint corner = i;
 
    uint j = 0;
    for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
    {
      uint upper = costs[j+1];
      if( *it1 == *it2 )
      {
		  costs[j+1] = corner;
	  }
      else
	  {
		uint t(upper<corner?upper:corner);
        costs[j+1] = (costs[j]<t?costs[j]:t)+1;
	  }
 
      corner = upper;
    }
  }
 
  return costs[n];
}
