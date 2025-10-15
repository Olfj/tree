#pragma once
#include <string>

struct TreeChars {
    std::string branchLast;
    std::string branchMid;
    std::string indentLast;
    std::string indentMid;
};

TreeChars initTreeChars();