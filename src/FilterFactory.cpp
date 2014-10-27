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

#include "FilterFactory.h"
#include "Common.h"

//Further Filters can be added like this:
// * include the needed header
// * add the name and the class to the map below
//
// important: implement  name() and description() method properly

#include "FilterDeleteComments.h"
#include "FilterDeleteWhitespace.hpp"
#include "FilterDelete.hpp"
#include "FilterIdentity.hpp"
#include "FilterToLower.hpp"

std::map<std::string, AbstractFilter*(*)()> FilterFactory::map = 
{
 {FilterDeleteComments().name(),   &createInstance<FilterDeleteComments>},
 {FilterDeleteWhitespace().name(), &createInstance<FilterDeleteWhitespace>},
 {FilterDelete().name(),           &createInstance<FilterDelete>},
 {FilterIdentity().name(),         &createInstance<FilterIdentity>},
 {FilterToLower().name(),          &createInstance<FilterToLower>}
};

/* ---------------------  HINT!! ----------------------------------
  Generic part below, no modification for new comparators needed!
-------------------------------------------------------------------*/

void FilterFactory::generate_filter(std::unique_ptr<AbstractFilter> &comp,
                                    const std::string& f)
{
    std::string c = fuzzy_match(f, filter_list());       
    std::unique_ptr<AbstractFilter> co{ map[c]() };
    comp = std::move(co);
    return;
}
   

std::vector<std::string> FilterFactory::filter_list(){
    std::vector<std::string> list;
    for(const auto &kv: map)
        list.push_back(kv.first);
    return list;
}
     

std::vector<std::tuple<std::string,std::string>> FilterFactory::filterdescription_list()
{
    std::vector<std::tuple<std::string,std::string>> desc;
    for(const auto &kv: map){
        std::unique_ptr<AbstractFilter> comp{ map[kv.first]() };
        desc.push_back( std::make_tuple( kv.first,comp->description() ) );
    }
    return desc;       
}

    







