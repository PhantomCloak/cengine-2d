#pragma once
#include <string>
#include <vector>

class FileSys {
    public:
    static std::vector<std::string> GetFilesInDirectory(std::string& path);
    static std::vector<std::string> GetFoldersInDirectory(std::string& path);
    static std::string GetParentDirectory(std::string path);
    static std::string GetFileExtension(std::string path);
    static std::string GetFileName(std::string path);
    static void OpenFileOSDefaults(std::string path);
};
