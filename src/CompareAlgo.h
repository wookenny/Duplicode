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

#pragma once
#include "AbstractTestComparator.h"
#include "AbstractTestFilter.h"
#include <vector>
#include <memory>
#include <string>
//x
class CompareAlgo{

    
    private:
        std::vector<std::unique_ptr<AbstractTestFilter>> filters_;
        std::unique_ptr<AbstractTestComparator> comparator_;

    public:
        CompareAlgo() = default;      

        void addFilter(std::unique_ptr<AbstractTestFilter>& );
        void addFilter(std::unique_ptr<AbstractTestFilter>&& );

        void setComparator(std::unique_ptr<AbstractTestComparator>& ); 
        std::string filter(const std::string&) const;       
        double compare(const std::string&, const std::string&) const;  
        double filterAndCompare(const std::string&, const std::string&) const;  

};
