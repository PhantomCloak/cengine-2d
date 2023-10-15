#include "system_explorer.h"
#include "../scene/scene.h"
#include "flecs.h"
#include "imgui.h"
#include <string>
#include <unordered_map>

bool SystemExplorer::IsFocused() {
    return ImGui::IsWindowFocused();
}

void SystemExplorer::RenderWindow() {
    static std::unordered_map<int, bool> isOnOff;
    static bool s = false;
    if (ImGui::Begin("SystemExplorer", &s)) {
        ImGui::BeginGroupPanel("Monitor");

        ImGui::EndGroupPanel();
    }
    ImGui::End();
}
