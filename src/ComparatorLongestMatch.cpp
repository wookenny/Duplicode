#include "ComparatorLongestMatch.h"
double ComparatorLongestMatch::operator()(const std::string& c1,
                                          const std::string& c2) const
{
    double max_match = 0;    
    #pragma omp parallel for
    for(uint i = 0; i < c1.size(); ++i )
        for(uint j = 0; j < c2.size();++j){
            double match = sizeOfMatch(c1,i,c2,j);
            max_match = std::max(match,max_match);       
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
