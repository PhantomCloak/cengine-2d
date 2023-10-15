
#include "../assetmgr/AssetManager.h"
#include "../io/cursor.h"
#include "../io/keyboard.h"
#include "../libs/imgui/imgui.h"
#include "../scene/scene.h"
#include "editor_style.h"
#include "editor_utils.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "nfd.h"
#include "systems/editor_systems.h"

#if RENDER_BACKEND_RAYLIB
#include "raylib.h"
#include "rlImGui.h"
#else
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

std::shared_ptr<Editor> Editor::Instance;

void Editor::Init(CommancheRenderer* renderer) {
    Log::Inf("Editor booting...");

    Instance = std::shared_ptr<Editor>(this);

    entityInspector = std::make_shared<EntityInspector>();
    logView = std::make_shared<LogView>();
    importer = std::make_shared<TileMapImporter>();
    tilePlacer = std::make_shared<TilePlacer>();
    viewport = std::make_shared<EditorViewPort>();
    sceneList = std::make_shared<SceneList>();
    menuBar = std::make_shared<EditorMenuBar>(Instance);

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

    AssetManager::AddTexture("axis", "./assets/editor/axis.png");

    Log::Inf("Editor started");

    EditorSystems::Init(Scene::ecs, this);
}

void FileMenu() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save Map")) {
            windowFlags[EDITOR_SHOW_SAVE_DIALOG] = true;
        }
        if (ImGui::MenuItem("Load Map")) {
            windowFlags[EDITOR_SHOW_LOAD_DIALOG] = true;
        }

        if (ImGui::MenuItem("System Explorer")) {
            windowFlags[EDITOR_SYSTEM_EXPLORER_DIALOG] = true;
        }
        ImGui::EndMenu();
    }
}


void EntitiesMenu() {
    if (ImGui::BeginMenu("Entity")) {
        if (ImGui::MenuItem("New")) {
            static long NextEntityId = 0;
            NextEntityId = rand();
            flecs::entity e = Scene::CreateEntity("new entity" + std::to_string(NextEntityId++));
            e.add<RectTransform>();
        }
        ImGui::EndMenu();
    }
}

void SaveDialog() {
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

void RebuildLog() {
    std::ostringstream combined;
    for (const auto& log : filteredLogs) {
        combined << log << "\n";
    }
    combinedLog = combined.str();
}

void FilterLogs() {
    if (strlen(searchBuffer) == 0) {
        // If search buffer is empty, show all logs
        filteredLogs = logs;
    } else {
        filteredLogs.clear();
        std::string searchStr(searchBuffer);
        for (const auto& log : logs) {
            if (log.find(searchStr) != std::string::npos) {
                filteredLogs.push_back(log);
            }
        }
    }
    RebuildLog();
}

void Draw() {
    static bool s = true;
    if (ImGui::Begin("Log Viewer", &s)) {
        ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer));

        FilterLogs();

        // Start a child region with a specific size and make it scrollable
        ImGui::BeginChild("LogRegion", ImVec2(0, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_HorizontalScrollbar);

        // ReadOnly Text Container with Scrolling
        ImGui::InputTextMultiline("##logs", &combinedLog[0], combinedLog.size(), ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);

        ImGui::EndChild(); // End the child region
    }
    ImGui::End();
}


void LoadDialog() {
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


void renderDockingSpace() {
    static bool opt_fullscreen = true;
    static ImGuiWindowFlags window_flags = 0;
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


    if (menuBar->enabledWindows[EDITOR_SHOW_MAP_EDITOR]) {
        tilePlacer->RenderWindow();
    }

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
