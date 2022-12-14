#ifndef RENDER_2D
#define RENDER_2D

#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../render/render.h"
#include "../ecs/system.h"
#include "../log/log.h"
#include "SDL_opengl.h"
#include <SDL.h>

struct RenderObject {
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Sprite> sprite;
};

class RenderSystem : public System {
    private:
    SDL_Rect* camera;
    CommancheRenderer* renderer;

    public:
    RenderSystem(CommancheRenderer* renderer, SDL_Rect* camera) {
        this->camera = camera;
        this->renderer = renderer;
        RequireComponent<Transform>();
        RequireComponent<Sprite>();
    };

    void Update() {
        std::vector<RenderObject> renderSortList;

        glClear(GL_COLOR_BUFFER_BIT);
        // get rende objs
        for (auto entity : GetEntities()) {
            auto& transform = entity.GetComponent<Transform>();
            auto sprite = entity.GetComponent<Sprite>();


            RenderObject renderObj = {
                .sprite = std::make_unique<Sprite>(sprite),
                .transform = std::make_unique<Transform>(transform)
            };

            renderSortList.push_back(renderObj);
        }

        // sort by z-index
        std::sort(renderSortList.begin(), renderSortList.end(), [](const RenderObject& a, const RenderObject& b) {
            return a.sprite->zIndex < b.sprite->zIndex;
        });

        for (RenderObject& renderObj : renderSortList) {

            SDL_Rect dstRect = {
                .x = static_cast<int>(renderObj.transform->pos.x - camera->x),
                .y = static_cast<int>(renderObj.transform->pos.y - camera->y),
                .w = (int)(renderObj.sprite->width * renderObj.transform->scale.x),
                .h = (int)(renderObj.sprite->height * renderObj.transform->scale.y)
            };

            SDL_Rect offsetRect = renderObj.sprite->srcRect;
            renderer->DrawImage(renderObj.sprite->texture, dstRect.x, dstRect.y, dstRect.w, dstRect.h, offsetRect.w, offsetRect.x, offsetRect.y);
        }
    };
};

#endif
