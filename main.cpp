#include "curl-http.hpp"
#include "read-file.hpp"
#include "settings.hpp"
#include "util.hpp"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (!ss::settings::read_file()) return 0;

    std::string str_path;

    if (argc > 1) { // check if the argument is provided
        str_path = argv[1];

        std::string outputPath;
        for (char c : str_path) {
            if (c == '\\') {
                outputPath += "\\\\";
            } else {
                outputPath += c;
            }
        }
        str_path = outputPath;
    } else {
        // read the exe file path
        std::filesystem::path current_dir = std::filesystem::current_path();
        str_path = current_dir.string();
    }

    auto source_file_value = ss::settings::get_source_file_value();
    ss::util::add_code_file(source_file_value);

    auto language_type = ss::settings::get_language_type();
    auto value = ss::settings::get_language_value();
    ss::http::set_prompt_language(language_type[value]);

    std::vector<std::string> cpp_files;
    std::vector<std::pair<std::string, std::string>> requests;
    ss::read_file::process_cpp_files(str_path, requests, cpp_files);

    std::cout << "Directory: " << str_path << "\n"
              << "Files count: " << cpp_files.size() << "\n"
              << "Requset count: " << requests.size() << std::endl;
    if (requests.empty()) {
        return 0;
    }

   ss::http::send_all_request(requests);
    return 1;
}

