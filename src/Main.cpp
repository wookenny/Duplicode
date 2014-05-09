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

#include "CompareAlgo.h"
#include "FilterIdentity.hpp"
#include "FilterDelete.hpp"
#include "FilterDeleteComments.h"
#include "AbstractTestFilter.h"
#include "ComparatorIdentical.hpp"
#include "ComparatorLongestMatch.h"
#include "Common.h"
#include <string>
#include <memory>
#include <iostream>
#include "boost/program_options.hpp"

#define VERSION "0.0.1"

using namespace std;
namespace po = boost::program_options;

void help(const po::options_description &desc);

int main(int ac, const char* av[]){

    int opt;
    string root,suffixes, filter, comparators;

    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("version,v","print program version")
    ("path,p", po::value<string>(&root)->default_value("."),
    "root path to look for source code files")
    ("suffix,e", po::value<string>(&suffixes)->required(),
    "comma separated suffixes of the source code files to look at,"
    " e.g., .java,.h,.cpp ")
    ("filter,f", po::value<string>(&filter)->default_value(""),
    "comma separated filter to normalize the source code")
    ("comparator,c", po::value<string>(&comparators)->default_value("identity"),
    "filter used to compare the filtered source code")
    ("list,l", "ignore all other argumtents and give a list of available "
    "filters and comparators.")
    ;

    po::variables_map vm;

    try{
        po::store(po::parse_command_line(ac, av, desc), vm);
        if (vm.count("help")){
           help(desc);
           return 0;
        }

        po::notify(vm);
    }catch(const boost::program_options::required_option& e){

        cout << "Required argument missing: "<< e.what()<<"\n";
        cout << desc << "\n";
        return 0;
    }catch(const boost::program_options::error& e ) {
        cout << "Error: "<< e.what()<<"\n";
        cout << desc << "\n";
        return 0;
    }catch(...){
        cout << "Usage:"<< "\n";
        cout << desc << "\n";
        return 0;
    }

    //parse all arguments



    if (vm.count("version")) {
        cout << "duplicode "<< VERSION << "\n";
        return 0;
    }

    //call it, baby

    vector<string> types = split(suffixes,',');
    for(string s: types){
        auto files =  get_all_files(root,s);
        cout << "Ending: "<<s<<endl;
        for(auto f: files)
            cout << "\t - "<<f<<"\n";
        cout<< endl;
    }


    /*
    CompareAlgo comp;
    const string c1 = "este er rewrd w";
    const string c2 = "asta ar lkewrd";

    cout<<"------------------------------------------------------" <<endl;

    std::unique_ptr<AbstractTestFilter> filter{new FilterIdentity()};
    comp.addFilter(filter);

    cout << "Comparision without comp.: "<< comp.filterAndCompare(c1,c2) <<endl;

    cout<<"------------------------------------------------------" <<endl;

    std::unique_ptr<AbstractTestComparator> comparator{new ComparatorLongestMatch()};
    comp.setComparator(comparator);

    cout << "Comparision (longest match): "<< comp.filterAndCompare(c1,c2) <<endl;



     cout<<"------------------------------------------------------" <<endl;

    filter = move( unique_ptr<AbstractTestFilter>{new FilterDelete()} );
    comp.addFilter(filter);
    cout << "Comparision (longest match, filter: deleteCommets): "<< comp.filterAndCompare(c1,c2) <<endl;


    //const fs::path& root
    vector<string> types = {".h",".cpp",".java"};
    for(string s: types){
        auto files =  get_all_files("../examples",s);
        cout << "Ending: "<<s<<endl;
        for(auto f: files)
            cout << "\t - "<<f<<"\n";
        cout<< endl;
    }
    */
}

void help(const po::options_description &desc){
    cout << "Usage: duplicode [OPTION]... [PATH]...\n";
    cout << "Find all source files with the specified suffix in the given\n";
    cout << " directory (the current directory by default).\n";
    cout << "Use filters and comparators to find the best match";
    cout << " between source code.\n";
    cout << desc << "\n";
}
