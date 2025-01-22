#pragma once
#include <future>
#include <string>
#include <vector>

namespace ss::http
{
    struct Http_Data final
    {
        std::string model;
        std::string prompt;
        bool stream;
    };

    auto send_request(const std::string& content, const std::string& filename) -> std::string;

    auto send_all_request(const std::vector<std::pair<std::string, std::string>>& requests) -> void;

    auto set_prompt_language(const std::string& language) -> void;
}

