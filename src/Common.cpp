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

#include "Common.h"
#include <sstream>
#include <algorithm>
#include <boost/regex.hpp>
#include <limits>
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
            if (fs::is_regular_file(*it)  and not fs::is_directory(*it) 
                                            and it->path().extension() == ext){
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
            if (fs::is_regular_file(*it) and not fs::is_directory(*it)){
                std::string filename =  it->path().filename().string();
                for(const std::string& s:  split(filenames,',')){
                    boost::regex expression(s);
                    boost::cmatch char_matches; 
                    
                    if(boost::regex_match(filename.c_str(),
                                           char_matches, 
                                           expression)){ 
                        files.push_back(it->path());
                    }        
                }
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
            if (fs::is_regular_file(*it) and is_ascii_text(it->path().string())  
                                                and not fs::is_directory(*it)){
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

std::vector<std::string> split(const std::string &s){
    std::istringstream iss(s);
    std::vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    return tokens;     
}

bool is_source(const std::string &file){
     magic_t myt = magic_open(MAGIC_CONTINUE|MAGIC_ERROR/*|MAGIC_DEBUG*/);
     magic_load(myt,NULL);
     std::string type = magic_file(myt,file.c_str());
     magic_close(myt);
     return (type.find("source")!=std::string::npos);
}     

bool is_ascii_text(const std::string &file){
     magic_t myt = magic_open(MAGIC_CONTINUE|MAGIC_ERROR/*|MAGIC_DEBUG*/);
     magic_load(myt,NULL);
     std::string type = magic_file(myt,file.c_str());
     magic_close(myt);
     return (type.find("ASCII text")!=std::string::npos);
}

bool file_in_list(const std::string &name, const std::string &list){
    auto file_list  = split(list, ',');  
    for(const std::string& str: file_list)
        if (name == str)
            return true;
    return false;        
}

std::string find_difftool(){
    std::vector<std::string> difftools = { "meld", "opendiff", 
            "kdiff3", "tkdiff", "xxdiff", "kompare", "gvimdiff", "diffuse",
            "diffmerge", "ecmerge", "p4merge", "araxis", "bc3", "codecompare",
            "emerge", "vimdiff"};
            
    for(auto &s: difftools){
        int i = system (("which "+s+" >> /dev/null").c_str());
        if(i==0)
            return s;
    }
    return "";
}

uint levenshtein_distance(const std::string &s1, const std::string &s2)
{
  const uint m(s1.size());
  const uint n(s2.size());
 
  if( m==0 ) return n;
  if( n==0 ) return m;
 
  vector<uint> costs(n + 1, 0);
 
  uint i = 0;
  for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
  {
    costs[0] = i+1;
    uint corner = i;
 
    uint j = 0;
    for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
    {
      uint upper = costs[j+1];
      if( *it1 == *it2 )
      {
		  costs[j+1] = corner;
	  }
      else
	  {
		uint t(upper<corner?upper:corner);
        costs[j+1] = (costs[j]<t?costs[j]:t)+1;
	  }
 
      corner = upper;
    }
  }
 
  return costs[n];
}

std::string fuzzy_match(const std::string& s,const std::vector<std::string>& list){
    int edit_dist = std::numeric_limits<int>::max();
    std::string match = s;
    for(const auto &l: list){
        int d = levenshtein_distance(l,s);
        int diff = l.size() - s.size();
        if(diff>0) d-= diff;
        if(d < edit_dist){
            edit_dist = d;
            match = l;
        }
    }
    return match;
}



