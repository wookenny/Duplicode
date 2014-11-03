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
#include <vector>
#include <string>
#include <cstdio>
#include "boost/filesystem.hpp"


std::vector<boost::filesystem::path> get_all_files_by_extensions(
                const boost::filesystem::path& root, 
                const std::string& ext, const std::string& ignore);

std::vector<boost::filesystem::path> get_all_source_files(
                const boost::filesystem::path& root, const std::string& ignore);
                
std::vector<boost::filesystem::path> get_all_files_by_name(
                const boost::filesystem::path& root, const std::string& filenames);                
                
std::vector<std::string> &split(const std::string &, char , 
                                std::vector<std::string> &);
std::vector<std::string> split(const std::string&, char);    



bool is_source(const std::string &file);
bool is_ascii_text(const std::string &file);
bool file_in_list(const std::string &name, const std::string &list); 
std::string find_difftool();
uint levenshtein_distance(const std::string &s1, const std::string &s2);

std::string fuzzy_match(const std::string&,const std::vector<std::string>&);

