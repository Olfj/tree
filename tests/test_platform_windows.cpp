#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "filetree.hpp"
#include "chars.hpp"
#include <windows.h>
#include <iostream>

using namespace std;
namespace fs = filesystem;

TEST_CASE("Testing fallback tree chars") {

    // This is a somewhat dubious method of testing this.
    // It works fine from powershell but we'll see how it goes in the actions workflow.
    REQUIRE(FreeConsole() == TRUE); 
    TreeChars chars = initTreeChars();
    REQUIRE(chars.branchLast == "+--- ");
    REQUIRE(chars.branchMid == "|--- ");
    REQUIRE(chars.indentLast == "    ");
    REQUIRE(chars.indentMid == "|   ");
}

TEST_CASE("Testing tree chars") {
    TreeChars chars = initTreeChars();
    REQUIRE(chars.branchLast == "└── ");
    REQUIRE(chars.branchMid == "├── ");
    REQUIRE(chars.indentLast == "    ");
    REQUIRE(chars.indentMid == "│   ");
}