#include <iostream>
#include <vector>

using namespace std;

struct Args {

    bool includeFiles = false;
    bool terminate = false;

    string path;                 
    string target;               
    
    vector<string> exclude;  
    vector<string> excludeSub;
};

namespace {

    void help() {

        cout << 
        R"(
        Description:
            Generates a directory tree in the style of the built in "tree" tool with additional options. 
            Default behaviour is to give the entire tree starting from the working directory, excluding files.

        Usage:
            tree_plus [Options]

        Options:
        
            -path, -P               [path]          Path to the tree root, relative to current directory 
                                                    (Default: current directory, ".").
            
            -include_files, -F                      Include files in the tree. 
            
            -exclude, -EX           [[name1] ...]   All folders, file extensions or complete file 
                                                    paths to exclude from the tree. Complete file paths should
                                                    be given starting from the working directory.
            
            -exclude_sub, -EX_S     [[name1] ...]   All folders or complete file paths that should have their content 
                                                    excluded so that the folder will be included but not any of its 
                                                    contents. Complete file paths should be given starting from the 
                                                    working directory.
            
            -target, -T             [path]          The target path for a txt file containing the directory tree.
            
            -h, --help, -?                          Show this help message and exit.
        )" 
        << std::endl;
    }
}


Args parse_args(int argc, char* argv[]) {

    Args args;
    args.path = ".";

    if (argc < 2) return args;

    string token = argv[1];

    if (token == "--help" || token == "-h" || token == "-?") {
            help();
            args.terminate = true;
            return args;
    }

    for (int i = 1; i < argc; ++i) {
        
        token = argv[i];

        if (token == "-include_files" || token == "-F") 
            args.includeFiles = true;
         
        else if ((token == "-path" || token == "-P"))
        
            if (i + 1 < argc && argv[i + 1][0] != '-') 
                args.path = argv[++i];
            else continue; 
        
        else if ((token == "-target" || token == "-T")) 

            if (i + 1 < argc && argv[i + 1][0] != '-') 
                args.target = argv[++i];
            else continue;    

        else if (token == "-exclude" || token == "-EX") {
          
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                args.exclude.emplace_back(argv[++i]);
            }
        } 
        
        else if (token == "-exclude_sub" || token == "-EX_S") {
          
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                args.excludeSub.emplace_back(argv[++i]);
            }
        } 
        
        else {
            cout << "\nUnknown argument: " << token << "\nRun \"tree_plus --help\" for more information.\n" << endl;
            args.terminate = true;
            return args;
        }
    }

    return args;
}
