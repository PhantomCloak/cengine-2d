#include "AssetManager.h"
#include "../log/log.h"
#include "../scripting/lua_manager.h"
#include "SDL_opengl.h"
#include <glm/glm.hpp>

std::map<std::string, int> textures;
std::map<std::string, int> fonts;

CommancheRenderer* renderer;

void AssetManager::Initialize(CommancheRenderer* render) {
    renderer = render;
    Log::Inf("Asset manager initialized");
    LuaManager::RegisterCppToLuaFunc("addTexture", &AssetManager::AddTexture);
}

void AssetManager::AddTexture(const std::string& assetId, const std::string& path) {
    int textureId = renderer->LoadTexture(path);

    if (!renderer->IsTextureValid(textureId)) {
        Log::Warn("Texture with invalid ID (" + std::to_string(textureId) + ") has tried to load");
        return;
    }

    CommancheTextureInfo inf = renderer->GetTextureInfo(textureId);
    Log::Inf("Texture has loaded id: " + assetId + " size: " + std::to_string(inf.width) + "x" + std::to_string(inf.height));
    textures.insert(std::make_pair(assetId, textureId));
}

void AssetManager::AddFont(const std::string& assetId, const std::string& path, int fontSize) {
    int fontId = renderer->LoadFont(path, fontSize);
    fonts.emplace(std::make_pair(assetId, fontId));
}

std::vector<std::string> AssetManager::GetLoadedTextures() {
    std::vector<std::string> loadedTextures;
    for (std::map<std::string, int>::iterator it = textures.begin(); it != textures.end(); ++it) {
        loadedTextures.push_back(it->first);
    }

    return loadedTextures;
}

int AssetManager::GetFont(const std::string& assetId) {

    if (fonts.find(assetId) == fonts.end()) {
        Log::Err("font: " + assetId + " couldn't found in the registry");
        return -1;
    }

    return fonts.at(assetId);
}

int AssetManager::GetTexture(const std::string& assetId) {
    if (textures.find(assetId) == textures.end()) {
        Log::Err("texture: " + assetId + " couldn't found in the registry");
        return -1;
    }

    return textures.at(assetId);
}

std::string AssetManager::GetTexture(int textureId) {
    for (auto it = textures.begin(); it != textures.end(); ++it)
        if (it->second == textureId)
            return it->first;

    return std::string();
}


TextureInf AssetManager::GetTextureInf(const std::string& assetId) {
    int textureId = textures.at(assetId);
    CommancheTextureInfo infR = renderer->GetTextureInfo(textureId);

    TextureInf inf = {
        .height = infR.height,
        .width = infR.width
    };

    return inf;
}
