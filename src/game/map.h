#pragma once

#include "../ecs/world.h"
#include "SDL_render.h"
#include <glm/glm.hpp>
#include <iostream>
#include <sol/sol.hpp>
#include <vector>

class Map {
    public:
    Map(std::shared_ptr<World> world, int tileSize, float tileScale);
    void Init(std::string assetName, int tileSize, int scale);
    void DestroyLevel();
    int PlaceTile(std::string assetId, int x, int y, int zIndex, int tileIndex);
    int GetMapHeight();
    int GetMapWidth();
    void TileIndexToTextCoord(const std::string& tileTextureId, int tileIndex, glm::vec2& start, glm::vec2& end);
    int GetEntityFromTileIndex(int tileIndex);
    bool isInitialized;

    private:
    int tileSize;
    float tileScale;
    std::string tileAssetId;
    void SetLevelTileMatrix();
    std::shared_ptr<World> world;
    std::vector<std::vector<int>> levelTile;
};
