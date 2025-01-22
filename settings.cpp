#include "settings.hpp"
#include "util.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

namespace ss::settings
{
    inline namespace
    {
        using json = nlohmann::json;
        Config_Data config{};

        void to_json(json& j, const Language_Value& lv)
        {
            j = json{{"language", lv.language}, {"value", lv.value}};
        }

        void from_json(const json& j, Language_Value& lv)
        {
            j.at("language").get_to(lv.language);
            j.at("value").get_to(lv.value);
        }

        void to_json(json& j, const SourceFile_Value& sfv)
        {
            j = json{{"extension", sfv.extension}, {"language", sfv.language}};
        }

        void from_json(const json& j, SourceFile_Value& sfv)
        {
            j.at("extension").get_to(sfv.extension);
            j.at("language").get_to(sfv.language);
        }

        void to_json(json& j, const Config_Data& config)
        {
            j = json{{"source_file_value", config.source_file_value_map},
                    {"language_type", config.language_type},
                    {"language_value", config.language_value_map}};
        }

        void from_json(const json& j, Config_Data& config)
        {
            j.at("source_file_value").get_to(config.source_file_value_map);
            j.at("language_type").get_to(config.language_type);
            j.at("language_value").get_to(config.language_value_map);
        }
    }

    auto read_file() -> bool
    {
        std::ifstream f("../../config.json");
        json data = json::parse(f);

        try {
            config = data.get<Config_Data>();  
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            return false;
        } catch (const json::type_error& e) {
            std::cerr << "JSON type error: " << e.what() << std::endl;
            return false;
        } catch (const json::out_of_range& e) {
            std::cerr << "JSON out of range error: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    auto get_language_type() -> std::map<std::string, std::string>&
    {
        return config.language_value_map;
    }

    auto get_language_value() -> std::string
    {
        return config.language_type;
    }

    auto get_source_file_value() -> std::map<std::string, std::string>&
    {
        return config.source_file_value_map;
    }
}

