#include "fileview.h"
#include "../assetmgr/AssetManager.h"
#include "../io/filesystem.h"
#include "imgui.h"
#include <iostream>


void reduceFileNameLenght(std::string& str) {
    str = str.substr(0, str.length() > 10 ? 10 : str.length());
}

bool createFileTile(std::string name, int& ordinal, bool* stateSet, int stateSetSize, bool isFolder) {
    static int folderIcon = AssetManager::GetTexture("folder");
    static int fileIcon = AssetManager::GetTexture("file");
    int icon = isFolder ? folderIcon : fileIcon;
    bool result = false;
    bool isHovered = ImGui::IsItemHovered();
    stateSet[ordinal - 1] = isHovered;

    ImGui::BeginGroup();
    if (stateSet[ordinal]) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    ImVec4 bgColor = ImVec4(0.05f, 0.05f, 0.05f, 1);
    ImVec4 tinColor = ImVec4(1, 1, 1, 1);

    if (name == "") {
        bgColor = ImVec4(0.05f, 0.05f, 0.05f, 1);
        tinColor = ImVec4(0.05f, 0.05f, 0.05f, 1);
    }

    ImGui::PushID(name.c_str());
    if (ImGui::ImageButton((void*)(intptr_t)icon, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), 0, bgColor, tinColor)) {
        result = true;
    }
    ImGui::PopID();

    ImGui::Text("%s", name.c_str());

    ImGui::EndGroup();

    if (stateSet[ordinal])
        ImGui::PopStyleVar();

    ImGui::SameLine();
    ordinal++;

    return result;
}

void FileView::RenderWindow() {
    static std::string currentPath = ".";

    if (ImGui::Begin("Files")) {

        static std::string lastPath = ".";
        static bool hoverList[1024];
        int ctx = 0;

        std::vector<std::string> files = FileSys::GetFilesInDirectory(currentPath);
        std::vector<std::string> directories = FileSys::GetFoldersInDirectory(currentPath);

        if (createFileTile("..", ctx, &hoverList[0], sizeof(hoverList), true)) {
            currentPath = FileSys::GetParentDirectory(currentPath);
        }

        for (int i = 0; i < directories.size(); i++) {
            std::string currentFolder = directories[i];
            std::string fileName = FileSys::GetFileName(currentFolder);

            reduceFileNameLenght(fileName);

            if (createFileTile(fileName, ctx, &hoverList[0], sizeof(hoverList), true)) {
                if (fileName == "..")
                    currentPath = FileSys::GetParentDirectory(currentPath);
                else
                    currentPath = currentFolder;
            }
        }

        for (int i = 0; i < files.size(); i++) {
            std::string currentFile = files[i];
            std::string fileName = FileSys::GetFileName(currentFile);
            reduceFileNameLenght(fileName);

            if (createFileTile(fileName, ctx, &hoverList[0], sizeof(hoverList), false)) {
                FileSys::OpenFileOSDefaults(currentFile);
            }
        }

        createFileTile("", ctx, &hoverList[0], sizeof(hoverList), false);
        ImGui::End();
    }
}
