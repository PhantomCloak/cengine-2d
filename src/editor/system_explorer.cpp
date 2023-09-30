#include "system_explorer.h"
#include "../scene/scene.h"
#include "imgui.h"
#include <string>a


static std::string _labelPrefix(const char* const label) {
    float width = ImGui::CalcItemWidth();

    float x = ImGui::GetCursorPosX();
    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetNextItemWidth(-1);

    std::string labelID = "##";
    labelID += label;

    return labelID;
}

void SystemExplorer::RenderWindow() {
    static std::unordered_map<int, bool> isOnOff;
    static bool s = false;
    if (ImGui::Begin("SystemExplorer", &s)) {
        ImGui::BeginGroupPanel("Monitor");
        // for (auto& systemEntry : Scene::world->storage->systems) {
        //     ImGui::Checkbox(_labelPrefix(systemEntry.first.name()).c_str(), &systemEntry.second->isActive);
        // }
        ImGui::EndGroupPanel();
    }
    ImGui::End();
}
