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
#include <filesystem>
#include <functional>
#include <memory>
#include <stdio.h>
namespace fs = std::filesystem;


#define EDITOR_SHOW_MAP_EDITOR 1
#define EDITOR_SHOW_SAVE_DIALOG 2
#define EDITOR_SHOW_LOAD_DIALOG 3

std::unordered_map<int, bool> windowFlags;
std::shared_ptr<World> worldRef;
CommancheCamera* cam;
EngineSerializer* serializer;
Map* mp;
bool tileSetIsInit = false;

static int zIndexStart = 800;

void Editor::Init(CommancheRenderer* renderer, Map* map, std::shared_ptr<World> world, CommancheCamera* camera, EventBus* bus) {
    worldRef = world;
    cam = camera;
    mp = map;

    MapLuaSerializer ser;
    std::vector<std::string> a;
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

    serializer = new EngineSerializer(world);
    this->world = world;
    this->renderer = renderer;

    AssetManager::AddTexture("folder", "./assets/editor/folder-icon.png");
    AssetManager::AddTexture("file", "./assets/editor/file-icon.png");
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


void SerializeEntity(Entity e) {
    if (e.HasComponent<RectTransform>()) {
        auto comp = e.GetComponent<RectTransform>();
        ImGui::BeginGroupPanel("Transform");
        ImGui::InputFloat(_labelPrefix("pos x:").c_str(), &comp.pos.x);
        ImGui::InputFloat(_labelPrefix("pos y:").c_str(), &comp.pos.y);

        ImGui::Spacing();

        ImGui::InputFloat(_labelPrefix("width:").c_str(), &comp.size.x);
        ImGui::InputFloat(_labelPrefix("height:").c_str(), &comp.size.y);

        ImGui::Spacing();

        ImGui::InputFloat(_labelPrefix("scale x:").c_str(), &comp.scale.x);
        ImGui::InputFloat(_labelPrefix("scale y:").c_str(), &comp.scale.y);

        ImGui::EndGroupPanel();
    }
    if (e.HasComponent<RigidBody>()) {
        auto comp = e.GetComponent<RigidBody>();
        ImGui::BeginGroupPanel("RigidBody");
        ImGui::InputFloat(_labelPrefix("velocity x:").c_str(), &comp.velocity.x);
        ImGui::InputFloat(_labelPrefix("velocity y:").c_str(), &comp.velocity.y);
        ImGui::InputFloat(_labelPrefix("restution:").c_str(), &comp.restution);
        ImGui::Checkbox(_labelPrefix("is static:").c_str(), &comp.isStatic);
        ImGui::EndGroupPanel();
    }
    if (e.HasComponent<Sprite>()) {
        auto comp = e.GetComponent<Sprite>();
        ImGui::BeginGroupPanel("Sprite");
        ImGui::InputInt(_labelPrefix("texture id:").c_str(), &comp.texture);
        ImGui::InputInt(_labelPrefix("z index:").c_str(), &comp.zIndex);
        ImGui::InputFloat(_labelPrefix("src x:").c_str(), &comp.srcRect.x);
        ImGui::InputFloat(_labelPrefix("src y:").c_str(), &comp.srcRect.y);
        ImGui::EndGroupPanel();
    }
    if (e.HasComponent<CharacterController>()) {
        auto comp = e.GetComponent<CharacterController>();
        ImGui::BeginGroupPanel("Character Controller");
        ImGui::Checkbox(_labelPrefix("left").c_str(), &comp.left);
        ImGui::Checkbox(_labelPrefix("right").c_str(), &comp.right);
        ImGui::Checkbox(_labelPrefix("up").c_str(), &comp.up);
        ImGui::EndGroupPanel();
    }
    if (e.HasComponent<Health>()) {
        auto comp = e.GetComponent<Health>();
        ImGui::BeginGroupPanel("Health");
        ImGui::InputInt(_labelPrefix("Health").c_str(), &comp.healthPercentage);
        ImGui::EndGroupPanel();
    }
}

static Entity selectedEntityId = NULL;
void Properties() {
    if (ImGui::Begin("Properties")) {
        if (selectedEntityId != NULL)
            SerializeEntity(selectedEntityId);
        ImGui::End();
    }
}

void SceneList() {

    static bool selectableEntityList[1024];
    if (ImGui::Begin("Scene List")) {

        for (auto entity : worldRef->GetEntities()) {
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

// void Editor::ProcessInput(SDL_Event* event) {
//     ImGui_ImplSDL2_ProcessEvent(event);
// }

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

    renderDockingSpace();

    static ImVec2 size;
    Properties();
    SceneList();
    if (ImGui::Begin("Files")) {
        static int folderIcon = AssetManager::GetTexture("folder");
        static int fileIcon = AssetManager::GetTexture("file");

        static std::string path = ".";
        static std::string lastPath = ".";
        static bool hoverList[1024];
        int ctx = 0;

        for (const auto& entry : fs::directory_iterator(path)) {

            if (ctx == 0) {

                ImGui::SameLine();
                bool isHovered = ImGui::IsItemHovered();
                if (isHovered) {
                    hoverList[ctx - 1] = true;
                } else
                    hoverList[ctx - 1] = false;

                ImGui::BeginGroup();
                if (hoverList[ctx]) {
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }

                ImGui::PushID("...");
                if (ImGui::ImageButton((void*)(intptr_t)folderIcon, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1))) {
                    path = lastPath;
                }
                ImGui::PopID();

                ImGui::Text("..");
                ImGui::EndGroup();

                if (hoverList[ctx])
                    ImGui::PopStyleVar();

                ImGui::SameLine();
            }

            std::string fileName = std::filesystem::path(entry.path()).stem();
            fileName = fileName.substr(0, fileName.length() > 10 ? 10 : fileName.length());

            int ico = 0;
            std::error_code ec;
            if (fs::is_directory(entry.path(), ec)) {
                ico = folderIcon;
            } else
                ico = fileIcon;

            bool isHovered = ImGui::IsItemHovered();
            if (isHovered) {
                hoverList[ctx - 1] = true;
            } else
                hoverList[ctx - 1] = false;

            ImGui::BeginGroup();
            if (hoverList[ctx]) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }

            ImGui::PushID(fileName.c_str());
            if (ImGui::ImageButton((void*)(intptr_t)ico, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1))) {
                lastPath = path;
                path = entry.path();
            }
            ImGui::PopID();

            ImGui::Text(fileName.c_str());
            ImGui::EndGroup();

            if (hoverList[ctx])
                ImGui::PopStyleVar();

            ImGui::SameLine();
            ctx++;
        }

        ctx++;
        ImGui::End();
    }
    if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse)) {

        if (size.x != ImGui::GetWindowWidth() || size.y != ImGui::GetWindowHeight()) {

            size.x = ImGui::GetWindowWidth();
            size.y = ImGui::GetWindowHeight();
            renderer->SetFrameSize(size.x, size.y);
        }

        auto tex = renderer->GetFrame();


        static ImVec2 uv0 = { 0, 1 };
        static ImVec2 uv1 = { 1, 0 };
        ImGui::Image((void*)tex, size, uv0, uv1);
        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
