#pragma once
#include "../ecs/system.h"
#include "../game/components.h"
#include "../render/physics_debug_renderer.h"
#include "box2d/box2d.h"

#define PIXELS_PER_METER 1

struct PhyscsBinding {
    Entity entity;
    b2Body* body;

    PhyscsBinding(Entity entity) {
        this->entity = entity;
        this->body = nullptr;
    }
};

class Physics : public System {
    private:
    const b2Vec2 grav = b2Vec2(0.0f, 10.0f * PIXELS_PER_METER);
    std::vector<PhyscsBinding*> bindings;
    b2World* world = new b2World(grav);

    public:
    Physics() {
        PhysicsDebugRenderer* renderer = new PhysicsDebugRenderer();
        world->SetDebugDraw(renderer);
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        flags += b2Draw::e_jointBit;
        flags += b2Draw::e_aabbBit;
        flags += b2Draw::e_pairBit;
        flags += b2Draw::e_centerOfMassBit;
        renderer->SetFlags(flags);

        RequireComponent<Sprite>();
        RequireComponent<RectTransform>();
        RequireComponent<RigidBody>();
        RequireComponent<Mesh>();
    };

    void renderPolygon(b2Body* body) {
        // Retrieve the vertices of the polygon
        b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
        b2Vec2* vertices = shape->m_vertices;
        int numVertices = shape->m_count;

        std::vector<b2Vec2> worldVertices;
        for (int i = 0; i < numVertices; i++) {
            b2Vec2 worldVertex = body->GetLocalPoint(vertices[i]);
            worldVertices.push_back(worldVertex);
        }

        //glPushMatrix();
        // glTranslated(body->GetPosition().x, body->GetPosition().y, 1);
        // glScalef(2, 2, 1);
        //glBegin(GL_POLYGON);
        for (int i = 0; i < numVertices; i++) {
            b2Vec2 worldVertex = worldVertices[i];
            //glVertex2f(worldVertex.x, worldVertex.y);
        }
        //glEnd();
        //glPopMatrix();
    }

    void renderPolygon2(b2Body* body) {
        // Retrieve the position and angle of the body
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();

        // Retrieve the vertices of the polygon
        b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
        b2Vec2* vertices = shape->m_vertices;
        int numVertices = shape->m_count;

        // Save the current modelview matrix
        //glPushMatrix();

        // Translate and rotate the coordinate system to match the body
        //glScalef(10, 10, 1);
        //glTranslatef(position.x, position.y, 0.0);
        //glRotatef(angle * 180.0 / M_PI, 0.0, 0.0, 1.0);

        // Draw the polygon using the local coordinates of the vertices
        //glBegin(GL_POLYGON);
        for (int i = 0; i < numVertices; i++) {
            //glVertex2f(vertices[i].x, vertices[i].y);
        }
        //glEnd();

        // Restore the modelview matrix
        //glPopMatrix();
    }

    void Update() {
        const int32 velocityIterations = 1;
        const int32 positionIterations = 1;


        for (auto entity : GetEntities()) {
            RectTransform transform = entity.GetComponent<RectTransform>();
            RigidBody& rigidBody = entity.GetComponent<RigidBody>();
            Mesh& mesh = entity.GetComponent<Mesh>();

            if (!rigidBody.isInit) {
                rigidBody.isInit = true;

                b2BodyDef bodyDef;
                bodyDef.type = rigidBody.isStatic ? b2_staticBody : b2_dynamicBody;
                bodyDef.position.Set(transform.pos.x, transform.pos.y);

                b2Body* body = world->CreateBody(&bodyDef);
                b2PolygonShape shape;

                float spriteProjectionX = transform.size.x * transform.scale.x;
                float spriteProjectionY = transform.size.y * transform.scale.y;

                //shape.SetAsBox(spriteProjectionX / 2, spriteProjectionY / 2, b2Vec2(spriteProjectionX / 2, spriteProjectionY / 2), 0);


                // leak
                b2Vec2* vec2 = (b2Vec2*)malloc(sizeof(b2Vec2) * mesh.verticiesCount);
                for(int i = 0; i < mesh.verticiesCount; i++)
                  vec2[i] = b2Vec2(mesh.vertices[i].x, mesh.vertices[i].y);

                shape.Set(vec2, mesh.verticiesCount);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &shape;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 1.0f;

                body->CreateFixture(&fixtureDef);

                PhyscsBinding* binding = new PhyscsBinding(entity);
                binding->body = body;
                bindings.push_back(binding);
            }
        }

        float timeStep = 1.0f / 60;

        for (int i = 0; i < bindings.size(); i++) {
            PhyscsBinding* pair = bindings[i];
            RigidBody rigidBody = pair->entity.GetComponent<RigidBody>();

            b2Vec2 accForce = b2Vec2((rigidBody.forceAcc.x * pair->body->GetMass()), (rigidBody.forceAcc.y * pair->body->GetMass()));
            accForce *= PIXELS_PER_METER;
            // pair->body->ApplyTorque(1, true);
            // auto f = pair->body->GetFixtureList();

            pair->body->ApplyForceToCenter(accForce, true);
            pair->body->ApplyLinearImpulseToCenter(accForce, true);
        }

        //glClear(GL_COLOR_BUFFER_BIT);
        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
        world->Step(timeStep, velocityIterations, positionIterations);
        world->ClearForces();
        // world->DebugDraw();

        for (int i = 0; i < bindings.size(); i++) {
            PhyscsBinding* pair = bindings[i];
            renderPolygon2(pair->body);
        }
        //glFlush();

        for (int i = 0; i < bindings.size(); i++) {
            PhyscsBinding* pair = bindings[i];
            RectTransform& transform = pair->entity.GetComponent<RectTransform>();
            RigidBody& rigidBody = pair->entity.GetComponent<RigidBody>();
            rigidBody.forceAcc.x = 0;
            rigidBody.forceAcc.y = 0;
            b2Vec2 position = pair->body->GetPosition();
            float radians = pair->body->GetAngle();

            radians = fmod(radians, 2 * M_PI);
            if (radians < 0.0) {
                radians += 2 * M_PI;
            }

            transform.pos.x = position.x;
            transform.pos.y = position.y;
            transform.rotation = radians;
        }
    }
};
