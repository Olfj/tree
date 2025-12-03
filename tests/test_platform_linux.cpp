#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "filetree.hpp"

using namespace std;
namespace fs = filesystem;

TEST_CASE("Testing tree chars") {
    TreeChars chars = initTreeChars();
    REQUIRE(chars.branchLast == "└── ");
    REQUIRE(chars.branchMid == "├── ");
    REQUIRE(chars.indentLast == "    ");
    REQUIRE(chars.indentMid == "│   ");
}

