#include "curl-http.hpp"
#include "util.hpp"
#include "curl/curl.h"
#include "nlohmannjson/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace ss::http
{
    inline namespace
    {
        namespace fs = std::filesystem;

        std::mutex cout_mutex; // mutex for cout
        std::string prompt_lanuage;

        auto get_file_extension(const std::string& input_path) -> std::string
        {
            fs::path path(input_path);
            return path.extension().string();
        }

        auto generate_new_filename(const std::string& input_path) -> std::string
        {
            // get file path
            fs::path path(input_path);

            // get file extension
            std::string extension = path.extension().string();
            std::string stem = path.stem().string();

            std::string new_filename = stem + "-" + ".md";

            // return new file path
            return path.parent_path().string() + "/" + new_filename;
        }

        void to_json(nlohmann::json& j, const Http_Data& p)
        {
            j = nlohmann::json{{"model", p.model}, {"prompt", p.prompt}, {"stream", p.stream}};
        }

        void from_json(const nlohmann::json& j, Http_Data& p)
        {
            j.at("model").get_to(p.model);
            j.at("prompt").get_to(p.prompt);
            j.at("stream").get_to(p.stream);
        }

        // write callback function to write response data to file
        size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
        {
            std::ofstream *outfile = static_cast<std::ofstream*>(userp);
            if (outfile->is_open()) {
                std::string str(reinterpret_cast<char*>(contents), size * nmemb);
                nlohmann::json json = nlohmann::json::parse(str);
                std::string response = json["response"];
                // std::cerr << "send_request response: " << response << std::endl;
                outfile->write(response.data(), response.size());
                return size * nmemb;
            } else {
                std::cerr << "Error opening output file." << std::endl;
                return 0;
            }
        }
    }

    auto send_request(const std::string& content, const std::string& filename) -> std::string
    {
        auto new_file = generate_new_filename(filename);
        std::ofstream outfile(new_file, std::ios::app);
        if (!outfile.is_open()) {
            std::cerr << "Error opening file: " << new_file << std::endl;
            return "File open error";
        }

        std::string readBuffer;
        std::string url = "http://localhost:11434/api/generate";
        std::string userpwd = "root:123456"; // username:password

        CURLcode res;
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // set URL
            curl_easy_setopt(curl, CURLOPT_POST, 1L); // set POST

            Http_Data config;
            config.model = "llama3.2";
            config.stream = false;

            auto file_extension = get_file_extension(filename);
            auto file_format = util::file_md_format(file_extension);
            config.prompt = prompt_lanuage + ": \\n ```" + file_format + " \\n " + content + " \\n ```";

            // std::cerr << "send_request: " << config.prompt << std::endl;

            nlohmann::json data;
            to_json(data, config);

            // set POST data
            std::string post_data = data.dump();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.length());

            // set userpwd
            //curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.c_str());

            // set header
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outfile);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                outfile.close();
                curl_easy_cleanup(curl);
                return "error";
            } else {
                outfile.close();
                curl_easy_cleanup(curl);
                return "success";
            }

            curl_slist_free_all(headers); // free header
            curl_easy_cleanup(curl);
        }

        outfile.close();
        return "error";
    }

    auto send_all_request(const std::vector<std::pair<std::string, std::string>>& requests) -> void
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);

        std::vector<std::future<std::string>> futures;
        for (const auto& request : requests) {
            std::future<std::string> future = std::async(std::launch::async, send_request, request.first, request.second);
            futures.push_back(std::move(future));
        }

        float count = (float)requests.size();
        int error_count = 0;

        // wait for all the futures to complete
        for (auto i = 0; i < futures.size();i++) {
            try {
                float current = i / count * 100;
                std::cout << std::fixed << std::setprecision(0) << current << "%" << " -> " << requests[i].second << "\r";
                std::cout.flush();

                auto result = futures[i].get(); // wait for the result
                {
                    std::lock_guard<std::mutex> lock(cout_mutex); //
                    // std::cout << "request " << result << std::endl;
                }
            }
            catch(const std::exception& e) {
                 // std::cerr << "request error " << e.what() << '\n';
                error_count++;
                continue;
            }
        }

        std::cout << std::endl << error_count << " request error" << std::endl;
        curl_global_cleanup();
    }

    auto set_prompt_language(const std::string& language) -> void
    {
        prompt_lanuage = language;
    }
}

