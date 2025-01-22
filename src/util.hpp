#pragma once
#include <string>
#include <map>

namespace ss::util
{
    auto add_code_file(const std::map<std::string, std::string>& ext) -> void;
    auto is_code_file(const std::string& file_ext) -> bool;
    auto file_md_format(const std::string& file_name) -> std::string;
}

