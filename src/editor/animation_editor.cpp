#include "animation_editor.h"
#include "../assetmgr/AssetManager.h"
#include "../io/cursor.h"
#include "../io/filesystem.h"
#include "../log/log.h"
#include "imgui.h"
#include <algorithm>
#include <list>
#include <numeric>

void reduceFileNameLenght(std::string& str) {
    str = str.substr(0, str.length() > 10 ? 10 : str.length());
}

bool createFileTile(std::string name, int& ordinal, bool* stateSet, int stateSetSize, bool isFolder) {
    static int folderIcon = AssetManager::GetTexture("folder");
    static int fileIcon = AssetManager::GetTexture("file");

    int icon = isFolder ? folderIcon : fileIcon;
    bool result = false;

    ImGui::BeginGroup();

    auto windowSize = ImGui::GetWindowSize();

    ImGui::PushID(name.c_str());

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));

    if (ImGui::ImageButton((void*)&folderIcon, ImVec2((float)50, (float)50), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
        result = true;
    }

    bool isHovered = ImGui::IsItemHovered(); // Move this after the ImageButton

    if (isHovered) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Text("%s", name.c_str());

    if (isHovered)
        ImGui::PopStyleVar();
    ImGui::EndGroup();

    ImGui::SameLine();
    ordinal++;

    return result;
}


void AnimationTree() {
    static std::string currentPath = "./assets/animations";
    static std::string lastPath = "./assets";
    static bool hoverList[1024];
    int ctx = 0;

    std::vector<std::string> files = FileSys::GetFilesInDirectory(currentPath);
    std::vector<std::string> directories = FileSys::GetFoldersInDirectory(currentPath);

    //if (createFileTile("..", ctx, &hoverList[0], sizeof(hoverList), true)) {
    //    currentPath = FileSys::GetParentDirectory(currentPath);
    //}

    for (int i = 0; i < directories.size(); i++) {
        std::string currentFolder = directories[i];
        std::string fileName = FileSys::GetFileName(currentFolder);

        reduceFileNameLenght(fileName);

        int breakCtx = ImGui::GetWindowWidth() / 60;

        if (ctx % breakCtx == 0)
            ImGui::NewLine();


        if (createFileTile(fileName, ctx, &hoverList[0], sizeof(hoverList), true)) {
            if (fileName == "..")
                currentPath = FileSys::GetParentDirectory(currentPath);
            else
                currentPath = currentFolder;
        }
        ctx++;
    }

    for (int i = 0; i < files.size(); i++) {
        std::string currentFile = files[i];
        std::string fileName = FileSys::GetFileName(currentFile);
        reduceFileNameLenght(fileName);

        int breakCtx = ImGui::GetWindowWidth() / 60;

        if (ctx % breakCtx == 0)
            ImGui::NewLine();

        if (createFileTile(fileName, ctx, &hoverList[0], sizeof(hoverList), false)) {
            FileSys::OpenFileOSDefaults(currentFile);
        }
        ctx++;
    }

    // createFileTile("", ctx, &hoverList[0], sizeof(hoverList), false);
}

void AnimationEditor::RenderWindow() {
    static bool Open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Design Anim", &Open, ImGuiWindowFlags_NoNav)) {

        static float zoomValue = 1;
        static float lastZoomValue = 1;

        auto rect = ImGui::GetContentRegionAvail();
        glm::vec2 currentRect = glm::vec2(rect.x, rect.y);

        static ImVec2 windowSize = ImVec2(0, 0);
        static ImVec2 contentAvail = ImVec2(0, 0);
        ImVec2 windowPos = ImVec2(0, 0);


        static bool a = true;

        for (int i = 0; i < 10; i++) {
        }

        glm::vec2 pos = Cursor::GetCursorPosition();
        windowPos = ImGui::GetWindowPos();


        ImGui::Begin("Anim Properties");
        ImGui::Text("Zoom");
        ImGui::SliderFloat("##zoom", &zoomValue, 0.1f, 10.0f, "%.1f");
        ImGui::End();

        ImGui::Begin("Animation Structure");
        AnimationTree();
        ImGui::End();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

bool AnimationEditor::IsFocused() {
    return isFocused;
}
