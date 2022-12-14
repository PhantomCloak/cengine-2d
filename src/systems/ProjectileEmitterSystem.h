#pragma once
#include "../game/commanche2d.h"
#include "SDL.h"

class ProjectileEmitterSystem : public System {
    private:
    std::shared_ptr<World> world;

    public:
    ProjectileEmitterSystem(std::shared_ptr<World> world, std::shared_ptr<EventBus> eventBus) {
        this->world = world;
        RequireComponent<ProjectileEmitter>();
        RequireComponent<Transform>();
        SubscribeToEvents(eventBus);
    }

    void SubscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
        eventBus->SubscribeEvent(this, &ProjectileEmitterSystem::onKeyPressed);
    }

    void onKeyPressed(KeyPressedEvent& event) {
        if (event.symbol == SDLK_SPACE) {
            for (auto entity : GetEntities()) {
                if (entity.HasTag("player")) {
                    const auto projectileEmitter = entity.GetComponent<ProjectileEmitter>();
                    const auto transform = entity.GetComponent<Transform>();
                    const auto rigidbody = entity.GetComponent<RigidBody>();

                    // If parent entity has sprite, start the projectile position in the middle of the entity
                    glm::vec2 projectilePosition = transform.pos;
                    if (entity.HasComponent<Sprite>()) {
                        auto sprite = entity.GetComponent<Sprite>();
                        projectilePosition.x += (transform.scale.x * sprite.width);
                        projectilePosition.y += (transform.scale.y * sprite.height / 2);
                    }

                    // If parent entity direction is controlled by the keyboard keys, modify the direction of the projectile accordingly
                    glm::vec2 projectileVelocity = projectileEmitter.particleVelocity;
                    int directionX = 0;
                    int directionY = 0;
                    if (rigidbody.velocity.x > 0)
                        directionX = +1;
                    if (rigidbody.velocity.x < 0)
                        directionX = -1;
                    if (rigidbody.velocity.y > 0)
                        directionY = +1;
                    if (rigidbody.velocity.y < 0)
                        directionY = -1;
                    projectileVelocity.x = projectileEmitter.particleVelocity.x * directionX;
                    projectileVelocity.y = projectileEmitter.particleVelocity.y * directionY;

                    // Create new projectile entity and add it to the world
                    Entity projectile = world->CreateEntity();
                    projectile.Group("projectiles");
                    projectile.AddComponent<Transform>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                    projectile.AddComponent<BoxCollider>(5, 5);
                    projectile.AddComponent<RigidBody>(projectileVelocity);
                    Log::Inf(std::to_string(projectileVelocity.x));
                    Log::Inf(std::to_string(projectileVelocity.y));
                    projectile.AddComponent<Sprite>(AssetManager::GetTexture("bullet"), 5, 5, 4);
                    projectile.AddComponent<Projectile>(true, 20, projectileEmitter.particleDuration);
                }
            }
        }
    }

    void Update() {
        for (auto entity : GetEntities()) {
            if (entity.HasTag("player"))
                continue;

            auto& projectileEmitter = entity.GetComponent<ProjectileEmitter>();
            const auto transform = entity.GetComponent<Transform>();

            // If emission frequency is zero, bypass re-emission logic
            if (projectileEmitter.repeatFrequency == 0) {
                continue;
            }

            // Check if its time to re-emit a new projectile
            if (SDL_GetTicks() - projectileEmitter.lastEmitTime > projectileEmitter.repeatFrequency) {
                glm::vec2 projectilePosition = transform.pos;
                if (entity.HasComponent<Sprite>()) {
                    const auto sprite = entity.GetComponent<Sprite>();
                    projectilePosition.x += (transform.scale.x * sprite.width);
                    projectilePosition.y += (transform.scale.y * sprite.height / 2);
                }

                // Add a new projectile entity to the registry
                Entity projectile = world->CreateEntity();
                projectile.Group("projectiles");
                projectile.AddComponent<Transform>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
                projectile.AddComponent<RigidBody>(projectileEmitter.particleVelocity);
                projectile.AddComponent<Sprite>(AssetManager::GetTexture("bullet"), 5,5, 4);
                projectile.AddComponent<BoxCollider>(5,5);
                projectile.AddComponent<Projectile>(projectileEmitter.isFriendly, projectileEmitter.projectileDamage, projectileEmitter.particleDuration);

                // Update the projectile emitter component last emission to the current milliseconds
                projectileEmitter.lastEmitTime = SDL_GetTicks();
            }
        }
    }
};
