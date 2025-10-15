#include <vector>
#include <string>
#include <filesystem>
#include "chars.hpp"

void createFileTree(
    const bool includeFiles,
    const std::filesystem::path& path, 
    const std::string indent, 
    const std::vector<std::string>& exclude,
    const std::vector<std::string>& excludeSub,
    const TreeChars& chars,
    std::ostringstream& oss
);