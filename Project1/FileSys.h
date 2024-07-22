#pragma once
#include <string>
#include <vector>

static class FileSys {
public:
    static std::vector<std::string> getFilesInDir(const std::string& dir);
};
