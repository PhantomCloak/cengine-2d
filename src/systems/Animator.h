#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../components/Animation.h"
#include "../components/Sprite.h"
#include "../ecs/system.h"

class Animator : public System {
    public:
    Animator() {
        RequireComponent<Sprite>();
        RequireComponent<Animation>();
    };

    void Update() {
        for (auto entity : GetEntities()) {
          auto& animationComponent = entity.GetComponent<Animation>();
          auto& spriteComponent = entity.GetComponent<Sprite>();

          animationComponent.currentFrame = (int)((SDL_GetTicks() - animationComponent.startTime) * animationComponent.frameSpeed / 1000.0) % animationComponent.numFrames;
          spriteComponent.srcRect.x = animationComponent.currentFrame * spriteComponent.width;
        }
    }
};
#endif
