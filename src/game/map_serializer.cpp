#include "map_serializer.h"
#include "commanche2d.h"
#include <fstream>
#include <iterator>
#include <string>
#include <vector>


std::vector<std::string> MapLuaSerializer::EntityToEntry(std::vector<Entity> entities) {
    std::vector<std::string> entries;
    entries.push_back("tiles = {}");
    for (auto entity : entities) {
        auto transform = &entity.GetComponent<Transform>();
        auto sprite = &entity.GetComponent<Sprite>();
        auto tile = &entity.GetComponent<MapTile>();

        entries.push_back("table.insert(tiles, addTile(\"" + AssetManager::GetTexture(sprite->texture) + "\"," + std::to_string((int)transform->pos.x) + "," + std::to_string((int)transform->pos.y) + "," + std::to_string(sprite->zIndex) + "," + std::to_string(tile->tileIndex) + "))");
    }

    return entries;
}

void MapLuaSerializer::SaveEntry(std::string path, std::vector<std::string> entries) {
    std::remove("./assets/scripts/town.lua");
    std::ofstream output_file("./assets/scripts/after_load/town.lua");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(entries.begin(), entries.end(), output_iterator);
}
