#pragma once

#include "../log/log.h"
#include "render_primitives.h"
#include "glm/glm.hpp"
#include <string>

class CommancheRenderer {
    public:
    void* wnd;
    void* gctx;
    std::string driverStr;
    void Initialize(const std::string& title, int windowHeight, int windowWidth);
    void InitializeShaders(const std::string& defaultShaderPath);
    void SetPPM(int val);
    bool IsTextureValid(int textureId);
    bool IsShaderValid(int shaderId);
    //void CDrawImage(int textureId, float x, float y, float width, float height, float rotation, float offsetX = 0, float offsetY = 0);
    void DrawRectRangle(int textureId, float x, float y, float width, float height, float rotation, int offsetX = 0, int offsetY = 0);
    void CDrawImage(int textureId, float x, float y, float width, float height, float rotation, float srcX, float srcY, float srcWidth, float srcHeight);
    void CDrawText(int fontId, std::string message, int x, int y, int size, CommancheColorRGB color = { 255, 255, 255 });
    void CDrawLine(float startx, float starty, float endx, float endy, CommancheColorRGB color = {255, 255, 255});
    void SetFrameSize(int width, int height);
    int GetFrame();
    int CLoadTexture(const std::string& path);
    int LoadShader(const std::string& path, const std::string shaderName);
    CommancheTextureInfo GetTextureInfo(int id);
    int LoadFont(const std::string& path, int size);
    void OffsetCamera(float vertical, float horizontal);
    void DrawGrids();
    void RenderApply();
    void RenderStart();
    void RenderEnd();
    void Destroy();
    static glm::mat4 ProjectionMat;
    glm::mat4 view;
    static int screenWidth;
    static int screenHeight;
    static int ho;
    static int vo;
    private:
    unsigned int framebuffer, textureColorbuffer;
};
