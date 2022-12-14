#pragma once

#include "../ecs/entity.h"
#include <string>

class IMapSerializer {
    public:
    virtual std::vector<std::string> EntityToEntry(std::vector<Entity> entities) = 0;
    virtual void SaveEntry(std::string path, std::vector<std::string> entries) = 0;
};

class MapLuaSerializer : virtual IMapSerializer {
    public:
    std::vector<std::string> EntityToEntry(std::vector<Entity> entities);
    void SaveEntry(std::string path, std::vector<std::string> entries);
};
