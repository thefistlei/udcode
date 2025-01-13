#include "read-file.hpp"
#include "http.hpp"
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <future>
#include <algorithm>
#include <unordered_map>
#include <thread>

namespace ss
{
    namespace ext_api_unc
    {
        namespace {
            std::vector<std::thread> threads;

            std::unordered_map<std::string, int> fail_count{};
            std::vector<std::string> cpp_files;
            std::size_t file_index = 0;
            std::size_t index = 0;
            std::vector<std::string> chunks;
            std::string curr_file_path;
            namespace fs = std::filesystem;
            size_t chunk_size = 512*1; // 设置每个分割块的大小（字节）

            auto find_cpp_files(const fs::path& dir_path, std::vector<std::string>& cpp_files) -> void
            {
                for (const auto& entry : fs::directory_iterator(dir_path)) {
                    if (fs::is_directory(entry.status())) {
                        // 如果是目录，递归遍历
                        find_cpp_files(entry.path(), cpp_files);
                    } else if (entry.path().extension() == ".cpp" || entry.path().extension() == ".py") {
                        // 如果是 .cpp 文件，添加到结果列表
                        cpp_files.push_back(entry.path().string());
                    }
                }
            }

            auto escape_quotes(const std::string& input) -> std::string
            {
                std::string output;
                output.reserve(input.length() * 2); // 预留足够的空间，避免频繁的重新分配

                for (char c : input) {
                    if (c == '"') {
                        output += "\\\"";
                    } else {
                        output += c;
                    }
                }
                return output;
            }

            // 读取文件内容并将其按指定大小分割
            auto split_file_content(const std::string& file_name) -> std::vector<std::string>
            {
                std::vector<std::string> chunks;
                std::ifstream file(file_name, std::ios::binary); // 以二进制模式读取，防止换行符转换

                if (!file.is_open()) {
                    std::cerr << "Error opening file: " << file_name << std::endl;
                    return chunks; // 返回空 vector
                }

                file.seekg(0, std::ios::end);
                size_t file_size = file.tellg();
                file.seekg(0, std::ios::beg);

                size_t numChunks = (file_size + chunk_size - 1) / chunk_size; // 计算块数，向上取整

                for (size_t i = 0; i < numChunks; ++i) {
                    size_t current_chunk_size = std::min(chunk_size, file_size - i * chunk_size);
                    std::string chunk(current_chunk_size, '\0');
                    file.read(&chunk[0], current_chunk_size);

                    // 将换行符替换为 "\n"
                    std::string processed_chunk;
                    processed_chunk.reserve(chunk.size() * 2); // 预留空间

                    for (char c : chunk) {
                    if(c == '\r') continue; //忽略windows下的\r
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

            auto get_file_extension(const std::string& input_path) -> std::string
            {
                fs::path path(input_path);
                std::string extension = path.extension().string();
                // remove '.'
                extension.erase(extension.begin());
                return extension;
            }

            auto generate_new_filename(const std::string& input_path) -> std::string
            {
                // 将输入路径转换为文件系统路径
                fs::path path(input_path);

                // 获取文件的扩展名和无扩展名的文件名
                std::string extension = path.extension().string();
                std::string stem = path.stem().string();

                std::string new_filename = stem + "-" + ".md";

                // 返回新的完整路径
                return path.parent_path().string() + "/" + new_filename;
            }

            auto save_to_file(const std::string& combined_response) -> void
            {
                // 将响应合并并保存到新的文件中
                std::string new_path = generate_new_filename(curr_file_path);
                std::ofstream outputFile(new_path, std::ios::app);
                if (outputFile.is_open()) {
                    outputFile << combined_response;
                    outputFile.close();
                    std::cout << "Process file:" << curr_file_path << " " << index << "time." << std::endl;
                } else {
                    std::cerr << "Unable to write to file: " << curr_file_path << std::endl;
                }
            }
        }

        auto init_file_settings() -> bool
        {
            if (file_index < cpp_files.size()) {
                curr_file_path = cpp_files[file_index];
                ext_common_utility::http::set_file_extention(get_file_extension(curr_file_path));

                chunks = split_file_content(curr_file_path);// 按照指定大小将文件内容分割
                std::cout << "Begin processed file: " << curr_file_path << std::endl;

                return true;
            }
            return false;
        }

        auto send_fixed_command_call_back(const std::string& text) -> void
        {
            auto go_next = []() {
                file_index++;
                if(auto result = init_file_settings()) {
                    index = 0;
                    auto chunk = chunks[index];
                    ext_common_utility::http::send_fixed_command_call(chunk, send_fixed_command_call_back);
                }
                else {
                    std::cout << "Process finished. " << std::endl;
                    return;
                }
            };

            if (text == "Failed") {
            }
            save_to_file(text);

            index++;
            if (index < chunks.size()) {
                auto chunk = chunks[index];
                ext_common_utility::http::send_fixed_command_call(chunk, send_fixed_command_call_back);
            }
            else {
                go_next();
            }
        }

        auto process_cpp_files(const std::string& dir_path) -> void
        {
            find_cpp_files(dir_path, cpp_files);
            std::cout << "Found " << cpp_files.size() << " .cpp files in directory: " << dir_path << std::endl;

            index = 0;
            file_index = 0;
            if(auto result = init_file_settings()) {
                auto chunk = chunks[index];
                threads.push_back(std::thread(ext_common_utility::http::send_fixed_command_call(chunk, send_fixed_command_call_back)));
            }
        }
    }
}

