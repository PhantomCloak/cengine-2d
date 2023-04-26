#pragma once

#include "../render.h"
#include "glm/glm.hpp"
#include "raylib.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <unordered_map>

std::unordered_map<int, Shader> ryShaders;
std::unordered_map<int, Texture2D> ryTextures;

int GenerateTextureFromText(const std::string& text, int fontId, Color textColor) {
    return -1;
}

int CommancheRenderer::LoadFont(const std::string& path, int fontSize) {
    return -1;
}
void CommancheRenderer::Initialize(const std::string& title, int windowWidth, int windowHeight) {
    InitWindow(windowWidth, windowHeight, title.c_str());
        SetTargetFPS(60);
}

void CommancheRenderer::InitializeShaders(const std::string& defaultShaderPath) {
    // int defaultShader = LoadShader("default.vert", "default.frag");
}

int CommancheRenderer::GetFrame() {
    return 0;
}


void CommancheRenderer::CDrawImage(int textureId, float x, float y, float width, float height, float rotation, int offsetX, int offsetY) {
    Texture2D texture = ryTextures[textureId];
    Vector2 position = { x, y };
    Vector2 origin = { (float)offsetX, (float)offsetY };
    Vector2 scale = { width / texture.width, height / texture.height };
    DrawTexturePro(texture, { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) }, { position.x, position.y, width, height }, origin, rotation, WHITE);
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

    // DrawText(message.c_str(), x, y, size, 2, clr);
    DrawText("text", 0, 0, 10, GRAY);
}

int CommancheRenderer::LoadShader(const std::string& path, const std::string shaderName) {
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
    inf.width = texture.width;
    inf.height = texture.height;

    return inf;
}

void CommancheRenderer::SetFrameSize(int width, int height) {
}


void CommancheRenderer::RenderStart() {
    ClearBackground(RAYWHITE);
    BeginDrawing();
}

void CommancheRenderer::RenderEnd() {
    EndDrawing();
}

void CommancheRenderer::RenderApply() {
}

void CommancheRenderer::Destroy() {
}
