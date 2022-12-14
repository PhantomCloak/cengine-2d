#include "AssetManager.h"
#include "../log/log.h"
#include "SDL_opengl.h"
#include <glm/glm.hpp>
#include "../scripting/lua_manager.h"

std::map<std::string, int> textures;
std::map<std::string, int> fonts;
std::vector<std::string> loadedTexutes;

CommancheRenderer* renderer;

void AssetManager::Initialize(CommancheRenderer* render) {
    renderer = render;
    Log::Inf("Asset manager initialized");
    LuaManager::RegisterCppToLuaFunc("addTexture", &AssetManager::AddTexture);
}

void AssetManager::AddTexture(const std::string& assetId, const std::string& path) {
    int textureId = renderer->LoadTexture(path);
    CommancheTextureInfo inf = renderer->GetTextureInfo(textureId);
    Log::Inf("Texture has loaded id: " + assetId + " size: " + std::to_string(inf.width) + "x" + std::to_string(inf.height));
    textures.insert(std::make_pair(assetId, textureId));
    loadedTexutes.push_back(assetId);
}

void AssetManager::AddFont(const std::string& assetId, const std::string& path, int fontSize) {
    int fontId = renderer->LoadFont(path, fontSize);
    fonts.emplace(std::make_pair(assetId, fontId));
}

std::vector<std::string> AssetManager::GetLoadedTextures() {
  return loadedTexutes;
}

int AssetManager::GetFont(const std::string& assetId) {

    if (fonts.find(assetId) == fonts.end()) {
        Log::Err("font: " + assetId + " couldn't found in the registry");
        return -1;
    }

    return fonts.at(assetId);
}

int AssetManager::GetTexture(const std::string& assetId) {
    return textures.at(assetId);
}

std::string AssetManager::GetTexture(int textureId){
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
