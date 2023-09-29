#include "serializer.h"
#include "../assetmgr/AssetManager.h"
#include "../core/util.h"
#include "../ecs/world.h"
#include "../game/components.h"
#include "../render/render_primitives.h"
//#include "../scene/scene.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeindex>

#define NAMEOF(variable) #variable


// Types
giri::json::JSON SerializeType(CommancheRect rect) {
    giri::json::JSON selfJson = giri::json::JSON();

    selfJson[NAMEOF(rect.x)] = rect.x;
    selfJson[NAMEOF(rect.y)] = rect.y;
    selfJson[NAMEOF(rect.width)] = rect.width;
    selfJson[NAMEOF(rect.height)] = rect.height;

    return selfJson;
}

CommancheRect DeserializeRectType(giri::json::JSON json) {
    CommancheRect rect;
    rect.x = json[NAMEOF(rect.x)].ToFloat();
    rect.y = json[NAMEOF(rect.y)].ToFloat();
    rect.width = json[NAMEOF(rect.width)].ToFloat();
    rect.height = json[NAMEOF(rect.height)].ToFloat();
    return rect;
}

giri::json::JSON SerializeType(glm::vec2 vec2) {
    giri::json::JSON selfJson = giri::json::JSON();
    selfJson[NAMEOF(vec2.x)] = vec2.x;
    selfJson[NAMEOF(vec2.y)] = vec2.y;
    return selfJson;
}

glm::vec2 DeserializeVec2Type(giri::json::JSON json) {
    return glm::vec2(json["vec2.x"].ToFloat(), json["vec2.y"].ToFloat());
}

giri::json::JSON SerializeComponent(Sprite component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["texture"] = AssetManager::GetTexture(component.texture).c_str();
    arrParam["zIndex"] = component.zIndex;
    arrParam["srcRect"] = SerializeType(component.srcRect);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

Sprite DeserializeSprite(giri::json::JSON json) {
    Sprite component;
    giri::json::JSON& parameterNode = json["parameters"];

    component.texture = AssetManager::GetTexture(parameterNode["texture"].ToString());
    component.zIndex = parameterNode["zIndex"].ToInt();
    component.srcRect = DeserializeRectType(parameterNode["srcRect"]);

    return component;
}

RectTransform DeserializeTransform(giri::json::JSON json) {
    RectTransform component;
    giri::json::JSON& parameterNode = json["parameters"];

    component.pos = DeserializeVec2Type(parameterNode[NAMEOF(component.pos)]);
    component.size = DeserializeVec2Type(parameterNode[NAMEOF(component.size)]);
    component.rotation = parameterNode[NAMEOF(component.rotation)].ToInt();
    component.scale = DeserializeVec2Type(parameterNode[NAMEOF(component.scale)]);

    return component;
}

giri::json::JSON SerializeComponent(RectTransform component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam[NAMEOF(component.pos)] = SerializeType(component.pos);
    arrParam[NAMEOF(component.rotation)] = component.rotation;
    arrParam[NAMEOF(component.size)] = SerializeType(component.size);
    arrParam[NAMEOF(component.scale)] = SerializeType(component.scale);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

giri::json::JSON SerializeComponent(BoxCollider component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam[NAMEOF(component.width)] = component.width;
    arrParam[NAMEOF(component.height)] = component.height;
    arrParam[NAMEOF(component.offset)] = SerializeType(component.offset);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

void DeserializeEntity(giri::json::JSON json) {
    //Entity entity = Scene::CreateEntity();
    //giri::json::JSON componentsNode = json["components"];

    //if (componentsNode.hasKey("sprite")) {
    //    Sprite comp = DeserializeSprite(componentsNode["sprite"]);
    //    entity.AddComponent<Sprite>(comp);
    //}

    //if (componentsNode.hasKey("transform")) {
    //    RectTransform comp = DeserializeTransform(componentsNode["transform"]);
    //    entity.AddComponent<RectTransform>(comp);
    //}
}


giri::json::JSON SerializeWorldStorage(WorldStorage* storage) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();
    arrParam = arrParam["systems"];

    for (auto systemEntry : storage->systems) {
      arrParam.append(systemEntry.first.name());
    }

    selfJson["parameters"] = arrParam;

    return selfJson;
}


giri::json::JSON SerializeComponent(RigidBody component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["velocity"] = SerializeType(component.velocityLinear);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

void EngineSerializer::SerializeSceneToFile(const std::string& path) {
    //std::vector<Entity> entities = Scene::GetEntities();

    //giri::json::JSON jsonState = giri::json::Object();
    //giri::json::JSON arrEntities = giri::json::Array();
    //giri::json::JSON arrSystems = giri::json::Array();
    //arrSystems = SerializeWorldStorage(Scene::world->storage);


    //for (int i = 0; i < entities.size(); i++) {
    //    auto entity = entities[i];
    //    giri::json::JSON enttityJsonNode = giri::json::Object();
    //    giri::json::JSON componentNode = giri::json::Array();

    //    if (entity.HasComponent<RectTransform>())
    //        componentNode["transform"] = SerializeComponent(entity.GetComponent<RectTransform>());
    //    if (entity.HasComponent<Sprite>())
    //        componentNode["sprite"] = SerializeComponent(entity.GetComponent<Sprite>());

    //    enttityJsonNode["components"] = componentNode;
    //    arrEntities[i] = enttityJsonNode;
    //}

    //jsonState["entities"] = arrEntities;
    //jsonState["systems"] = arrSystems;


    //std::ofstream myfile;
    //myfile.open(path);
    //myfile << jsonState.ToString();
    //myfile.close();
}

void EngineSerializer::DeserializeFileToScene(const std::string path) {
    //std::ifstream fs(path);
    //std::stringstream buffer;
    //buffer << fs.rdbuf();

    //std::string jsonOutput = buffer.str();

    //giri::json::JSON jsonRootNode = giri::json::JSON::Load(jsonOutput);
    //giri::json::JSON entitiesNode = jsonRootNode["entities"];
    //giri::json::JSON systemsNode = jsonRootNode["systems"]["parameters"];

    //for (auto& entity : entitiesNode.ArrayRange()) {
    //    DeserializeEntity(entity);
    //}

    //for (auto& system : systemsNode.ArrayRange()) {
    //    std::string systemId = system.ToString();
    //    Scene::SetSystemStatus(systemId, true);
    //}
}
