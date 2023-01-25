#include "editor.h"
#include "../assetmgr/AssetManager.h"
#include "../components/Sprite.h"
#include "../components/Transform.h"
#include "../ecs/world.h"
#include "../game/game.h"
#include "../game/map_serializer.h"
#include "../libs/imgui/imgui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <functional>
#include <memory>
#include <stdio.h>

#define EDITOR_SHOW_MAP_EDITOR 1
#define EDITOR_SHOW_SAVE_DIALOG 2
#define EDITOR_SHOW_LOAD_DIALOG 3

std::unordered_map<int, bool> windowFlags;
std::shared_ptr<World> worldRef;
CommancheCamera* cam;
EngineSerializer* serializer;
static CommancheRenderer* renderer;
Map* mp;
bool tileSetIsInit = false;

static ImGuiIO io;
static int zIndexStart = 800;

void Editor::Init(CommancheRenderer* renderer1, Map* map, std::shared_ptr<World> world, CommancheCamera* camera, EventBus* bus) {
    worldRef = world;
    cam = camera;
    mp = map;

    MapLuaSerializer ser;
    std::vector<std::string> a;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;


    ImGui::StyleColorsDark();

    // ImGui_ImplSDL3_InitForOpenGL((SDL_Window*)renderer->wnd, renderer->gctx);
    // ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)renderer1->wnd, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // bus->SubscribeEvent(this, &Editor::onMousePressed);
    serializer = new EngineSerializer(world);
    this->world = world;
    renderer = renderer1;
}

Transform* draggableTransform;
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
        serializer->SerializeWorldToFile("./assets/maps/" + std::string(mapNameBuff) + ".json");
        windowFlags[EDITOR_SHOW_SAVE_DIALOG] = false;
    }
    ImGui::End();
}

void LoadDialog() {
    ImGui::Begin("Load Menu");
    static char* mapNameBuff = (char*)malloc(128);
    ImGui::InputText("Map Name: ", mapNameBuff, 128);
    ImGui::Spacing();
    ImGui::Spacing();
    if (ImGui::Button("OK")) {
        serializer->DeserializeFileToWorld("./assets/maps/" + std::string(mapNameBuff) + ".json");
        windowFlags[EDITOR_SHOW_LOAD_DIALOG] = false;
    }
    ImGui::End();
}

void MapEditor() {
    const int gridSize = 9;
    int currentGrid = 0;
    ImGui::Begin("Tile Editor");

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


    for (int i = 0; i < totalScanArea; i++) {
        glm::vec2 start;
        glm::vec2 end;
        mp->TileIndexToTextCoord(std::string(selectedItem), i, start, end);
        auto identifier = std::to_string(i);

        if (ImGui::ImageButton(identifier.c_str(),
            (void*)(intptr_t)selectedTextureId,
            ImVec2(64, 64),
            ImVec2(pixelCordToUvX2(start.x, selectedMafInf.width),
            pixelCordToUvY2(start.y, selectedMafInf.height)),
            ImVec2(pixelCordToUvX2(start.x + tileSize, selectedMafInf.width),
            pixelCordToUvY2(start.y + tileSize, selectedMafInf.height)))) {


            Entity piece = worldRef->CreateEntity();
            piece.Group("tiles");

            piece.AddComponent<Sprite>(selectedTextureId, zIndexStart, start.x, start.y);
            piece.AddComponent<Serializable>();
            piece.AddComponent<Transform>(glm::vec2(100, 100), glm::vec2(4, 4), 0);
            piece.AddComponent<MapTile>(i);

            if (collider) {
                piece.AddComponent<BoxCollider>(16, 16);
            }

            draggableTransform = &piece.GetComponent<Transform>();
            zIndexStart++;
        }
        currentGrid++;
        if (currentGrid % gridSize != 0)
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
void InterpolateToGrid(glm::vec2* vec, int gridSize) {
    vec->x = floor(vec->x / gridSize) * gridSize;
    vec->y = floor(vec->y / gridSize) * gridSize;
}

// void Editor::ProcessInput(SDL_Event* event) {
//     ImGui_ImplSDL2_ProcessEvent(event);
// }

ImGuiWindowFlags window_flags = 0;

void Editor::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    
        if (ImGui::BeginMainMenuBar()) {
            FileMenu();
            EntitiesMenu();
            AssetsMenu();
            ImGui::EndMainMenuBar();
        }
    
        if (windowFlags[EDITOR_SHOW_MAP_EDITOR]) {
            MapEditor();
        }
        if (windowFlags[EDITOR_SHOW_SAVE_DIALOG]) {
            SaveDialog();
        }
        if (windowFlags[EDITOR_SHOW_LOAD_DIALOG]) {
            LoadDialog();
        }
    
        if (draggableTransform != nullptr) {
            auto pos = glm::vec2(0);
            pos.x += cam->x;
            pos.y += cam->y;
    
            InterpolateToGrid(&pos, 32);
            draggableTransform->pos = pos;
        }
    
    static ImVec2 size;
    if (ImGui::Begin("Scene List")) {}
    ImGui::End();
    if (ImGui::Begin("Files")) {}
    ImGui::End();
    if (ImGui::Begin("Viewport")) {

        if (size.x != ImGui::GetWindowWidth() || size.y != ImGui::GetWindowHeight()) {

            size.x = ImGui::GetWindowWidth();
            size.y = ImGui::GetWindowHeight();
        }
        renderer->SetFrameSize(size.x, size.y);

        renderer->Render1();
        world->GetSystem<RenderSystem>().Update();
        renderer->Render2();
        // renderer->Render();
        auto tex = renderer->GetFrame();


        ImVec2 uv0 = {0, 1};
        ImVec2 uv1 = {1, 0};
        ImGui::Image((void*)tex, size, uv0, uv1);

        //ImGui::Image((void*)tex, size);
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    renderer->Render();
}
