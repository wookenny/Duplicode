#include "FilterFactory.h"

#include "FilterDeleteComments.h"
#include "FilterDelete.hpp"
#include "FilterIdentity.hpp"
#include "FilterToLower.hpp"

void FilterFactory::generate_filter(std::unique_ptr<AbstractTestFilter> &filter,
                                    const std::string& f)
{
    if(f=="DeleteComments"){
        std::unique_ptr<AbstractTestFilter> f{new FilterDeleteComments()};
        filter = std::move(f);
        return;
    }
    if(f=="DeleteAll"){
        std::unique_ptr<AbstractTestFilter> f{new FilterDelete()};
        filter = std::move(f);
        return;
    }
    if(f=="Identity"){
        std::unique_ptr<AbstractTestFilter> f{new FilterIdentity()};
        filter = std::move(f);
        return;
    }
    if(f=="ToLower"){
        std::unique_ptr<AbstractTestFilter> f{new FilterToLower()};
        filter = std::move(f);
        return;
    }    
    return;
}
   
std::vector<std::string> FilterFactory::filter_list(){
    return {"DeleteComments",
            "DeleteAll",
            "Identity",
            "ToLower"};
}
   
   
std::vector<std::tuple<std::string,std::string>> FilterFactory::filterdescription_list()
{
    std::vector<std::tuple<std::string,std::string>> desc;
    desc.push_back(std::make_tuple("DeleteComments",
                            "deletes comments like '\\\\' and '\\*.. *\\'."));
    desc.push_back(std::make_tuple("DeleteAll","deletes complete code."));
    desc.push_back(std::make_tuple("Identity", "does not change the code.")); 
    desc.push_back(std::make_tuple("ToLower", "changes every character to lower case."));   
           
    return desc;       
}


