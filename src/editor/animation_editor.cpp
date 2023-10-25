#define IMGUI_DEFINE_MATH_OPERATORS
#include "animation_editor.h"
#include "../assetmgr/AssetManager.h"
#include "../io/cursor.h"
#include "../io/filesystem.h"
#include "../log/log.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <algorithm>
#include <list>
#include <numeric>

void reduceFileNameLenght(std::string& str) {
    str = str.substr(0, str.length() > 10 ? 10 : str.length());
}

void a() {
    int titleBarHeight = 10;
    ImVec2 availableSpace = ImGui::GetContentRegionAvail();
    ImVec2 projectAreaPadding = ImVec2(10.0f, 10.0f);
    ImVec2 iconPadding = ImVec2(10.0f, 10.0f);
    float iconBorderRounding = 10.0f;
    float iconBorderThickness = 2.0f;
    float buttonAreaHeight = 50.0f;
    float dropShadowHeight = 10.0f;


    ImGui::BeginChild("##ProjectArea", ImVec2(availableSpace.x, availableSpace.y - (titleBarHeight + buttonAreaHeight)));

    float iconWidth = (availableSpace.x - projectAreaPadding.x * 2.0f);
    iconWidth = (iconWidth / 3.0f) - (iconPadding.x * 2.0f);
    float iconHeight = (1080.0f / 1920.0f) * iconWidth;
    ImVec2 projectAreaBegin = ImGui::GetCursorScreenPos();
    // ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + projectAreaPadding);
    float absProjectAreaX = ImGui::GetCursorScreenPos().x;

    float childScrollY = ImGui::GetScrollY();
    ImVec2 buttonAreaBegin = projectAreaBegin + ImVec2(0.0f, availableSpace.y - (titleBarHeight + buttonAreaHeight) + childScrollY);

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, iconBorderRounding);
    auto drawList = ImGui::GetWindowDrawList();
    for (int i = 0; i < 1; i++) {

        ImVec2 iconStart = ImGui::GetCursorPos();
        ImVec2 absIconStart = ImGui::GetCursorScreenPos();
        ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + iconPadding);

        std::string uid = "##1";
        bool selected = true;
        if (ImGui::Selectable(uid.c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(iconWidth, iconHeight))) {
            // selectedProjectIndex = i;
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            // res = true;
        }

        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectMax = ImGui::GetItemRectMax();
        drawList->PushClipRect(ImGui::GetCurrentWindow()->ClipRect.Min, ImGui::GetCurrentWindow()->ClipRect.Max);
        drawList->AddRectFilled(
        ImVec2(0, 0), ImVec2(1, 1),
        IM_COL32(255, 255, 255, 255));

        const ImVec4& borderColor = ImColor::HSV(0.0f, 0.0f, 0.0f);

        drawList->AddRect(rectMin, rectMax, ImColor(borderColor), iconBorderRounding, 0, iconBorderThickness);
        drawList->PopClipRect();

        ImVec2 textSize = ImVec2(10, 10);
        float offsetX = (iconWidth / 2.0f - textSize.x / 2.0f) + iconPadding.x;
        ImGui::SetCursorPosX(iconStart.x + offsetX);
        ImGui::Text("AAA");

        ImGui::SetCursorScreenPos(absIconStart + ImVec2(iconWidth + iconPadding.x * 2.0f, 0.0f));

        if (((i + 1) % 3) == 0) {
            ImGui::SetCursorScreenPos(ImVec2(absProjectAreaX, absIconStart.y + iconHeight + textSize.y + iconPadding.y));
        }
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImColor color = ImColor(ImU32(0xFFFFFFFF));
    drawList->AddRectFilledMultiColor(
    buttonAreaBegin + ImVec2(0.0f, -dropShadowHeight),
    buttonAreaBegin + ImVec2(availableSpace.x, 0.0f),
    IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0),
    color, color);
    ImGui::EndChild();
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

    // if (createFileTile("..", ctx, &hoverList[0], sizeof(hoverList), true)) {
    //     currentPath = FileSys::GetParentDirectory(currentPath);
    // }

    for (int i = 0; i < directories.size(); i++) {
        std::string currentFolder = directories[i];
        std::string fileName = FileSys::GetFileName(currentFolder);

        reduceFileNameLenght(fileName);

        int breakCtx = ImGui::GetWindowWidth() / 60;

        if (breakCtx == 0)
            breakCtx = 1;
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

void DrawCenteredTextInRect(const char* text, const ImVec2& rectMin, const ImVec2& rectMax, ImU32 rectColor) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw the rectangle

    // Calculate text size
    ImVec2 textSize = ImGui::CalcTextSize(text);

    // Calculate position to center the text inside the rectangle
    ImVec2 textPos;
    textPos.x = rectMin.x + (rectMax.x - rectMin.x) * 0.5f - textSize.x * 0.5f;
    textPos.y = rectMin.y + (rectMax.y - rectMin.y) * 0.5f - textSize.y * 0.5f;

    // Draw the centered text
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), text);
}

void DrawTimelineWindow() {
    int totalTime = 5;
    ImGui::Begin("Timeline");
    float eventTimes[5] = { 1.0f, 3.5f, 5.0f, 7.5f, 9.0f };

    // Define the range of the timeline
    float timelineStart = 0.0f;
    float timelineEnd = 10.0f;

    // Render the timeline bar
    float timelineWidth = ImGui::GetContentRegionAvail().x;
    float timelineHeight = 20.0f;

    ImGui::Dummy(ImVec2(0, timelineHeight / 2)); // Add some vertical spacing

    ImGui::InvisibleButton("##timeline_bar", ImVec2(timelineWidth, timelineHeight));

    ImVec2 timelineStartPos = ImGui::GetItemRectMin();
    ImVec2 timelineEndPos = ImGui::GetItemRectMax();

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(timelineStartPos, timelineEndPos, ImGui::GetColorU32(ImGuiCol_FrameBg));

    const int numPins = 50;
    for (int i = 0; i <= numPins; i++) {
        float pinPosX = timelineStartPos.x + (timelineWidth / numPins) * i;
        drawList->AddLine(ImVec2(pinPosX, timelineStartPos.y + 10), ImVec2(pinPosX, timelineEndPos.y), IM_COL32(150, 150, 150, 255), 1.0f);
    }

    // Render the events on the timeline
    for (int i = 0; i < 5; i++) {
        float eventPosX = timelineStartPos.x + (eventTimes[i] - timelineStart) / (timelineEnd - timelineStart) * timelineWidth;
        float eventPosY = timelineStartPos.y + timelineHeight / 2;

        DrawCenteredTextInRect(std::to_string(i).c_str(), ImVec2(eventPosX - 20.0f, eventPosY - 10.0f), ImVec2(eventPosX + 20.0f, eventPosY + 10.0f), ImGui::GetColorU32(ImGuiCol_FrameBgActive));
        ImGui::SetCursorScreenPos(ImVec2(eventPosX - 20.0f, eventPosY + 10.0f));
    }


    ImGui::Dummy(ImVec2(0, timelineHeight / 2)); // Add some vertical spacing
    ImGui::InvisibleButton("##timeline_bar", ImVec2(timelineWidth, timelineHeight));
    drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(timelineEndPos, ImVec2(timelineEndPos.x, timelineEndPos.y + ImGui::GetContentRegionMax().y), ImGui::GetColorU32(ImU32(0xFF141318)));
    //drawList->AddRectFilled(timelineEndPos, ImVec2(300, timelineEndPos.y + ImGui::GetContentRegionMax().y), ImGui::GetColorU32(ImU32(0xFF2B2B2D)));

    float numOfGrids = 5;

    // Vertical Lines
//    for (int i = 0; i < numOfGrids; i++) {
//        ImVec2 start = timelineStartPos;
//        ImVec2 end = timelineEndPos;
//
//        start.x = (timelineEndPos.x / numOfGrids) * i;
//        end.x = (timelineEndPos.x / numOfGrids) * i;
//
//        drawList->AddLine(start, end, IM_COL32(150, 150, 150, 255), 1.0f);
//    }
//
//    // Horizontal Lines
//    for (int i = 0; i < numOfGrids; i++) {
//        ImVec2 start = timelineStartPos;
//        ImVec2 end = timelineEndPos;
//
//        start.y = (timelineEndPos.y / numOfGrids) * i;
//        end.y = (timelineEndPos.y / numOfGrids) * i;
//
//        drawList->AddLine(start, end, IM_COL32(150, 150, 150, 255), 1.0f);
//    }
//
    ImGui::End();
}

void AnimationEditor::RenderWindow() {
    static bool Open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Design Anim", &Open, ImGuiWindowFlags_NoNav)) {
        // a();
        DrawTimelineWindow();
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
