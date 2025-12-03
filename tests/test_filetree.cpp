#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include "filetree.hpp" 
#include "chars.hpp"
#include "args.hpp" 

using namespace std;
namespace fs = filesystem;

//-------- Testing createFileTree --------//

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

TEST_CASE_METHOD(FileTreeFixture, "Tree includes files") {
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

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes files") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(false, base, "    ", {}, {}, chars, oss);

    string output = oss.str();
    for (vector<string> dirFiles : files){
        for (string file : dirFiles){
            REQUIRE(output.find(file) == string::npos);
        }
    }
    for (auto it = next(folders.begin()); it != folders.end(); ++it) {
        REQUIRE(output.find(*it) != string::npos);
    }
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes specific folder") {
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

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes file type") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {".pdf"}, {}, chars, oss);

    string output = oss.str();
    REQUIRE(output.find(".pdf") == string::npos);
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes directory content") {
    
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
    
    for (auto it = next(folders.begin()); it != folders.end(); ++it) {
        REQUIRE(output.find(*it) != string::npos);
    
    }
}

TEST_CASE_METHOD(FileTreeFixture, "Tree excludes directory content in root") {
    
    ostringstream oss;
    TreeChars chars = initTreeChars();

    createFileTree(true, base, "    ", {}, {base.string()}, chars, oss);
    
    string output = oss.str();

    for (string folder : folders) {
        REQUIRE(output.find(folder) == string::npos);
    }
    for (vector<string> folder : files) {
        for (string file : folder) {
            REQUIRE(output.find(file) == string::npos);
        }
    }
}

// TEST_CASE_METHOD(FileTreeFixture, "Program handles non existing path", "[!mayfail]") {

//     // There is no test case here yet, it just simply sees what happens. This should be updated to expect something. 
//     // Aditionally the code should do something other than just crashing. Maybe return a simple message about non existing paths and terminating.

//     ostringstream oss;
//     TreeChars chars = initTreeChars();

//     createFileTree(true, "non_existent", "    ", {}, {base.string()}, chars, oss);
// }


//-------- Testing parse_args --------//


TEST_CASE("Default args when no arguments provided") {
    const char* argv[] = { "program" };
    int argc = 1;

    Args args = parse_args(argc, argv);

    REQUIRE(args.path == ".");
    REQUIRE_FALSE(args.includeFiles);
    REQUIRE_FALSE(args.terminate);
    REQUIRE(args.exclude.empty());
    REQUIRE(args.excludeSub.empty());
}

TEST_CASE("Help flag terminates program") {
    const char* argv[] = { "program", "--help" };
    int argc = 2;

    Args args = parse_args(argc, argv);

    REQUIRE(args.terminate);
}

TEST_CASE("Include files flag works") {
    const char* argv[] = { "program", "-F" };
    int argc = 2;

    Args args = parse_args(argc, argv);

    REQUIRE(args.includeFiles);
}

TEST_CASE("Path argument sets correctly") {
    const char* argv[] = { "program", "-path", "/tmp" };
    int argc = 3;

    Args args = parse_args(argc, argv);

    REQUIRE(args.path == "/tmp");
}

TEST_CASE("Target argument sets correctly") {
    const char* argv[] = { "program", "-T", "hejbaberiba" };
    int argc = 3;

    Args args = parse_args(argc, argv);

    REQUIRE(args.target == "hejbaberiba");
}

TEST_CASE("Exclude arguments collect multiple values") {
    const char* argv[] = { "program", "-EX", "hej", "tjena" };
    int argc = 4;

    Args args = parse_args(argc, argv);

    REQUIRE(args.exclude.size() == 2);
    REQUIRE(args.exclude[0] == "hej");
    REQUIRE(args.exclude[1] == "tjena");
}

TEST_CASE("Exclude_sub arguments collect multiple values") {
    const char* argv[] = { "program", "-EX_S", "hej", "tjena" };
    int argc = 4;

    Args args = parse_args(argc, argv);

    REQUIRE(args.excludeSub.size() == 2);
    REQUIRE(args.excludeSub[0] == "hej");
    REQUIRE(args.excludeSub[1] == "tjena");
}

TEST_CASE("Unknown argument terminates program") {
    const char* argv[] = { "program", "-unknown" };
    int argc = 2;

    Args args = parse_args(argc, argv);

    REQUIRE(args.terminate);
}



    