#pragma once
#include <string>
#include <vector>

class FileSys {
    public:
    static std::vector<std::string> GetFilesInDirectory(std::string path);
    static std::vector<std::string> GetFoldersInDirectory(std::string path);
    static std::string GetParentDirectory(std::string path);
    static std::string GetFileExtension(std::string path);
    static std::string GetFileName(std::string path);
    static std::string ReadFile(std::string path);
    static void CopyFile(std::string source, std::string destination);
    static void OpenFilePickerDialog(std::function<void(std::string filePath)> callback);
    static void OpenFileSaveDialog(std::string defaultName, std::string defaultPath, std::function<void(std::string filePath)>&& callback);
    static void OpenFileOSDefaults(std::string path);
};
