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
    //void DrawRectangle(float x, float y, float w, float h, float rotation, CommancheColorRGB color);
    bool IsTextureValid(int textureId);
    bool IsShaderValid(int shaderId);
    void DrawImage(int textureId, float x, float y, float width, float height, float rotation, int offsetX = 0, int offsetY = 0);
    void DrawText(int fontId, std::string message, int x, int y, int size, CommancheColorRGB color = { 255, 255, 255 });
    void SetFrameSize(int width, int height);
    int GetFrame();
    int LoadTexture(const std::string& path);
    int LoadShader(const std::string& path);
    CommancheTextureInfo GetTextureInfo(int id);
    int LoadFont(const std::string& path, int size);
    //void DrawTest();
    void Render();
    void Render1();
    void Render2();
    void Destroy();
    private:
    unsigned int framebuffer, textureColorbuffer;
};
