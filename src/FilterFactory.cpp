#include "FilterFactory.h"

#include "FilterDeleteComments.h"
#include "FilterDelete.hpp"

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
    //other cases
    return;
}
   
std::vector<std::string> FilterFactory::filter_list(){
    return {"DeleteComments",
            "DeleteAll"};
}
   
   
std::vector<std::tuple<std::string,std::string>> FilterFactory::filterdescription_list()
{
    std::vector<std::tuple<std::string,std::string>> desc;
    desc.push_back(std::make_tuple("DeleteComments",
                            "deletes comments like '\\\\' and '\\*.. *\\'."));
    desc.push_back(std::make_tuple("DeleteAll","deletes complete code."));    
           
    return desc;       
}


