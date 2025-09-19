#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
namespace fs = filesystem;

struct Args {
    string path;                  
    vector<string> exclude;  
    vector<string> exclude_sub;
    string target;  
    bool print_out;              
};

struct TreeChars {
    string branch_last;
    string branch_mid;
    string indent_last;
    string indent_mid;
};

Args parse_args(int argc, char* argv[]) {

    Args args;
    bool print_out = false;

    for (int i = 1; i < argc; ++i) {
        string token = argv[i];

        if (token == "-path" && i + 1 < argc) {
            args.path = argv[++i];  // take next arg
        } else if (token == "-exclude_sub") {
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                args.exclude_sub.emplace_back(argv[++i]);
            }
        } else if (token == "-exclude") {
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                args.exclude.emplace_back(argv[++i]);
            }
        } else if (token == "-target" && i + 1 < argc) {
            args.target = argv[++i];
            args.print_out = false;
        } else if (token == "-print_out") {
            print_out = true;
        } else {
            cerr << "Unknown argument: " << token << endl;
        }
    }
    args.print_out = print_out;

    if (args.path.empty()) {
        args.path = ".";  // default path
    }

    return args;
}

TreeChars init_tree_chars() {
    
    #ifdef _WIN32
        if (!SetConsoleOutputCP(CP_UTF8)) {
            // ASCII if UTF-8 fails
            return {"+--- ", "|--- ", "    ", "|   "};
        }
    #endif
    
    // default
    return {"└── ", "├── ", "    ", "│   "};
}

vector<fs::directory_entry> valid_entries(
    const fs::path& path, 
    const vector<string>& exclude,
    const vector<string>& exclude_sub
) {
    
    vector<fs::directory_entry> entries;

    if (find(exclude_sub.begin(), exclude_sub.end(), path.filename()) != exclude_sub.end()) // check if folder contents shouldbe excluded
        return entries;

    if (find(exclude_sub.begin(), exclude_sub.end(), path) != exclude_sub.end()) // check if folder contents shouldbe excluded
        return entries;

    for (const fs::directory_entry& entry : fs::directory_iterator(path)) {

        fs::path rel = entry.path().filename();
        fs::path ext = rel.extension();
        
        if (find(exclude.begin(), exclude.end(), ext) != exclude.end()) // check folders and specific files
            continue;

        if (find(exclude.begin(), exclude.end(), rel) != exclude.end()) // check file types
            continue;
        
        if (find(exclude.begin(), exclude.end(), entry.path()) != exclude.end()) //Check whole paths
            continue;
        
        entries.push_back(entry);
    }
    return entries;
}

void print_dir_recursive(
    const fs::path& path, 
    const string indent, 
    const vector<string>& exclude,
    const vector<string>& exclude_sub,
    const TreeChars& chars,
    ostringstream& oss
) {

    vector<fs::directory_entry> entries = valid_entries(path, exclude, exclude_sub);

    for (size_t i = 0; i < entries.size(); ++i) {

        bool is_last = (i == entries.size() - 1);

        string prefix = (is_last ? chars.branch_last: chars.branch_mid);
        string recursion_prefix = (is_last ? chars.indent_last : chars.indent_mid);
        
        const fs::directory_entry &entry = entries[i];
        string relPath = entry.path().filename().string();

        if (entry.is_directory()) {
            
            oss << indent << prefix << relPath << endl;
            print_dir_recursive(entry.path(), indent + recursion_prefix, exclude, exclude_sub, chars, oss);
        
        } else {
        
            oss << indent << prefix << relPath << endl;
        
        }
    }
}

void run(Args args, TreeChars chars) {

    const string indent = "    ";
    ostringstream oss;
    oss << indent + "." << endl;
    print_dir_recursive(args.path, indent, args.exclude, args.exclude_sub, chars, oss);

    if (!args.target.empty()) {

        ofstream out(args.target);
        if (!out) {
            cerr << "Error: could not open file " << args.target << endl;
        }

        out << oss.str();
        out.close();
    }
    if (args.print_out) {
        string result = oss.str();
        cout << result << endl;
    }  
}

int main(int argc, char* argv[])
{
    Args args = parse_args(argc, argv);
    TreeChars chars = init_tree_chars();
    run(args, chars);   
}
