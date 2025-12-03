#pragma once
#include <string>
#include <vector>

struct Args {

    bool includeFiles = false;
    bool terminate = false;

    std::string path;                 
    std::string target;               
    
    std::vector<std::string> exclude;  
    std::vector<std::string> excludeSub;
};

Args parse_args(int argc, const char* const argv[]);