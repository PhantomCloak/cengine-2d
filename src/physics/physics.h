#pragma once
#include "../components/RectTransform.h"
#include "../components/RigidBody.h"
#include "../log/log.h"
#include "physics_metadata.h"
#include <map>

#define PIXELS_PER_METER 1

class Physics {
    public:
    static void Initialize();
    static void Update();
    static std::map<int, PhysicsResult> GetUpdatedEntries();
    static bool HasRegistered(int body);
    static void RegisterBody(RigidBody body, RectTransform transform);
    static void SetPosition(RigidBody body, glm::vec2 pos);
    static void AddForce(RigidBody body, glm::vec2 forceVector);
    //static void AddTorque(RigidBody body, float torque);
    //static void Destroy(RigidBody body);
};
