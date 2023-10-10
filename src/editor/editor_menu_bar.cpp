#include "editor_menu_bar.h"
#include "../ecs/serializer.h"
#include "../scene/scene.h"
#include "../io/filesystem.h"
#include "flecs.h"
#include "imgui.h"

#define EDITOR_SHOW_MAP_EDITOR 1
#define EDITOR_SHOW_SAVE_DIALOG 2
#define EDITOR_SHOW_LOAD_DIALOG 3
#define EDITOR_SYSTEM_EXPLORER_DIALOG 4


std::unordered_map<int, bool> windowFlags;

void EditorMenuBar::LoadDialog() {
    ImGui::Begin("Load Menu");

    static std::string loc = "./assets/maps";
    static int ctx;
    std::vector<std::string> names = FileSys::GetFilesInDirectory(loc);

    std::vector<const char*> cstrNames;
    for (const auto& name : names) {
        cstrNames.push_back(name.c_str());
    }

    // Display the combo box
    ImGui::Combo(_labelPrefix("Selected Map: ").c_str(), &ctx, cstrNames.data(), cstrNames.size());
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    if (ImGui::Button("OK")) {
        EngineSerializer::DeserializeFileToScene(names[ctx], Scene::ecs);
        windowFlags[EDITOR_SHOW_LOAD_DIALOG] = false;
    }
    ImGui::End();
}

void EditorMenuBar::SaveDialog() {
    ImGui::Begin("Save Menu");
    static char* mapNameBuff = (char*)malloc(128);
    ImGui::Text("Map Name: ");
    ImGui::SameLine();
    ImGui::InputText("##", mapNameBuff, 128);
    ImGui::Spacing();
    ImGui::Spacing();
    if (ImGui::Button("OK")) {
        EngineSerializer::SerializeSceneToFile("./assets/maps/" + std::string(mapNameBuff) + ".json", Scene::ecs);
        windowFlags[EDITOR_SHOW_SAVE_DIALOG] = false;
    }
    ImGui::End();
}

void EditorMenuBar::FileMenu() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save Map")) {
            windowFlags[EDITOR_SHOW_SAVE_DIALOG] = true;
        }
        if (ImGui::MenuItem("Load Map")) {
            windowFlags[EDITOR_SHOW_LOAD_DIALOG] = true;
        }

        ImGui::EndMenu();
    }
}

void EditorMenuBar::EntitiesMenu() {
    if (ImGui::BeginMenu("Entity")) {
        if (ImGui::MenuItem("New")) {
            flecs::entity e = Scene::CreateEntity("chewbacca");
            e.add<RectTransformC>();
            Log::Inf("CHEWBACCA CALLED");
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Scripting")) {
        ImGui::EndMenu();
    }
}

void EditorMenuBar::AssetsMenu() {
    static bool showImporter = false;

    if (ImGui::BeginMenu("Assets")) {
        if (ImGui::MenuItem("Map Editor")) {
            windowFlags[EDITOR_SHOW_MAP_EDITOR] = true;
        }
        if (ImGui::MenuItem("Refresh Asset Database")) {
            // windowFlags[EDITOR_SHOW_MAP_EDITOR] = true;
        }
        if (ImGui::MenuItem("Import Asset")) {
            // Editor::Instance->importer->OpenImporter([](int textureId) {
            //     showImporter = false;
            // });
            showImporter = true;
        }
        ImGui::EndMenu();
    }

    if (showImporter) {
        // Editor::Instance->importer->RenderWindow();
    }
}

void EditorMenuBar::RenderWindow() {

    if (ImGui::BeginMainMenuBar()) {
        FileMenu();
        EntitiesMenu();
        AssetsMenu();

        ImGui::SameLine(ImGui::GetWindowWidth() - 400);
        if (true) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("Game Running: Yes");
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("Game Running: No");
            ImGui::PopStyleColor();
        }

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(10.0f, 0.0f));

        ImGui::SameLine();
        if (false) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("Physics Running: Yes");
            ImGui::PopStyleColor();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("Physics Running: No");
            ImGui::PopStyleColor();
        }

        ImGui::SameLine();
        ImGui::Dummy(ImVec2(10.0f, 0.0f));

        float fps = GetFPS();
        ImGui::SameLine();
        ImGui::Text("FPS: %.1f", fps);
    }
    ImGui::EndMainMenuBar();
}
