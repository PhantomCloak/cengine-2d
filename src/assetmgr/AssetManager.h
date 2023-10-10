#pragma once

#include <string>
#include <map>
#include "../render/render.h"

struct TextureInf{
  int width;
  int height;
};

class AssetManager {
  public:
    static void Initialize(CommancheRenderer* render);
    static int AddTexture(const std::string& assetId, const std::string& path);
    static void AddShader(const std::string& assetId, const std::string& path);
    static int GetTexture(const std::string& assetId);
    static int GetShader(const std::string& shaderId);
    static std::string GetTexture(int textureId);
    static TextureInf GetTextureInf(const std::string& assetId);
    static std::vector<std::string> GetLoadedTextures();
    static void AddFont(const std::string& assetId, const std::string& path, int fontSize);
    static int GetFont(const std::string& assetId);

    static std::string GetAssetRootPath() {
      return "assets/";
    }

    static AssetManager* Instance;
#if RENDER_BACKEND_RAYLIB
    static std::map<int, Texture2D> textureMapAct;
#endif
};
