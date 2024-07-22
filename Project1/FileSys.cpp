#include "FileSys.h"

#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> FileSys::getFilesInDir(const std::string& dir) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(dir)) { files.push_back(entry.path().string()); }
    return files;
}
