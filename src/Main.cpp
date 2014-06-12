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

#include "FilterFactory.h"
#include "ComparisonMatrix.h"
#include "ComparatorFactory.h"
#include "Common.h"
#include "gnuplot-iostream.h"
 #include "window.h"
#include <string>
#include <memory>
#include <iostream>
#include <chrono>  
#include <thread>
#include <boost/program_options.hpp>
#include <QApplication>

#define VERSION "0.0.1"

using namespace std;
namespace po = boost::program_options;

void help(const po::options_description &desc);
    
vector<string> collect_files(const string& root, const string& suffixes, 
                             const std::string& ignore);
vector<string> collect_files_by_names(const string& root,  const string& filenames);
void print_filter_comparators();
void display_matrix(ComparisonMatrix &matrix);

int main(int ac, char* av[]){
    QApplication app(ac, av);
    string root,suffixes, filter, comparator, ignore, filenames;

    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("version,v","print program version")
    ("path,p", po::value<string>(&root)->default_value("."),
    "root path to look for source code files")
    ("suffix,e", po::value<string>(&suffixes)->default_value(""),
    "comma separated suffixes of the source code files to look at,"
    " e.g., .java,.h,.cpp ")
    ("ignore,i", po::value<string>(&ignore)->default_value(""),
    "comma separated list of files to be ignored. ")
    ("exclusive,x", po::value<string>(&filenames),
    "comma separated list of files to considered exclusively."
    " Everything else will be ignored. E.g., 'Decompress.java'")
    ("filter,f", po::value<string>(&filter)->default_value(""),
    "comma separated filter to normalize the source code")
    ("comparator,c", po::value<string>(&comparator)->default_value("DiffLib"),
    "comparator used to compare the filtered source code")
    ("list,l", "ignore all other argumtents and give a list of available "
    "filters and comparators.")
    ("show,s", "use a visual difftool to show the best machtes.")
    ("matrix,m", "use gnuplot to display a matrix with the matches")
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
        cout << "Usage:"<<std::boolalpha<< "\n";
        cout << desc << "\n";
        return 0;
    }

    //parse all arguments
    if (vm.count("version")) {
        cout << "duplicode "<< VERSION << "\n";
        return 0;
    }

    if (vm.count("list")) {
       print_filter_comparators();
       return 0;
    }   
    
    
    //call it, baby
    std::vector<std::string> files;
    if(vm.count("exclusive")){
        files  = collect_files_by_names(root, filenames);
    }else{ 
        files = collect_files(root, suffixes, ignore);
    }    
    ComparisonMatrix comp_matrix(root);
    comp_matrix.addCodes(files);    
    if(filter!=""){
        auto filters = split(filter,',');
        for(auto f: filters){
            std::unique_ptr<AbstractTestFilter> filter = nullptr;
            FilterFactory::generate_filter(filter,f);
            if(filter!=nullptr){
                std::cout<<"added filter: "<<f<<std::endl;
                comp_matrix.addFilter(filter);
            }
        }
    }
    
    //load comparator
    std::unique_ptr<AbstractTestComparator> comp = nullptr;
    ComparatorFactory::generate_comparator(comp,comparator);
    if(comp!=nullptr){
        std::cout<<"using comparator: "<<comparator<<std::endl;
        comp_matrix.setComparator(comp);
    }else{//load difflib
        ComparatorFactory::generate_comparator(comp,"DiffLib");
        assert(comp!=nullptr);
        comp_matrix.setComparator(comp);
    }   

    
    //comp_matrix.print_files();
    //comp_matrix.calculateComparisionMatrix(); 
    auto results = comp_matrix.get_sorted_matches();
    for(uint i = 0; i<15 and i<results.size();++i){
        CodeMatch &m = results[i]; 
        std::cout<< m.group1<<" vs. "<<m.group2<<":\t"<<m.similarity<<"\n";  
    }
    //print statistics
    if(results.size()>0){
        double max = results.front().similarity;
        double min = results.back().similarity;
        double median = results[results.size()/2].similarity;
        double avg = 0;
        for(auto & elem : results)
            avg += elem.similarity;
        avg /= results.size();     
        std::cout<<"min: "<<min<<"\tmedian: "<<median<<"\tmax: "
                 <<max<<"\tavg: "<<avg<<std::endl;
    }
    std::cout<<std::endl;
    
    if(results.size()>0 and vm.count("show")){
         std::cout<<"using visual difftool to view findings"<<std::endl;
         std::string difftool = find_difftool();
         for(uint i = 0;i<results.size();++i){
            CodeMatch &m = results[i];
            std::cout<<"Comparing: '"<<m.group1<<"' '"<<m.group2<<"'  (";
            std::cout<<m.similarity<<")"<<std::endl;
            std::cout<<"Launch '"<<difftool<<"' [Y/n]"<<std::endl;
            std::string answer; 
            std::cin >> answer;
            if(answer=="n" or answer=="N"){
                std::cout<<"Quit."<<std::endl;
                break;
            }
            std::string syscall = difftool;
            syscall += " "+root+m.file1;
            syscall += " "+root+m.file2 + " 2>&1 >/dev/null | grep 'something'";
            if(system(syscall.c_str())){};
                
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }
    }
    
    
    if(results.size()>0 and vm.count("matrix")){
        Window window(comp_matrix);
        window.show();
        //display_matrix(comp_matrix);
        app.exec();
    }

}

void help(const po::options_description &desc){
    cout << "Usage: duplicode [OPTION]... [PATH]...\n";
    cout << "Find all source files with the specified suffix in the given\n";
    cout << " directory (the current directory by default).\n";
    cout << "Use filters and comparators to find the best match";
    cout << " between source code.\n";
    cout << desc << "\n";
}

    
vector<string> collect_files(const string& root, const string& suffixes, const std::string& ignore=""){
    //TODO: maybe not using f.string but more involed things with filesystem:;path object 
    vector<string> matching_files;
    vector<string> types = split(suffixes,',');
    if (types.size()==0){
        auto files =  get_all_source_files(root,ignore);
        for(auto f: files){
            string name = f.string();
            name = name.substr(root.size()); 
            matching_files.push_back(name);  
        }   
    }   
       
    for(string s: types){
        auto files =  get_all_files_by_extensions(root,s,ignore);
        for(auto f: files){
            string name = f.string();
            name = name.substr(root.size()); 
            matching_files.push_back(name);  
        }
    }   
    
    return matching_files; 
}

vector<string> collect_files_by_names(const string& root,  const string& filenames){
    vector<string> matching_files;
    auto files = get_all_files_by_name(root,filenames);
    for(auto f: files){
        string name = f.string();
        name = name.substr(root.size()); 
        matching_files.push_back(name);  
    }
    return matching_files;   
}

void print_filter_comparators(){
    cout << "filters: \n";
    auto filters = FilterFactory::filterdescription_list();
    for(const auto &f: filters){
        std::string name,desc;
        std::tie(name,desc) = f;
        std::cout<<"\t'"<<name<<"':   "<<desc <<"\n";
    
    }
    
    cout << "\n\ncomparators: \n";
    auto comps = ComparatorFactory::comparatordescription_list();
    for(const auto &c: comps){
        std::string name,desc;
        std::tie(name,desc) = c;
        std::cout<<"\t'"<<name<<"':   "<<desc <<"\n";
    
    }
}
    
void inline display_matrix(ComparisonMatrix &matrix){
   
	// For debugging or manual editing of commands:
	//Gnuplot gp(std::fopen("plot.gnu"));
	// or
	//Gnuplot gp("tee plot.gnu | gnuplot -persist");
    auto results = matrix.get_comp_matrix();
    auto keys =  matrix.get_keys();
    auto sorted_results = matrix.get_sorted_matches();
    
    /*
    //std::string xtics = "set xtics (\""+keys[0]+"\" 0";
    //for(uint i=1; i<keys.size();++i)
    //    xtics += ", \""+keys[i]+"\" "+std::to_string(i);
    //xtics += ")\n";
    //gp << xtics; 
    Gnuplot gp;   
    gp << "set palette defined (0 \"white\", 1 \"red\")\n";
    gp << "set cbrange [0:"<<sorted_results.front().similarity<<"]\n";
	gp << "plot '-' matrix with image \n";
	gp.send1d(results);   
    */
    Window window(matrix);
    window.show();
}
