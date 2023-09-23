#include "editor.h"
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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <filesystem>
#include <functional>
#include <memory>
#include <stdio.h>


#define EDITOR_SHOW_MAP_EDITOR 1
#define EDITOR_SHOW_SAVE_DIALOG 2
#define EDITOR_SHOW_LOAD_DIALOG 3
#define EDITOR_SYSTEM_EXPLORER_DIALOG 4

std::unordered_map<int, bool> windowFlags;
bool tileSetIsInit = false;

static int zIndexStart = 800;
glm::vec2 Editor::ScreenSize;

void Editor::Init(CommancheRenderer* renderer) {
    MapLuaSerializer ser;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)renderer->wnd, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    this->renderer = renderer;

    AssetManager::AddTexture("folder", "./assets/editor/folder-icon.png");
    AssetManager::AddTexture("file", "./assets/editor/file-icon.png");

    fileView = new FileView();
    explorer = new SystemExplorer();
    entityInspector = new EntityInspector();
}

RectTransform* draggableTransform;
Entity* draggableEntity;

void Editor::onMousePressed(MousePressedEvent& event) {
    if (draggableTransform != nullptr) {
        draggableTransform = nullptr;
    }
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
        }
        ImGui::EndMenu();
    }
}


float pixelCordToUvY2(float y, int height) {
    return y / height;
}

float pixelCordToUvX2(float x, int width) {
    return x / width;
}

void SelectMapTile(std::string mapName) {
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
        EngineSerializer::SerializeSceneToFile("./assets/maps/" + std::string(mapNameBuff) + ".json");
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
        EngineSerializer::DeserializeFileToScene(names[ctx]);
        windowFlags[EDITOR_SHOW_LOAD_DIALOG] = false;
    }
    ImGui::End();
}

void Editor::Keybindings() {
    if (Keyboard::IsKeyPressing(KeyCode::Key_RArrow)) {
        renderer->OffsetCamera(25, 0);
    }

    if (Keyboard::IsKeyPressing(KeyCode::Key_LArrow)) {
        renderer->OffsetCamera(-25, 0);
    }

    if (Keyboard::IsKeyPressing(KeyCode::Key_UArrow)) {
        renderer->OffsetCamera(0, -25);
    }
    if (Keyboard::IsKeyPressing(KeyCode::Key_DArrow)) {
        renderer->OffsetCamera(0, 25);
    }
}

void MapEditor() {
    static bool isOpen = false;
    ImGui::Begin("Tile Editor", &isOpen);

    static const std::vector<std::string>& loadedAssets = AssetManager::GetLoadedTextures();
    static const char* selectedItem = loadedAssets[0].c_str();
    static TextureInf selectedMafInf = AssetManager::GetTextureInf(selectedItem);
    static int selectedTextureId = AssetManager::GetTexture(selectedItem);

    ImGui::Text("Drag & drop textures");
    ImGui::Text("atlas size = %d x %d", selectedMafInf.width, selectedMafInf.height);
    ImGui::Text("grid size = %d x %d", 64, 64);

    static bool collider = false;

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::BeginCombo("combo", selectedItem)) {

        for (int i = 0; i < AssetManager::GetLoadedTextures().size(); ++i) {
            bool isSelected = (selectedItem == loadedAssets[i].c_str());
            if (ImGui::Selectable(loadedAssets[i].c_str(), isSelected)) {
                selectedItem = loadedAssets[i].c_str();
                selectedTextureId = AssetManager::GetTexture(std::string(selectedItem));
                selectedMafInf = AssetManager::GetTextureInf(std::string(selectedItem));
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Checkbox("Collider   ", &collider);
    ImGui::Spacing();
    ImGui::Spacing();
    int tileSize = 16;

    int columns = selectedMafInf.width / tileSize;
    int rows = selectedMafInf.height / tileSize;
    int totalScanArea = columns * rows;


    ImVec2 availRegion = ImGui::GetContentRegionAvail();
    int slice = availRegion.x / 64;

    for (int i = 0; i < totalScanArea; i++) {
        auto identifier = std::to_string(i);

        if ((i % slice) == 0) {
            ImGui::NewLine();
        }

        int currentColumn = i % columns;
        int currentRow = i / columns;

        ImVec2 uv0 = ImVec2(pixelCordToUvX2(currentColumn * tileSize, selectedMafInf.width), pixelCordToUvY2(currentRow * tileSize, selectedMafInf.height));
        ImVec2 uv1 = ImVec2(pixelCordToUvX2((currentColumn + 1) * tileSize, selectedMafInf.width), pixelCordToUvY2((currentRow + 1) * tileSize, selectedMafInf.height));


        if (ImGui::ImageButton(identifier.c_str(), (void*)(intptr_t)selectedTextureId, ImVec2(64, 64), uv0, uv1)) {
            Entity piece = Scene::CreateEntity();
            piece.Group("tiles");

            piece.AddComponent<Sprite>(selectedTextureId, zIndexStart, currentColumn * tileSize, currentRow * tileSize);
            piece.AddComponent<RectTransform>(glm::vec2(0, 0), glm::vec2(50, 50));

            if (collider) {
                piece.AddComponent<BoxCollider>(16, 16);
            }

            draggableTransform = &piece.GetComponent<RectTransform>();
            zIndexStart++;
        }
        // currentGrid++;
        ImGui::SameLine();
    }

    ImGui::End();
}

void AssetsMenu() {
    if (ImGui::BeginMenu("Assets")) {
        if (ImGui::MenuItem("Map Editor")) {
            windowFlags[EDITOR_SHOW_MAP_EDITOR] = true;
        }
        ImGui::EndMenu();
    }
}


void SerializeEntity(Entity e) {
}

static Entity selectedEntityId = NULL;
void Properties() {
}

void SceneList() {
    static bool selectableEntityList[1024];
    if (ImGui::Begin("Scene List")) {

        for (auto entity : Scene::GetEntities()) {
            std::string txt = "Entity: " + std::to_string(entity.GetId());

            if (ImGui::Selectable(txt.c_str(), &selectableEntityList[entity.GetId()])) {
                for (int i = 0; i < sizeof(selectableEntityList); i++)
                    selectableEntityList[i] = false;

                selectableEntityList[entity.GetId()] = true;
                selectedEntityId = entity;
            }
        }
        ImGui::End();
    }
}


void InterpolateToGrid(glm::vec2* vec, int gridSize) {
    vec->x = floor(vec->x / gridSize) * gridSize;
    vec->y = floor(vec->y / gridSize) * gridSize;
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

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();
}

void Editor::Render() {
    Keybindings();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderDockingSpace();

    if (windowFlags[EDITOR_SHOW_MAP_EDITOR]) {
        MapEditor();
    }
    if (windowFlags[EDITOR_SHOW_SAVE_DIALOG]) {
        SaveDialog();
    }
    if (windowFlags[EDITOR_SHOW_LOAD_DIALOG]) {
        LoadDialog();
    }
    if (windowFlags[EDITOR_SYSTEM_EXPLORER_DIALOG]) {
        explorer->RenderWindow();
    }

    static bool tff = true;

    if (Keyboard::IsKeyPressing(KeyCode::Key_U) && tff) {
        tff = false;
        EngineSerializer::DeserializeFileToScene("./assets/maps/brew.json");
    }

    if (ImGui::BeginMainMenuBar()) {
        FileMenu();
        EntitiesMenu();
        AssetsMenu();
        ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin("Properties")) {
        if (selectedEntityId != NULL) {
            EntityInspector::SetEntity(selectedEntityId);
            entityInspector->RenderWindow();
        }
        ImGui::End();
    }

    SceneList();
    fileView->RenderWindow();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

        if (ScreenSize.x != ImGui::GetWindowWidth() || ScreenSize.y != ImGui::GetWindowHeight()) {
            ScreenSize.x = ImGui::GetWindowWidth();
            ScreenSize.y = ImGui::GetWindowHeight();
            renderer->SetFrameSize(ScreenSize.x, ScreenSize.y);
        }

        auto tex = renderer->GetFrame();

        static ImVec2 uv0 = { 0, 1 };
        static ImVec2 uv1 = { 1, 0 };
        ImVec2 avail = ImGui::GetContentRegionAvail();


        ImGui::Image((void*)tex, avail, uv0, uv1);

        if (draggableTransform != nullptr) {
            auto mPos = ImGui::GetMousePos();

            ImVec2 viewportPos = ImGui::GetMainViewport()->WorkPos; glm::vec2 local_mouse_pos(mPos.x - viewportPos.x, mPos.y - viewportPos.y);

            glm::vec2 worldPos = Cursor::GetCursorWorldPosition(local_mouse_pos);
            float gridSize = 25.0f; // define the grid size
            worldPos.x = std::round(worldPos.x / gridSize) * gridSize;
            worldPos.y = std::round(worldPos.y / gridSize) * gridSize;

            draggableTransform->pos = worldPos;
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
