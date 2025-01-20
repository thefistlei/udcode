#include "curl-http.hpp"
#include "read-file.hpp"
#include "util.hpp"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string str_path;

    if (argc > 1) { // check if the argument is provided
        str_path = argv[1];
    } else {
        // read the exe file path
        std::filesystem::path current_dir = std::filesystem::current_path();
        str_path = current_dir.string();
    }

    std::unordered_map<std::string, std::string> ext{};
    ss::util::add_code_file(ext);

    std::vector<std::string> cpp_files;
    std::vector<std::pair<std::string, std::string>> requests;
    ss::read_file::process_cpp_files(str_path, requests, cpp_files);

    std::cout << "Directory: " << str_path << "\n"
              << "Files count: " << cpp_files.size() << "\n"
              << "Requset count: " << requests.size() << std::endl;
    if (requests.empty()) {
        return 0;
    }

    int type = 1;
    if (argc > 2) {
        type = std::atoi(argv[2]);
    }
    std::unordered_map<int, std::string> language_type{
        {1, "以中文回答, 解释下面代码"},
        {2, "explain the code"},
    };
    ss::http::set_prompt_language(language_type[type]);
    ss::http::send_all_request(requests);
    return 1;
}

