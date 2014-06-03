#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "AbstractTestFilter.h"

namespace FilterFactory{

   void generate_filter(std::unique_ptr<AbstractTestFilter> &filter,
                        const std::string &f);
   std::vector<std::string> filter_list();
   std::vector<std::tuple<std::string,std::string>> filterdescription_list();

}
