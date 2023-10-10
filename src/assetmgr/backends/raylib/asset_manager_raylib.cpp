#include "../../AssetManager.h"
#include "../../../log/log.h"
#include <raylib.h>
#include <string>
#include <map>
#include <vector>

std::map<std::string, Shader> shaderMap;
std::map<std::string, Font> fontMap;
std::map<std::string, Texture2D> textureMap;

std::map<int, Texture2D> AssetManager::textureMapAct;

void AssetManager::Initialize(CommancheRenderer* render) {
    // Initialization code here if needed
    Log::Inf("Asset manager initialized with raylib");
}

int AssetManager::AddTexture(const std::string& assetId, const std::string& path) {
    Texture2D texture = LoadTexture(path.c_str());
    if (!texture.id) {
        Log::Warn("Texture loading failed for ID: " + assetId);
        return -1;
    }
    textureMap[assetId] = texture;
    textureMapAct[texture.id] = texture;
    Log::Inf("Texture loaded for ID: " + assetId);

    return texture.id;
}

void AssetManager::AddShader(const std::string& assetId, const std::string& path) {
    Shader shader = LoadShader(nullptr, path.c_str());
    if (!shader.id) {
        Log::Warn("Shader loading failed for ID: " + assetId);
        return;
    }
    shaderMap[assetId] = shader;
}

int AssetManager::GetTexture(const std::string& assetId) {
    if (textureMap.find(assetId) == textureMap.end()) {
        return -1;  // Indicates failure
    }
    return textureMap[assetId].id;
}

int AssetManager::GetShader(const std::string& shaderId) {
    if (shaderMap.find(shaderId) == shaderMap.end()) {
        return -1;  // Indicates failure
    }
    return shaderMap[shaderId].id;
}

std::string AssetManager::GetTexture(int textureId) {
    for (const auto& pair : textureMap) {
        if (pair.second.id == textureId) {
            return pair.first;
        }
    }
    return "";  // Indicates not found
}

TextureInf AssetManager::GetTextureInf(const std::string& assetId) {
    Texture2D texture = textureMap[assetId];
    TextureInf inf = {
        .width = texture.width,
        .height = texture.height
    };
    return inf;
}

std::vector<std::string> AssetManager::GetLoadedTextures() {
    std::vector<std::string> textureNames;
    for (const auto& pair : textureMap) {
        textureNames.push_back(pair.first);
    }
    return textureNames;
}

void AssetManager::AddFont(const std::string& assetId, const std::string& path, int fontSize) {
    Font font = LoadFontEx(path.c_str(), fontSize, 0, 0);
    if (!font.baseSize) {
        Log::Warn("Font loading failed for ID: " + assetId);
        return;
    }
    fontMap[assetId] = font;
}

int AssetManager::GetFont(const std::string& assetId) {
    if (fontMap.find(assetId) == fontMap.end()) {
        return -1;  // Indicates failure
    }
    return fontMap[assetId].baseSize;  // Return the base size of the font as its identifier
}

