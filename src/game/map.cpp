#include "map.h"
#include "../assetmgr/AssetManager.h"
#include "../components/MapTile.h"
#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../log/log.h"
#include "../scripting/lua_manager.h"
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

int mapWidth = 0;
int mapHeight = 0;

std::string tileAssetId;
const int tileSize = 16;
const float tileScale = 4;

Map::Map(std::shared_ptr<World> world) {
    this->world = world;
    LuaManager::RegisterCppToLuaFunc("addTile", &Map::PlaceTile, this);
    isInitialized = world->GetEntitiesByGroup("tiles").size() > 0;
}

void Map::Init(std::string assetId, int tileSize, int tileScale) {
    this->tileAssetId = assetId;

    Log::Inf("Level loader initialized with asset_id: " + assetId + " tile_size: " + std::to_string(tileSize) + " tile_scale: " + std::to_string(tileScale));
}

void Map::TileIndexToTextCoord(const std::string& tileTextureId, int tileIndex, glm::vec2& start, glm::vec2& end) {

    TextureInf mapInf = AssetManager::GetTextureInf(tileTextureId);
    int columns = (int)(mapInf.width / tileSize);

    int startX = tileIndex > columns ? (int)(tileIndex % columns) * tileSize : tileIndex * tileSize;
    int endY = tileIndex > columns ? (int)(floor((tileIndex) / columns)) * tileSize : 0;

    int endX = glm::max(startX - tileSize, 0);
    int startY = glm::max(endY - tileSize, 0);

    start = glm::vec2(startX, startY);
    end = glm::vec2(endX, endX);
}

int Map::PlaceTile(std::string assetId, int x, int y, int zIndex, int tileIndex) {
    glm::vec2 start;
    glm::vec2 end;
    TileIndexToTextCoord(assetId, tileIndex, start, end);

    Entity tile = world->CreateEntity();
    tile.Group("tiles");

    tile.AddComponent<Sprite>(AssetManager::GetTexture(assetId), tileSize, tileSize, zIndex, start.x, start.y);
    tile.AddComponent<MapTile>(tileIndex);
    tile.AddComponent<Transform>(glm::vec2(x, y), glm::vec2(tileScale, tileScale), 0);

    return tile.GetId();
}

void Map::DestroyLevel() {
    Log::Inf("Level loader has destroyed");
    const std::vector<Entity>& tiles = world->GetEntitiesByGroup("tiles");

    for (auto tile : tiles) {
        tile.Kill();
    }
}

int Map::GetMapHeight() {
    return mapHeight;
}

int Map::GetMapWidth() {
    return mapWidth;
}
