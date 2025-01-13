#include "http.hpp"
//#include "nlohmannjson/json.hpp"
//#include "curl.h"
#include "include/curl/curl.h"
#include <iostream>

using namespace std;

namespace ss::ext_common_utility::http
{
    inline namespace
    {
        std::string response{"Failed"};

     /*   void to_json(nlohmann::json& j, const Http_Data& p)
        {
            j = nlohmann::json{{"model", p.model}, {"prompt", p.prompt}, {"stream", p.stream}};
        }

        void from_json(const nlohmann::json& j, Http_Data& p)
        {
            j.at("model").get_to(p.model);
            j.at("prompt").get_to(p.prompt);
            j.at("stream").get_to(p.stream);
        }*/

        size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
        {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
        std::string file_extension{};
    }

    auto send_fixed_command_call(const std::string& cmd, Call_Back_Func func) -> void
    {
        send_fixed_command_call_(cmd, func);
        return;
    }

    auto send_fixed_command_call_(const std::string& cmd, Call_Back_Func func) -> void
    {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        std::string url = "http://localhost:11434/api/generate";
        std::string userpwd = "root:123456"; // �û���������

        curl_global_init(CURL_GLOBAL_DEFAULT); // ��ʼ�� libcurl

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // ���� URL
            curl_easy_setopt(curl, CURLOPT_POST, 1L); // ����Ϊ POST ����

            std::string post_data = R"({
                "model": "llama3.2",
                "prompt": "解释代码:#include <filesystem>\n namespace ss \n void init(){}",
                "stream": false
            })";

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.length());

            // ������֤��Ϣ
        //    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.c_str());

            // ���� Content-Type ͷ��
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            } else {
                std::cout << readBuffer << std::endl;
            }

            curl_slist_free_all(headers); // �ͷ�ͷ���б�
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup(); // ���� libcurl
    }

    auto set_file_extention(const std::string& ext) -> void
    {
        file_extension = ext;
    }
}

