#pragma once

#include "../game/entities.h"
#include "../game/components.h"
#include "../game/systems.h"
#include "json.hpp"

enum {
  S_WRITE,
  S_WRITE_NESTED,
  S_END
};

class EngineSerializer {
    public:
    static void SerializeSceneToFile(const std::string& path);
    static void DeserializeFileToScene(const std::string path);
};
