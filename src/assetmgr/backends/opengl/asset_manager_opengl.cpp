#include "../../../log/log.h"
#include "../../../scripting/lua_manager.h"
#include "../../AssetManager.h"
#include <glm/glm.hpp>

std::map<std::string, int> textures;
std::map<std::string, int> shaders;
std::map<std::string, int> fonts;

CommancheRenderer* crender;

void AssetManager::Initialize(CommancheRenderer* render) {
    crender = render;
    Log::Inf("Asset manager initialized");
}

void AssetManager::AddShader(const std::string& assetId, const std::string& path) {
    int shaderId = crender->CLoadShader(path, assetId);

    if (!crender->IsShaderValid(shaderId)) {
        Log::Warn("Shader with invalid ID %d has tried to load", shaderId);
        return;
    }

    Log::Inf("Shader has loaded id: %d", shaderId);

    shaders.insert(std::make_pair(assetId, shaderId));
}

int AssetManager::AddTexture(const std::string& assetId, const std::string& path) {
    int textureId = crender->CLoadTexture(path);

    if (!crender->IsTextureValid(textureId)) {
        Log::Warn("Texture with invalid ID %d has tried to load", textureId);
        return -1;
    }

    CommancheTextureInfo inf = crender->GetTextureInfo(textureId);
    Log::Inf("Texture has loaded id: %d size: %dx%d", textureId, inf.width, inf.height);
    textures.insert(std::make_pair(assetId, textureId));
    return textureId;
}

void AssetManager::AddFont(const std::string& assetId, const std::string& path, int fontSize) {
    // int fontId = renderer->LoadFont(path, fontSize);
    // fonts.emplace(std::make_pair(assetId, fontId));
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
        Log::Err("font: %s couldn't found in the registry", assetId.c_str());
        return -1;
    }

    return fonts.at(assetId);
}

int AssetManager::GetTexture(const std::string& assetId) {
    if (textures.find(assetId) == textures.end()) {
        Log::Err("texture: %s couldn't found in the registry", assetId.c_str());
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
    CommancheTextureInfo infR = crender->GetTextureInfo(textureId);

    TextureInf inf = {
        .height = infR.height,
        .width = infR.width
    };

    return inf;
}
