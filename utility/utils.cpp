#include <fstream>
#include <iostream>
#include <string>

namespace utils {
int log(const std::string& path, const std::string& type, const std::string& message) {
    std::ofstream logFile(path, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << path << std::endl;
        return 1;
    }

    logFile << "[" << type << "] " << message << std::endl;

    logFile.close();
    return 0;
    }

}
