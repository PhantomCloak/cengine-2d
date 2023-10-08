#pragma once

#include "../../../assetmgr/AssetManager.h"
#include "../../../core/coordinate_system.h"
#include "../../render.h"
#include "glm/glm.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "rlgl.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <unordered_map>

std::unordered_map<int, Shader> ryShaders;
std::unordered_map<int, Texture2D> ryTextures;
glm::mat4 CommancheRenderer::ProjectionMat;

int CommancheRenderer::vo = 0;
int CommancheRenderer::ho = 0;

int CommancheRenderer::screenWidth = 1920;
int CommancheRenderer::screenHeight = 1080;

CommancheRenderer* CommancheRenderer::Instance;

int GenerateTextureFromText(const std::string& text, int fontId, Color textColor) {
    return -1;
}

int CommancheRenderer::LoadFont(const std::string& path, int fontSize) {
    return -1;
}

void CommancheRenderer::UpdateRenderTexture(glm::vec2 size) {
    printf("Updating render texture with size %f x %f\n", size.x, size.y);
    UnloadRenderTexture(viewTexture);
    viewTexture = LoadRenderTexture(size.x, size.y);

    camX.target = (Vector2){ size.x / 2.0f, size.y / 2.0f };
    camX.offset = (Vector2){ size.x / 2.0f, size.y / 2.0f };
    camX.zoom = 1.0f;
}
void CommancheRenderer::Initialize(const std::string& title, int windowWidth, int windowHeight) {
    Instance = this;

    std::string titleStr = title + " - Backend [Raylib]";
    InitWindow(1920, 1080, titleStr.c_str());
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    rlImGuiSetup(true);
}

void CommancheRenderer::InitializeShaders(const std::string& defaultShaderPath) {
    // int defaultShader = LoadShader("default.vert", "default.frag");
}

int CommancheRenderer::GetFrame() {
    return viewTexture.texture.id;
}

void CommancheRenderer::DrawGrids() {
    static bool isInit = false;
    static Vector2 vertices[81 * 4];
    static int lastScreenWidth = -1;  // Initialize with a value that is unlikely to be a screen width
    static int lastScreenHeight = -1; // Initialize with a value that is unlikely to be a screen height


    const int gridSize = 80;
    float gridWidth = 5;
    float gridHeight = 5;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Convert grid dimensions to current screen resolution
    CoordinateCalculator::ConvertMetersToPixels(gridWidth, gridHeight);

    if (!isInit || screenWidth != lastScreenWidth || screenHeight != lastScreenHeight) { // Check if screen dimensions have changed
        int index = 0;

        for (int i = 0; i <= gridSize; i++) {
            // Horizontal lines
            vertices[index].x = 0;
            vertices[index].y = i * gridHeight;
            index++;
            vertices[index].x = screenWidth;
            vertices[index].y = i * gridHeight;
            index++;

            // Vertical lines
            vertices[index].x = i * gridWidth;
            vertices[index].y = 0;
            index++;
            vertices[index].x = i * gridWidth;
            vertices[index].y = screenHeight;
            index++;
        }

        lastScreenWidth = screenWidth;
        lastScreenHeight = screenHeight;
        isInit = true;
    }

    // Set the line color
    Color gridColor = { 0, 0, 0, 77 }; // RGBA: (0, 0, 0, 0.3 * 255)

    // Draw the grid lines
    for (int i = 0; i < (gridSize + 1) * 4; i += 2) {
        DrawLineV(vertices[i], vertices[i + 1], gridColor);
    }
}


void CommancheRenderer::DrawRectRangle(float x, float y, float width, float height, float rotation) {
    CoordinateCalculator::ConvertMetersToPixels(x, y);

    Rectangle destRec = { x, y, width, height };
    Vector2 origin = { width / 2.0f, height / 2.0f };
    DrawRectanglePro(destRec, origin, rotation, RED);
}

void CommancheRenderer::CDrawImage(int textureId, float x, float y, float width, float height, float rotation, float srcX, float srcY, float srcWidth, float srcHeight, CommancheColorRGBA color) {

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    CoordinateCalculator::ConvertMetersToPixels(x, y);
    CoordinateCalculator::ConvertMetersToPixels(width, height);

    Texture2D texture = AssetManager::textureMapAct[textureId];

    Rectangle sourceRec = { srcX, srcY, srcWidth, srcHeight };
    Rectangle destRec = { x, y, width, height };
    Vector2 origin = { width / 2.0f, height / 2.0f };

    DrawTexturePro(texture, sourceRec, destRec, origin, rotation, Color({ static_cast<unsigned char>(color.r), static_cast<unsigned char>(color.g), static_cast<unsigned char>(color.b), static_cast<unsigned char>(color.a) })); // WHITE means no tint
}

bool CommancheRenderer::IsShaderValid(int shaderId) {
    Shader shader = ryShaders[shaderId];
    return shader.id != 0;
}

bool CommancheRenderer::IsTextureValid(int textureId) {
    if (ryTextures.find(textureId) == ryTextures.end()) {
        return false;
    }
    return true;
}

void CommancheRenderer::CDrawText(int fontId, std::string message, int x, int y, int size, CommancheColorRGB color) {
    Color clr;
    clr.a = 255;
    clr.r = color.r;
    clr.g = color.g;
    clr.b = color.b;

    DrawText("text", 0, 0, 10, GRAY);
}

int CommancheRenderer::CLoadShader(const std::string& path, const std::string shaderName) {
    return 0;
}

int CommancheRenderer::CLoadTexture(const std::string& path) {
    Texture2D tex = LoadTexture(path.c_str());
    ryTextures[tex.id] = tex;
    return tex.id;
}

CommancheTextureInfo CommancheRenderer::GetTextureInfo(int id) {
    Texture2D texture = ryTextures[id];

    CommancheTextureInfo inf;
    // inf.width = texture.width;
    // inf.height = texture.height;

    return inf;
}

void CommancheRenderer::OffsetCamera(float vertical, float horizontal) {
    camX.offset.x -= vertical;
    camX.offset.y -= horizontal;
    // vo += vertical;
    // ho += horizontal;
}

void CommancheRenderer::BeginDraw() {
    ClearBackground(RAYWHITE);
    BeginTextureMode(viewTexture);
    BeginMode2D(camX);
}

void CommancheRenderer::EndDraw() {
    EndMode2D();
    EndTextureMode();
}
void CommancheRenderer::RenderStart() {
    BeginDrawing();
}

void CommancheRenderer::RenderEnd() {
    EndDrawing();
}

void CommancheRenderer::Destroy() {
}
