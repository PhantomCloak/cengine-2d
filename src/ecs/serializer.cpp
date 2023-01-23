#include "serializer.h"
#include "../assetmgr/AssetManager.h"
#include "../ecs/world.h"
#include "../game/components.h"
#include "../render/render_primitives.h"
#include <fstream>
#include <iostream>
#include <sstream>


static std::shared_ptr<World> world;
EngineSerializer::EngineSerializer(std::shared_ptr<World> worldRef) {
    world = worldRef;
}

giri::json::JSON SerializeType(CommancheRect rect) {
    giri::json::JSON selfJson = giri::json::JSON();
    selfJson["x"] = rect.x;
    selfJson["y"] = rect.y;
    selfJson["h"] = rect.height;
    selfJson["w"] = rect.width;
    return selfJson;
}

giri::json::JSON SerializeType(glm::vec2 vec2) {
    giri::json::JSON selfJson = giri::json::JSON();
    selfJson["x"] = vec2.x;
    selfJson["y"] = vec2.y;
    return selfJson;
}

glm::vec2 DeserializeVec2Type(giri::json::JSON json) {
    return glm::vec2(json["x"].ToFloat(), json["y"].ToFloat());
}

CommancheRect DeserializeSDLRectType(giri::json::JSON json) {
    CommancheRect rect;
    rect.x = json["x"].ToInt();
    rect.y = json["y"].ToInt();
    rect.width = json["w"].ToInt();
    rect.height = json["h"].ToInt();
    return rect;
}

giri::json::JSON SerializeComponent(Sprite component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["texture"] = AssetManager::GetTexture(component.texture).c_str();
    //arrParam["width"] = component.width;
    //arrParam["height"] = component.height;
    arrParam["zIndex"] = component.zIndex;
    arrParam["srcRect"] = SerializeType(component.srcRect);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

Sprite DeserializeSprite(giri::json::JSON json) {
    Sprite component;
    giri::json::JSON& parameterNode = json["parameters"];

    component.texture = AssetManager::GetTexture(parameterNode["texture"].ToString());
    //component.width = parameterNode["width"].ToInt();
    //component.height = parameterNode["height"].ToInt();
    component.zIndex = parameterNode["zIndex"].ToInt();
    component.srcRect = DeserializeSDLRectType(parameterNode["srcRect"]);

    return component;
}

Transform DeserializeTransform(giri::json::JSON json) {
    Transform component;
    giri::json::JSON& parameterNode = json["parameters"];

    component.pos = DeserializeVec2Type(parameterNode["pos"]);
    component.rotation = parameterNode["rotation"].ToInt();
    component.scale = DeserializeVec2Type(parameterNode["scale"]);

    return component;
}

BoxCollider DeserializeBoxCollider(giri::json::JSON json) {
    BoxCollider component;
    giri::json::JSON& parameterNode = json["parameters"];

    component.width = parameterNode["width"].ToInt();
    component.height = parameterNode["height"].ToInt();
    component.offset = DeserializeVec2Type(parameterNode["offset"]);

    return component;
}

giri::json::JSON SerializeComponent(Transform component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["pos"] = SerializeType(component.pos);
    arrParam["rotation"] = component.rotation;
    arrParam["scale"] = SerializeType(component.scale);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

giri::json::JSON SerializeComponent(Camera component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    selfJson["parameters"] = arrParam;

    return selfJson;
}


giri::json::JSON SerializeComponent(Animation component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["numFrames"] = component.numFrames;
    arrParam["frameSpeed"] = component.frameSpeed;
    arrParam["isLoop"] = component.isLoop;

    selfJson["parameters"] = arrParam;

    return selfJson;
}


giri::json::JSON SerializeComponent(RigidBody component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["velocity"] = SerializeType(component.velocity);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

giri::json::JSON SerializeComponent(BoxCollider component) {
    giri::json::JSON selfJson = giri::json::JSON();
    giri::json::JSON arrParam = giri::json::JSON();

    arrParam["width"] = component.width;
    arrParam["height"] = component.height;
    arrParam["offset"] = SerializeType(component.offset);

    selfJson["parameters"] = arrParam;

    return selfJson;
}

void DeserializeEntity(giri::json::JSON json) {
    Entity entity = world->CreateEntity();
    giri::json::JSON componentsNode = json["components"];

    if (componentsNode.hasKey("sprite")) {
        Sprite comp = DeserializeSprite(componentsNode["sprite"]);
        entity.AddComponent<Sprite>(comp);
    }
    if (componentsNode.hasKey("transform")) {
        Transform comp = DeserializeTransform(componentsNode["transform"]);
        entity.AddComponent<Transform>(comp);
    }
    if (componentsNode.hasKey("boxcollider")) {
        BoxCollider comp = DeserializeBoxCollider(componentsNode["boxcollider"]);
        entity.AddComponent<BoxCollider>(comp);
    }
}

void EngineSerializer::SerializeWorldToFile(const std::string& path) {
    std::vector<Entity> entities = world->GetComponentEntities<Serializable>();

    giri::json::JSON jsonState = giri::json::Object();
    giri::json::JSON arrEntities = giri::json::Array();

    for (int i = 0; i < entities.size(); i++) {
        auto entity = entities[i];
        giri::json::JSON enttityJsonNode = giri::json::Object();
        giri::json::JSON componentNode = giri::json::Array();

        // Serialize It's components
        if (entity.HasComponent<Camera>())
            componentNode["camera"] = SerializeComponent(entity.GetComponent<Camera>());
        if (entity.HasComponent<Transform>())
            componentNode["transform"] = SerializeComponent(entity.GetComponent<Transform>());
        if (entity.HasComponent<Sprite>())
            componentNode["sprite"] = SerializeComponent(entity.GetComponent<Sprite>());
        if (entity.HasComponent<RigidBody>())
            componentNode["rigidbody"] = SerializeComponent(entity.GetComponent<RigidBody>());
        if (entity.HasComponent<BoxCollider>())
            componentNode["boxcollider"] = SerializeComponent(entity.GetComponent<BoxCollider>());
        if (entity.HasComponent<RigidBody>())
            componentNode["animation"] = SerializeComponent(entity.GetComponent<Animation>());

        enttityJsonNode["components"] = componentNode;
        arrEntities[i] = enttityJsonNode;
    }

    jsonState["entities"] = arrEntities;


    std::ofstream myfile;
    myfile.open(path);
    myfile << jsonState.ToString();
    myfile.close();
}

void EngineSerializer::DeserializeFileToWorld(const std::string path) {
    std::ifstream fs(path);
    std::stringstream buffer;
    buffer << fs.rdbuf();

    std::string jsonOutput = buffer.str();

    giri::json::JSON jsonRootNode = giri::json::JSON::Load(jsonOutput);
    giri::json::JSON entitiesNode = jsonRootNode["entities"];

    for (auto& entity : entitiesNode.ArrayRange()) {
        DeserializeEntity(entity);
    }
}
