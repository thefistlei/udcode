#include "read-file.hpp" 
#include <iostream>
#include <filesystem>

int main()
{
    // read the exe file path
    std::filesystem::path currentDir = std::filesystem::current_path();
    std::cout << "Current directory: " << currentDir << std::endl;
    ss::ext_api_unc::process_cpp_files(currentDir.string());
}

