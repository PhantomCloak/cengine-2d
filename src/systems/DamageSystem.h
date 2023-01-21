#pragma once
#include "../components/Health.h"
#include "../components/BoxCollider.h"
#include "../components/Projectile.h"
#include "../ecs/system.h"
#include "../ecs/world.h"
#include "../events/CollisionEvent.h"

class DamageSystem : public System {
    private:
    std::shared_ptr<EventBus> eventBus;

    public:
    DamageSystem(std::shared_ptr<EventBus> eventBus) {
        this->eventBus = eventBus;
        RequireComponent<BoxCollider>();
        SubscribeToEvents(eventBus);
    }

    void SubscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
        //eventBus->SubscribeEvent(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent& event) {
        Entity a = event.a;
        Entity b = event.b;

        Log::Inf("Damage hit between " + std::to_string(a.GetId()) + " and " + std::to_string(b.GetId()));
        
        if (a.IsInGroup("projectiles") && b.HasTag("player")) {
            OnProjectileHitPlayer(b, a);
        }

        if (b.IsInGroup("projectiles") && a.HasTag("player")) {
            OnProjectileHitPlayer(a, b);
        }

        if (a.IsInGroup("projectiles") && b.IsInGroup("enemy")) {
            OnProjectileHitEnemy(b, a);
        }

        if (b.IsInGroup("projectiles") && a.IsInGroup("enemy")) {
            OnProjectileHitEnemy(a, b);
        }
    }


    void OnProjectileHitPlayer(Entity player, Entity projectile) {
        Projectile projectileComponent = projectile.GetComponent<Projectile>();

        if (!projectileComponent.friendly) {
          auto& health = player.GetComponent<Health>();
          health.healthPercentage -= projectileComponent.hitPercentageDmg;

          if(health.healthPercentage <= 0)
            player.Kill();
        }

        projectile.Kill();
    }

    void OnProjectileHitEnemy(Entity enemy, Entity projectile) {
        Projectile projectileComponent = projectile.GetComponent<Projectile>();

        if (projectileComponent.friendly) {
          auto& health = enemy.GetComponent<Health>();
          health.healthPercentage -= projectileComponent.hitPercentageDmg;

          if(health.healthPercentage <= 0)
            enemy.Kill();
        }

        projectile.Kill();
    }
    void Update() {
    }
};
