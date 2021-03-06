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

#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include "AbstractComparator.h"

class ComparatorFactory{
    public: 
        template<typename T> 
        static AbstractComparator * createInstance() { return new T; } 
        static std::vector<std::string> comparator_list();
        static std::vector<std::tuple<std::string,std::string>> comparatordescription_list();
        static void generate_comparator(std::unique_ptr<AbstractComparator> &comp,
                                                      const std::string &f);

    private:
        static std::map<std::string, AbstractComparator*(*)()> map;

};
