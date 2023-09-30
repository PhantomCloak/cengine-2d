#include "serializer.h"
#include "../assetmgr/AssetManager.h"
#include "../core/util.h"
#include "../ecs/world.h"
#include "../game/components.h"
#include "../render/render_primitives.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeindex>

#define NAMEOF(variable) #variable


bool hasRegisterTypesInit = false;
void RegisterTypes(flecs::world& world) {
    if (hasRegisterTypesInit)
        return;
    hasRegisterTypesInit = true;

    world.component<std::string>()
    .opaque(flecs::String) // Opaque type that maps to string
    .serialize([](const flecs::serializer* s, const std::string* data) {
        const char* str = data->c_str();
        return s->value(flecs::String, &str); // Forward to serializer
    })
    .assign_string([](std::string* data, const char* value) {
        *data = value; // Assign new value to std::string
    });

    world.component<CommancheRect>()
    .member<float>("x")
    .member<float>("y")
    .member<float>("height")
    .member<float>("width");

    world.component<glm::vec2>("vec2")
    .member<float>("x")
    .member<float>("y");

    world.component<RectTransform>()
    .member<glm::vec2>("pos")
    .member<glm::vec2>("size")
    .member<glm::vec2>("scale")
    .member<int>("rotation");

    world.component<Sprite>()
    .member<int>("texture")
    .member<int>("zIndex")
    .member<bool>("isFixed")
    .member<CommancheRect>("srcRect");

    world.component<BoxCollider>()
    .member<float>("width")
    .member<float>("height")
    .member<glm::vec2>("offset");

    world.component<RigidBody>()
    .member<glm::vec2>("velocityLinear");
}

void EngineSerializer::SerializeSceneToFile(const std::string& path, flecs::world& world) {
    RegisterTypes(world);
    flecs::snapshot snapshot(world);
    snapshot.take();

    const ecs_world_to_json_desc_t desc = { 0 };
    const char* json_1 = ecs_world_to_json(world.c_ptr(), &desc); std::cout << json_1;

    std::string json(json_1);

    std::ofstream outfile;
    outfile.open(path);
    outfile << json;
    outfile.close();
}

void EngineSerializer::DeserializeFileToScene(const std::string path, flecs::world& world) {
    RegisterTypes(world);
    std::ifstream fs(path);
    std::stringstream buffer;
    buffer << fs.rdbuf();
    std::string jsonOutput = buffer.str();

    const ecs_from_json_desc_t desc = { 0 };
    ecs_world_from_json(world.c_ptr(), jsonOutput.c_str(), &desc);
}
