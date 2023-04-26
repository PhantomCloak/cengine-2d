#include "physics.h"
#include <iostream>

const b2Vec2 grav = b2Vec2(0.0f, 10.0f * PIXELS_PER_METER);
static b2World* world = nullptr;
static std::map<int, bool> registeredBody;

void Physics::Initialize() {
    if (world == nullptr) {
        world = new b2World(grav);
    }
}

b2Body* findBodyById(int id) {
    b2Body* body = world->GetBodyList();
    b2Body* curBody = body;

    do {
        b2BodyUserData& uData = curBody->GetUserData();
        PhysicsMetadata* meta = (PhysicsMetadata*)uData.pointer;
        if (meta->identifier == id)
            return curBody;
    } while ((curBody = curBody->GetNext()) != nullptr);

    assert(false);
}

void Physics::AddForce(RigidBody body, glm::vec2 forceVector) {
    b2Body* bBody = findBodyById(body.rigId);
    int fX = (forceVector.x * bBody->GetMass());
    int fY = (forceVector.y * bBody->GetMass());

    bBody->ApplyLinearImpulseToCenter(b2Vec2(fX, fY), true);
}

void Physics::SetPosition(RigidBody body, glm::vec2 pos) {
    b2Body* bBody = findBodyById(body.rigId);
    bBody->SetTransform(b2Vec2(pos.x, pos.y), bBody->GetAngle());
}

void Physics::RegisterBody(RigidBody body, RectTransform transform) {
    PhysicsMetadata* meta = new PhysicsMetadata();
    meta->identifier = body.rigId;

    b2BodyDef bodyDef;
    bodyDef.type = body.isStatic ? b2_staticBody : b2_dynamicBody;
    bodyDef.position.Set(transform.pos.x, transform.pos.y);
    bodyDef.userData.pointer = (uintptr_t)meta;

    b2Body* bBody = world->CreateBody(&bodyDef);

    bBody->SetFixedRotation(body.isFixedRot);

    b2PolygonShape shape;

    shape.SetAsBox(((transform.size.x * transform.scale.x) / PIXELS_PER_METER ) / 2, ((transform.size.y * transform.scale.y) / PIXELS_PER_METER) / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;

    fixtureDef.density = body.density;
    fixtureDef.friction = body.friction;
    fixtureDef.restitution = body.restution;

    bBody->CreateFixture(&fixtureDef);
    registeredBody.insert(std::make_pair(body.rigId, true));
}

bool Physics::HasRegistered(int body) {
    if (registeredBody.find(body) == registeredBody.end())
        return false;

    return registeredBody[body];
}


void Physics::Update() {
    static const float timeStep = 1.0f / 60;
    static const int32 velocityIterations = 1;
    static const int32 positionIterations = 1;

    world->Step(timeStep, velocityIterations, positionIterations);
    world->ClearForces();
}

std::map<int, PhysicsResult> Physics::GetUpdatedEntries() {
    b2Body* body = world->GetBodyList();
    std::map<int, PhysicsResult> results;

    b2Body* curBody = body;

    do {
        b2BodyUserData& uData = curBody->GetUserData();

        if (uData.pointer == 0) {
            Log::Err("Registered body has no user data on it it! index: ");
            continue;
        }

        PhysicsMetadata* meta = (PhysicsMetadata*)uData.pointer;
        PhysicsResult result;
        b2Vec2 linVel = curBody->GetLinearVelocity();
        b2Vec2 bPos = curBody->GetPosition();

        result.position = glm::vec2(bPos.x, bPos.y);
        result.mass = curBody->GetMass();
        result.velocityLinear = glm::vec2(linVel.x, linVel.y);
        result.velocityAngular = curBody->GetAngularVelocity();
        result.rotation = curBody->GetAngle();


        results.insert(std::make_pair(meta->identifier, result));
    } while ((curBody = curBody->GetNext()) != nullptr);

    return results;
}
