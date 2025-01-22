#pragma once
#include "nlohmannjson/json.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace ss::settings
{
    struct Language_Value final
    {
        std::string language{};
        std::string value{};
    };

    struct SourceFile_Value final
    {
        std::string extension{};
        std::string language{};
    };

    struct Config_Data final
    {
        std::map<std::string, std::string> source_file_value_map{};
        std::string language_type{};
        std::map<std::string, std::string> language_value_map{};
    };

    auto read_file() -> bool;

    auto get_language_type() -> std::map<std::string, std::string>&;
    auto get_language_value() -> std::string;
    auto get_source_file_value() -> std::map<std::string, std::string>&;
}

