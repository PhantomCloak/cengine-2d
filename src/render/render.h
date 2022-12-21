#pragma once

#include "../log/log.h"
#include "render_primitives.h"
#include <string>

class CommancheRenderer {
    public:
    void* wnd;
    void* gctx;
    std::string driverStr;
    void Initialize(const std::string& title, int windowHeight, int windowWidth);
    void DrawRectangle(int x, int y, int w, int h, CommancheColorRGB color);
    // void DrawFillRectangle(int x, int y, int w, int h, CommancheColorRGB color);
    bool IsTextureValid(int textureId);
    void DrawImage(int textureId, int x, int y, int width, int height, int size, int offsetX = 0, int offsetY = 0);
    void DrawText(int fontId, std::string message, int x, int y, int size, CommancheColorRGB color = { 255, 255, 255 });
    int LoadTexture(std::string path);
    CommancheTextureInfo GetTextureInfo(int id);
    int LoadFont(const std::string& path, int size);
    void Render();
    void Destroy();
};
