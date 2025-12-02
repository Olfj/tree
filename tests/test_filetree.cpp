#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include "filetree.hpp" 
#include "chars.hpp"


using namespace std;
namespace fs = filesystem;

struct FileTreeFixture {
    fs::path base;
    vector<string> folders;
    vector<vector<string>> files;

    FileTreeFixture() {
        
        base = fs::temp_directory_path() / "filetree_fixture";
        folders = {"filetree_fixture", "subdir1", "subdir2"};
        files = {{"a.txt", "a.pdf", "b.txt", "b.png"}, {"c.txt", "c.pdf", "d.txt", "d.png"}, {}};

        fs::create_directory(base);
        fs::create_directory(base / "subdir1");
        fs::create_directory(base / "subdir2");

        ofstream(base / "a.txt");  
        ofstream(base / "a.pdf");      
        ofstream(base / "b.txt");
        ofstream(base / "b.png");
        
        ofstream(base / "subdir1" / "c.txt");
        ofstream(base / "subdir1" / "c.pdf");
        ofstream(base / "subdir1" / "d.txt");
        ofstream(base / "subdir1" / "d.png");
    }

    ~FileTreeFixture() {
        fs::remove_all(base);
    }
};

TEST_CASE_METHOD(FileTreeFixture, "Tree includes files", "[filetree]") {
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {}, {}, chars, oss);

    string output = oss.str();
    
    for (vector<string> dirFiles : files){
        for (string file : dirFiles){
            REQUIRE(output.find(file) != string::npos);
        }
    }
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes files", "[filetree]") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(false, base, "    ", {}, {}, chars, oss);

    string output = oss.str();
    for (vector<string> dirFiles : files){
        for (string file : dirFiles){
            REQUIRE(output.find(file) == string::npos);
        }
    }
    for (auto it = std::next(folders.begin()); it != folders.end(); ++it) {
        REQUIRE(output.find(*it) != string::npos);
    }
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes specific folder", "[filetree]") {
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {"subdir1"}, {}, chars, oss);

        
    string output = oss.str();

    for (string file : files[0]) {
        REQUIRE(output.find(file) != string::npos);
    }
    for (string file : files[1]) {
        REQUIRE(output.find(file) == string::npos);
    }
    REQUIRE(output.find("subdir1") == string::npos);
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes file type", "[filetree]") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {".pdf"}, {}, chars, oss);

    string output = oss.str();
    REQUIRE(output.find(".pdf") == string::npos);
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes directory content", "[filetree]") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {}, {"subdir1"}, chars, oss);
    
    string output = oss.str();

    for (string file : files[1]) {
        REQUIRE(output.find(file) == string::npos);
    }
    for (string file : files[0]) {
        REQUIRE(output.find(file) != string::npos);
    }
    
    for (auto it = std::next(folders.begin()); it != folders.end(); ++it) {
        REQUIRE(output.find(*it) != string::npos);
    
    }
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes directory content in root", "[filetree]") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {}, {base.string()}, chars, oss);
    
    string output = oss.str();

    cout << "\n--- Tree includes files ---\n" << output << "\n";

    for (string folder : folders) {
        REQUIRE(output.find(folder) == string::npos);
    }
    for (vector<string> folder : files) {
        for (string file : folder) {
            REQUIRE(output.find(file) == string::npos);
        }
    }
}

TEST_CASE_METHOD(FileTreeFixture, "Program handles non existing path", "[!mayfail]") {

    // There is no test case here yet, it just simply sees what happens. This should be updated to expect something. 
    // Aditionally the code should do something other than just crashing. Maybe return a simple message about non existing paths and terminating.

    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, "non_existent", "    ", {}, {base.string()}, chars, oss);
}

    