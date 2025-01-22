#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace ss::read_file
{
    auto process_cpp_files(const std::string& dir_path, std::vector<std::pair<std::string, std::string>>& requests, std::vector<std::string>& files) -> void;
}

