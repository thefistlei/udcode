#include "read-file.hpp"
#include "util.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

namespace ss::read_file
{
    namespace
    {
        namespace fs = std::filesystem;
        size_t chunk_size = 1024*4; // set split block size（byte）

        auto find_cpp_files(const fs::path& dir_path, std::vector<std::string>& cpp_files) -> void
        {
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                if (fs::is_directory(entry.status())) {
                    // if folder, recursive find
                    find_cpp_files(entry.path(), cpp_files);
                } else if (util::is_code_file(entry.path().extension().string())) {
                    // if cpp file, add to vector
                    cpp_files.push_back(entry.path().string());
                }
            }
        }

        //  read file and split by fixed size
        auto split_file_content(const std::string& file_name) -> std::vector<std::string>
        {
            std::vector<std::string> chunks;
            std::ifstream file(file_name, std::ios::binary); // binary mode

            if (!file.is_open()) {
                std::cerr << "Error opening file: " << file_name << std::endl;
                return chunks; //  return empty vector
            }

            file.seekg(0, std::ios::end);
            size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            size_t numChunks = (file_size + chunk_size - 1) / chunk_size; //  calculate the number of chunks

            for (size_t i = 0; i < numChunks; ++i) {
                size_t current_chunk_size = std::min(chunk_size, file_size - i * chunk_size);
                std::string chunk(current_chunk_size, '\0');
                file.read(&chunk[0], current_chunk_size);

                // do with '\n'
                std::string processed_chunk;
                processed_chunk.reserve(chunk.size() * 2); // space for '\n'

                for (char c : chunk) {
                if(c == '\r') continue; //ingore \r on windows platform
                    if (c == '\n') {
                        processed_chunk += "\\n";
                    } else {
                        processed_chunk += c;
                    }
                }
                chunks.push_back(processed_chunk);
            }

            return chunks;
        }
    }

    auto process_cpp_files(const std::string& dir_path, std::vector<std::pair<std::string, std::string>>& requests, std::vector<std::string>& cpp_files) -> void
    {
        find_cpp_files(dir_path, cpp_files);

        for (const auto& curr_file_path : cpp_files) {
            auto chunks = split_file_content(curr_file_path);// split file by fixed size
            for (size_t i = 0; i < chunks.size(); ++i) {
                requests.push_back({chunks[i], curr_file_path});
            }
        }
    }
}

