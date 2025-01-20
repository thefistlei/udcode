#include "util.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

namespace ss::util
{
    namespace
    {
        std::unordered_map<std::string, std::string> file_ext{};

        std::unordered_map<std::string, std::string> def_file_ext{
            {".c", "c"},
            {".cpp", "cpp"},
            {".py", "py"}
        };
    }

    auto add_code_file(const std::unordered_map<std::string, std::string>& ext) -> void
    {
        for (const auto& pair : def_file_ext) {
            file_ext.insert(pair);
        }

        for (const auto& pair : ext) {
            file_ext.insert(pair);
        }
    }

    auto is_code_file(const std::string& current_file_ext) -> bool
    {
        return file_ext.find(current_file_ext) != file_ext.end();
    }

    auto file_md_format(const std::string& file_name) -> std::string
    {
        if (is_code_file(file_name)) {
            return file_ext[file_name];
        }

        return "";
    }
}

