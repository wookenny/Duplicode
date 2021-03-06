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
#include "AbstractComparator.h"
#include "AbstractFilter.h"
#include "CodeFile.hpp"
#include <vector>
#include <memory>
#include <string>

class CompareAlgo{

    
    private:
        std::vector<std::unique_ptr<AbstractFilter>> filters_;
        std::unique_ptr<AbstractComparator> comparator_;

    public:
        CompareAlgo() = default;      

        void addFilter(std::unique_ptr<AbstractFilter>& );
        void addFilter(std::unique_ptr<AbstractFilter>&& );
        void initialize_comparators( const CodeMap& codes ) const; 
        void setComparator(std::unique_ptr<AbstractComparator>& ); 
        std::string filter(const std::string&) const;       
        double compare(const std::string&, const std::string&, std::string&) const;  
        double filterAndCompare(const std::string&, const std::string&, std::string&) const;  

};
