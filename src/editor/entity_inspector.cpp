#include "entity_inspector.h"
#include "../game/components.h"
#include "../scene/scene.h"
#include "imgui.h"

flecs::entity EntityInspector::e;

void EntityInspector::SetEntity(flecs::entity entity) {
    e = entity;
}

std::string _labelPrefix(const char* const label) {
    float width = ImGui::CalcItemWidth();

    float x = ImGui::GetCursorPosX();
    ImGui::Text("%s", label);
    ImGui::SameLine();
    ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetNextItemWidth(-1);

    std::string labelID = "##";
    labelID += label;

    return labelID;
}

void EntityInspector::RenderWindow() {
    if (e != 0 && e.has<RectTransform>()) {
        flecs::ref<RectTransform> comp = e.get_ref<RectTransform>();

        ImGui::BeginGroupPanel("Transform");
        ImGui::InputFloat(_labelPrefix("pos x:").c_str(), &comp->pos.x, 1);
        ImGui::InputFloat(_labelPrefix("pos y:").c_str(), &comp->pos.y, 1);

        ImGui::Spacing();

        ImGui::InputFloat(_labelPrefix("width:").c_str(), &comp->size.x, 1);
        ImGui::InputFloat(_labelPrefix("height:").c_str(), &comp->size.y, 1);

        ImGui::Spacing();

        ImGui::InputFloat(_labelPrefix("scale x:").c_str(), &comp->scale.x, 1);
        ImGui::InputFloat(_labelPrefix("scale y:").c_str(), &comp->scale.y, 1);

        ImGui::Spacing();

        ImGui::InputFloat(_labelPrefix("rotation: ").c_str(), &comp->rotation, 1);

        ImGui::EndGroupPanel();
    }
    //if (e.has<RigidBody>()) {
    //    //auto& comp = e.GetComponent<RigidBody>();
    //    //ImGui::BeginGroupPanel("RigidBody");
    //    //ImGui::InputFloat(_labelPrefix("velocity x:").c_str(), &comp.velocityLinear.x);
    //    //ImGui::InputFloat(_labelPrefix("velocity y:").c_str(), &comp.velocityLinear.y);
    //    //ImGui::InputFloat(_labelPrefix("mass:").c_str(), &comp.mass);
    //    //ImGui::InputFloat(_labelPrefix("restution:").c_str(), &comp.restution);
    //    //ImGui::Checkbox(_labelPrefix("is static:").c_str(), &comp.isStatic);
    //    //ImGui::Checkbox(_labelPrefix("is fixed rot:").c_str(), &comp.isFixedRot);
    //    //ImGui::EndGroupPanel();
    //}
    //if (e.has<Sprite>()) {
    //    //auto comp = e.GetComponent<Sprite>();
    //    //ImGui::BeginGroupPanel("Sprite");
    //    //ImGui::InputInt(_labelPrefix("texture id:").c_str(), &comp.texture);
    //    //ImGui::InputInt(_labelPrefix("z index:").c_str(), &comp.zIndex);
    //    //ImGui::InputFloat(_labelPrefix("src x:").c_str(), &comp.srcRect.x);
    //    //ImGui::InputFloat(_labelPrefix("src y:").c_str(), &comp.srcRect.y);
    //    //ImGui::EndGroupPanel();
    //}
    //if (e.has<CharacterController>()) {
    //    //auto comp = e.GetComponent<CharacterController>();
    //    //ImGui::BeginGroupPanel("Character Controller");
    //    //ImGui::Checkbox(_labelPrefix("left").c_str(), &comp.left);
    //    //ImGui::Checkbox(_labelPrefix("right").c_str(), &comp.right);
    //    //ImGui::Checkbox(_labelPrefix("up").c_str(), &comp.up);
    //    //ImGui::EndGroupPanel();
    //}
    //if (e.has<Health>()) {
    //    //auto& comp = e.GetComponent<Health>();
    //    //ImGui::BeginGroupPanel("Health");
    //    //ImGui::InputInt(_labelPrefix("Health").c_str(), &comp.healthPercentage);
    //    //ImGui::EndGroupPanel();
    //}

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("SC");
    }

    if (ImGui::BeginPopup("SC")) {

        if (ImGui::MenuItem("RigidBody")) {
            Scene::AddComponentToEntity<RigidBody>(e);
            // Code to add Component 1
        }

        if (ImGui::MenuItem("Box Collider")) {
            // Code to add Component 2
        }

        if (ImGui::MenuItem("Sprite")) {
            // code to add sprite
        }
        ImGui::EndPopup();
    }
}
