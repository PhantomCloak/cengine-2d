#include "scene_list.h"
#include "imgui.h"
#include "../scene/scene.h"

std::function<void(flecs::entity)> selectCallback;


void SceneList::SetSelectCallback(std::function<void(flecs::entity)> callback){
    selectCallback = callback;
}

void SceneList::RenderWindow() {
    static std::unordered_map<flecs::entity_t, bool> selectableEntityList;

    if (ImGui::Begin("Scene List")) {
        for (auto entity : Scene::GetEntities()) {
            std::string txt = entity.name().c_str() + std::string(" : ") + std::to_string(entity.id());
            bool isSelected = selectableEntityList[entity.id()];

            if (ImGui::Selectable(txt.c_str(), &isSelected)) {
                for (auto& pair : selectableEntityList) {
                    pair.second = false;
                }

                selectableEntityList[entity.id()] = true;
                selectCallback(entity);
            }

            // Right click context menu for deleting
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Delete")) {
                  Scene::DestroyEntity(entity);
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}

