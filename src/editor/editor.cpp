
#include "../assetmgr/AssetManager.h"
#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../ecs/serializer.h"
#include "../ecs/world.h"
#include "../game/game.h"
#include "../game/map_serializer.h"
#include "../io/cursor.h"
#include "../io/filesystem.h"
#include "../libs/imgui/imgui.h"
#include "../scene/scene.h"
#include "editor_style.h"
#include "editor_utils.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "nfd.h"
#include "systems/editor_systems.h"
#include "tilemap_importer.h"
#include <filesystem>
#include <functional>
#include <memory>
#include <stdio.h>
#include <unordered_map>

#if RENDER_BACKEND_RAYLIB
#include "raylib.h"
#include "rlImGui.h"
#endif

std::unique_ptr<Editor> Editor::Instance;

void Editor::Init(CommancheRenderer* renderer) {
    Log::Inf("Editor booting...");

    Instance = std::unique_ptr<Editor>(this);

    entityInspector = std::make_unique<EntityInspector>();
    logView = std::make_unique<LogView>();
    importer = std::make_unique<TileMapImporter>();
    tilePlacer = std::make_unique<TilePlacer>();
    viewport = std::make_unique<EditorViewPort>();
    sceneList = std::make_unique<SceneList>();
    menuBar = std::make_unique<EditorMenuBar>();

    sceneList->SetSelectCallback([this](const flecs::entity entity) {
        entityInspector->SetEntity(entity);
    });

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

#if RENDER_BACKEND_OPENGL
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)renderer->wnd, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
#endif

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    EditorStyle::Init();

    Log::Inf("Editor started");

    EditorSystems::Init(Scene::ecs, this);
}




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



void Editor::Keybindings() {
    if (Keyboard::IsKeyPressing(KeyCode::Key_RArrow)) {
        CommancheRenderer::Instance->OffsetCamera(25, 0);
    }

    if (Keyboard::IsKeyPressing(KeyCode::Key_LArrow)) {
        CommancheRenderer::Instance->OffsetCamera(-25, 0);
    }

    if (Keyboard::IsKeyPressing(KeyCode::Key_UArrow)) {
        CommancheRenderer::Instance->OffsetCamera(0, -25);
    }
    if (Keyboard::IsKeyPressing(KeyCode::Key_DArrow)) {
        CommancheRenderer::Instance->OffsetCamera(0, 25);
    }
}

ImGuiWindowFlags window_flags = 0;

void renderDockingSpace() {
    static bool opt_fullscreen = true;
    static bool p_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

glm::vec2 Editor::GetCursorPosition() {
    return Instance->viewport->ViewportCursorPos;
}

glm::vec2 Editor::GetViewPortSize() {
    return Instance->viewport->ViewportSize;
}

void Editor::Render() {
    Keybindings();

#if RENDER_BACKEND_OPENGL
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
#endif

#if RENDER_BACKEND_RAYLIB
    rlImGuiBegin();
#endif

    menuBar->RenderWindow();
    renderDockingSpace();


    //if (windowFlags[EDITOR_SHOW_MAP_EDITOR]) {
    //    tilePlacer->RenderWindow();
    //}
    //if (windowFlags[EDITOR_SHOW_SAVE_DIALOG]) {
    //    SaveDialog();
    //}
    //if (windowFlags[EDITOR_SHOW_LOAD_DIALOG]) {
    //    LoadDialog();
    //}

    sceneList->RenderWindow();
    entityInspector->RenderWindow();
    logView->RenderWindow();

    viewport->RenderWindow();

#if RENDER_BACKEND_RAYLIB
    rlImGuiEnd();
#endif
#if RENDER_BACKEND_OPENGL
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#endif
}
