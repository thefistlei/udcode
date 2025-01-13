#pragma once
#include <string>
#include <future>

namespace ss::ext_common_utility::http
{
    struct Http_Data final
    {
        std::string model;
        std::string prompt;
        bool stream;
    };

    using Call_Back = auto (std::string& ret) -> void;
    using Call_Back_Func = std::function<Call_Back>;

    auto send_fixed_command_call(const std::string& cmd, Call_Back_Func func) -> void;
    auto set_file_extention(const std::string& ext) -> void;
    auto send_fixed_command_call_(const std::string& cmd, Call_Back_Func func) -> void;
}

