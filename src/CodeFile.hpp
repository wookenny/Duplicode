#pragma once
#include <string>
#include <fstream>

struct CodeMatch{
    std::string group1;
    std::string group2;
    double similarity;
    std::string file1;
    std::string file2;
    std::string hint;
};

struct Similarity{
    double val;
    std::string file1;
    std::string file2;
    std::string hint;
};

struct CodeFile{
    std::string filename;  
    std::string content;
    std::string filtered_content; 
           
    CodeFile(const std::string f,const std::string root){
        std::ifstream ifs(root+f);
        std::string c( (std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>() ) );
        //assert(c!="");
        filename = f;
        content  = c;
        filtered_content = "";                       
    }
};
