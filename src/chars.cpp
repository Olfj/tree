#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

struct TreeChars {
    string branchLast;
    string branchMid;
    string indentLast;
    string indentMid;
};

TreeChars initTreeChars() {
    
    #ifdef _WIN32
        if (!SetConsoleOutputCP(CP_UTF8)) {
            // ASCII if UTF-8 fails
            return {"+--- ", "|--- ", "    ", "|   "};
        }
    #endif
    
    // default
    return {"└── ", "├── ", "    ", "│   "};
}