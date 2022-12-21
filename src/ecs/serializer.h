#pragma once

#include "../game/entities.h"
#include "../game/components.h"
#include "json.hpp"

enum {
  S_WRITE,
  S_WRITE_NESTED,
  S_END
};

class EngineSerializer {
    public:
    EngineSerializer();
    EngineSerializer(std::shared_ptr<World> world);
    void SerializeWorldToFile(const std::string& path);
    void DeserializeFileToWorld(const std::string path);
};
