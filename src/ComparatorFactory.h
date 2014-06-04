#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "AbstractTestComparator.h"

namespace ComparatorFactory{

   void generate_comparator(std::unique_ptr<AbstractTestComparator> &comp,
                        const std::string &f);
   std::vector<std::string> comparator_list();
   std::vector<std::tuple<std::string,std::string>> comparatordescription_list();

}
