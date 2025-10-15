#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include "chars.hpp"
#include <iostream>

using namespace std;
namespace fs = filesystem;

namespace {

    bool isInPaths(
        const vector<string>& paths, 
        const fs::path& path
    ) {

        bool contains_name = (find(paths.begin(), paths.end(), path.filename()) != paths.end());
        bool contains_path = (find(paths.begin(), paths.end(), path) != paths.end());
        bool contains_ending = (find(paths.begin(), paths.end(), path.extension()) != paths.end());

        return contains_name or contains_path or contains_ending;
    }

    vector<fs::directory_entry> validEntries(
        const fs::path& path, 
        const vector<string>& exclude,
        const vector<string>& excludeSub
    ) {
        
        vector<fs::directory_entry> entries;

        if (isInPaths(excludeSub, path)) return entries;
        
        for (const fs::directory_entry& entry : fs::directory_iterator(path)) {

            if (isInPaths(exclude, entry.path())) continue;
            
            entries.push_back(entry);
        }
        return entries;
    }

    vector<fs::directory_entry> validDirectories(
        const fs::path& path, 
        const vector<string>& exclude,
        const vector<string>& excludeSub
    ) {
        
        vector<fs::directory_entry> entries;

        if (isInPaths(excludeSub, path)) return entries;
        
        for (const fs::directory_entry& entry : fs::directory_iterator(path)) {

            if (!entry.is_directory()) continue;
            if (isInPaths(exclude, entry.path())) continue;
            
            entries.push_back(entry);
        }
        return entries;
    }
}

void createFileTree(
    const bool includeFiles,
    const fs::path& path, 
    const string indent, 
    const vector<string>& exclude,
    const vector<string>& excludeSub,
    const TreeChars& chars,
    ostringstream& oss
) {

    vector<fs::directory_entry> entries;

    if (includeFiles)  entries = validEntries(path, exclude, excludeSub);
    else entries = validDirectories(path, exclude, excludeSub);

    for (size_t i = 0; i < entries.size(); ++i) {

        bool is_last = (i == entries.size() - 1);

        string prefix = indent + (is_last ? chars.branchLast: chars.branchMid);
        string recursionPrefix = indent + (is_last ? chars.indentLast : chars.indentMid);
        
        const fs::directory_entry &entry = entries[i];
        string relPath = entry.path().filename().string();

        if (entry.is_directory()) {
            
            oss << prefix << relPath <<  endl;
            createFileTree(includeFiles, entry.path(), recursionPrefix, exclude, excludeSub, chars, oss);
        
        } else {
        
            oss << prefix << relPath << endl;
        
        }
    }
}