#include "filesystem.h"
#include "nfd.h"
#include <dirent.h>
#include <filesystem>
#include <iostream>


std::vector<std::string> FileSys::GetFilesInDirectory(std::string& path) {
    std::vector<std::string> fileList;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (!entry.is_directory()) {
            fileList.push_back(entry.path().string());
        }
    }
    return fileList;
}


std::vector<std::string> FileSys::GetFoldersInDirectory(std::string& path) {
    std::vector<std::string> fileList;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            fileList.push_back(entry.path().string());
        }
    }
    return fileList;
}


std::string FileSys::GetFileExtension(std::string path) {
    std::size_t found = path.find_last_of(".");
    if (found == std::string::npos) {
        return "";
    }
    return path.substr(found + 1);
}

std::string FileSys::GetFileName(std::string path) {
    std::size_t found = path.find_last_of("/\\");
    if (found == std::string::npos) {
        return path;
    }
    return path.substr(found + 1);
}

std::string FileSys::GetParentDirectory(std::string path) {
    std::filesystem::path p(path);
    return p.parent_path().string();
}

void FileSys::OpenFileOSDefaults(std::string path) {
    std::string command = "open " + path;
    system(command.c_str());
}

void FileSys::OpenFilePicker(std::function<void(std::string filePath)>&& callback) {
    NFD_Init();

    nfdchar_t* outPath;
    nfdfilteritem_t filterItem[1] = { { "Image", "png,jpg" } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

    if (result == NFD_OKAY) {
        puts("Success!");
        puts(outPath);
        callback(outPath);
        NFD_FreePath(outPath);
    } else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
    } else {
        printf("Error: %s\n", NFD_GetError());
    }

    NFD_Quit();
}
