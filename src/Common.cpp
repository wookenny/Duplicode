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

#include "Common.h"
#include <sstream>
#include <algorithm>
#include "magic.h"
namespace fs = boost::filesystem;
using namespace std;

// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
std::vector<boost::filesystem::path> get_all_files_by_extensions(const fs::path& root, 
                                        const string& ext,const std::string& ignore)
{  
    std::vector<boost::filesystem::path> files;
    if (!fs::exists(root)) 
        return files;
    
    if (fs::is_directory(root)){
        fs::recursive_directory_iterator it(root);
        fs::recursive_directory_iterator endit;
        while(it != endit){
            if (fs::is_regular_file(*it) and it->path().extension() == ext){
                if(not file_in_list(it->path().filename().string(),ignore))  
                    files.push_back(it->path());
            }
            ++it;
        }
    }
    return files;
}

std::vector<boost::filesystem::path> get_all_files_by_name(
                const boost::filesystem::path& root, const std::string& filenames)
{
    std::vector<boost::filesystem::path> files;
    if (!fs::exists(root)) 
        return files;
    
    if (fs::is_directory(root)){
        fs::recursive_directory_iterator it(root);
        fs::recursive_directory_iterator endit;
        while(it != endit){
            if (fs::is_regular_file(*it)){
                if(file_in_list(it->path().filename().string(),filenames))  
                    files.push_back(it->path());
            }
            ++it;
        }
    }
    return files;                
}        

// return the filenames of all files that seem to be sourcecode
// in the specified directory and all subdirectories
std::vector<boost::filesystem::path> get_all_source_files(const fs::path& root,
                                                      const std::string& ignore)
{  
    std::vector<boost::filesystem::path> files;
    if (!fs::exists(root)) 
        return files;
    
    if (fs::is_directory(root)){
        fs::recursive_directory_iterator it(root);
        fs::recursive_directory_iterator endit;
        while(it != endit){
            if (fs::is_regular_file(*it) and is_source(it->path().string())){
                if(not file_in_list(it->path().filename().string(),ignore)) 
                    files.push_back(it->path());
            }
            ++it;
        }
    }
    return files;
}



std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


bool is_source(const std::string &file){
     magic_t myt = magic_open(MAGIC_CONTINUE|MAGIC_ERROR/*|MAGIC_DEBUG*/);
     magic_load(myt,NULL);
     std::string type = magic_file(myt,file.c_str());
     magic_close(myt);
     return (type.find("source")!=std::string::npos);
}     


bool file_in_list(const std::string &name, const std::string &list){
    auto file_list  = split(list, ',');  
    for(const std::string& str: file_list)
        if (name == str)
            return true;
    return false;        
}

