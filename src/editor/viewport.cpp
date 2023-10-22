#include "viewport.h"
#include "../io/cursor.h"
#include "../render/render.h"
#include "imgui.h"
#include <algorithm>

void Fit(int image, int width, int height, bool center = false) {
    ImVec2 area = ImGui::GetContentRegionAvail();

    float scale = area.x / width;

    float y = height * scale;
    if (y > area.y) {
        scale = area.y / height;
    }

    int sizeX = int(width * scale);
    int sizeY = int(height * scale);

    if (center) {
        ImGui::SetCursorPosX(0);
        ImGui::SetCursorPosX(area.x / 2 - sizeX / 2);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (area.y / 2 - sizeY / 2));
    }

    CommancheRect sourceRect = CommancheRect{ 0, 0, float(width), -float(height) };
    float destWidth = float(sizeX);
    float destHeight = float(sizeY);
    ImVec2 uv0;
    ImVec2 uv1;

    if (sourceRect.width < 0) {
        uv0.x = -((float)sourceRect.x / width);
        uv1.x = (uv0.x - (float)(fabs(sourceRect.width) / width));
    } else {
        uv0.x = (float)sourceRect.x / width;
        uv1.x = uv0.x + (float)(sourceRect.width / width);
    }

    if (sourceRect.height < 0) {
        uv0.y = -((float)sourceRect.y / height);
        uv1.y = (uv0.y - (float)(fabs(sourceRect.height) / height));
    } else {
        uv0.y = (float)sourceRect.y / height;
        uv1.y = uv0.y + (float)(sourceRect.height / height);
    }

    static int imgIdx = 0;

    imgIdx = image;

#if RENDER_BACKEND_OPENGL
    ImGui::Image((void*)imgIdx, ImVec2(float(destWidth), float(destHeight)), uv0, uv1);
#elif RENDER_BACKEND_RAYLIB
    ImGui::Image((void*)&imgIdx, ImVec2(float(destWidth), float(destHeight)), uv0, uv1);
#endif
}

bool EditorViewPort::IsFocused() {
    return isFocused;
}

void EditorViewPort::RenderWindow() {
    static bool Open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    std::string title = "2D View " + std::to_string((int)ViewportSize.x) + "x" + std::to_string((int)ViewportSize.y) + "###2DView";
    if (ImGui::Begin(title.c_str(), &Open, ImGuiWindowFlags_NoNav)) {

        isFocused = ImGui::IsWindowHovered();
        static float zoomValue = 1;
        static float lastZoomValue = 1;

        //if (ImGui::IsWindowFocused())
        //    zoomValue += GetMouseWheelMove();
        if (zoomValue != lastZoomValue) {
            zoomValue = std::clamp(zoomValue, 1.0f, 10.0f);
            CommancheRenderer::Instance->SetCameraZoom(zoomValue);
            lastZoomValue = zoomValue;
        }

        auto rect = ImGui::GetContentRegionAvail();
        glm::vec2 currentRect = glm::vec2(rect.x, rect.y);

        static ImVec2 windowSize = ImVec2(0, 0);
        static ImVec2 contentAvail = ImVec2(0, 0);
        ImVec2 windowPos = ImVec2(0, 0);

        if (ViewportSize.x != currentRect.x || ViewportSize.y != currentRect.y) {
            ViewportSize = glm::vec2(currentRect.x, currentRect.y);

            CommancheRenderer::Instance->UpdateRenderTexture(ViewportSize);

            windowSize = ImGui::GetWindowSize();
            contentAvail = ImGui::GetContentRegionAvail();
        }

        glm::vec2 pos = Cursor::GetCursorPosition();
        windowPos = ImGui::GetWindowPos();

        pos.x -= (windowSize.x - contentAvail.x);
        pos.y -= windowPos.x;

        pos.y -= (windowSize.y - contentAvail.y);
        pos.y -= windowPos.y;

        ViewportCursorPos = pos;

        int frameId = CommancheRenderer::Instance->GetFrame();
        Fit(frameId, currentRect.x, currentRect.y, true);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
