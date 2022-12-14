#pragma once

#include "../game/commanche2d.h"

class ProjectileSystem : public System {
    private:
    std::shared_ptr<EventBus> eventBus;

    public:
    ProjectileSystem() {
        RequireComponent<Projectile>();
    }

    void Update() {
        for (auto entity : GetEntities()) {
            auto projectile = entity.GetComponent<Projectile>();

            if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
                entity.Kill();
            }
        }
    }
};
