#include <string>
#include <fstream>
#include <iostream>
#include "args.hpp"
#include "chars.hpp"
#include "filetree.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    Args args = parse_args(argc, argv);
    TreeChars chars = initTreeChars();  

    if (args.terminate) return 0; 

    const string indent = "    ";
    ostringstream oss;
    oss << indent + "." << endl;
    createFileTree(args.includeFiles, args.path, indent, args.exclude, args.excludeSub, chars, oss);

    cout << oss.str() << endl;  

    if (!args.target.empty()) {

        ofstream out(args.target);
        if (!out) {
            cerr << "Error: could not open file " << args.target << endl;
        }

        out << oss.str();
        out.close();
    } 
}
