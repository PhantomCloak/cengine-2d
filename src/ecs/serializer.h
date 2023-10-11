#pragma once

#include "../game/components.h"
#include "flecs.h"

enum {
  S_WRITE,
  S_WRITE_NESTED,
  S_END
};

class EngineSerializer {
    public:
    static void SerializeSceneToFile(const std::string& path, flecs::world& world);
    static void DeserializeFileToScene(const std::string path, flecs::world& world);
};
