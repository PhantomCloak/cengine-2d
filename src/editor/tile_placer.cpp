#include "tile_placer.h"
#include "../assetmgr/AssetManager.h"
#include "../game/components.h"
#include "../io/filesystem.h"
#include "../scene/scene.h"
#include "editor_utils.h"
#include "flecs.h"
#include "imgui.h"
#include <vector>

int zIndexStart = 500;
int lastLoadedAssetSize = 0;
std::vector<std::string> loadedAssets;

TextureInf selectedMafInf;
static int selectedTextureId;

static bool isInit = false;
static int tileWidth = 16;
static int tileHeight = 16;
static int tileOffsetX = 0;
static int tileOffsetY = 0;
std::vector<int> blacklistedTiles;

void RefreshLoadedTiles() {
    loadedAssets = AssetManager::GetLoadedTextures();
    lastLoadedAssetSize = loadedAssets.size();
}

void LoadTileAsset(const char* selectedItem) {
    selectedTextureId = AssetManager::GetTexture(selectedItem);
    selectedMafInf = AssetManager::GetTextureInf(selectedItem);

    std::string assetPathRoot = AssetManager::GetAssetRootPath();

    std::string mapMetaJson = FileSys::ReadFile(assetPathRoot + "tile_maps/" + std::string(selectedItem) + ".json");

    if (mapMetaJson == "") {
        Log::Warn("No map meta data found for " + std::string(selectedItem));
        return;
    }

    auto mapMeta = json::JSON::Load(mapMetaJson);

    tileWidth = mapMeta["tileWidth"].ToInt();
    tileHeight = mapMeta["tileHeight"].ToInt();
    tileOffsetX = mapMeta["tileOffsetX"].ToInt();
    tileOffsetY = mapMeta["tileOffsetY"].ToInt();

    for (auto& tile : mapMeta["emptyIndices"].ArrayRange()) {
        blacklistedTiles.push_back(tile.ToInt());
    }

    Log::Inf("Loaded map meta data for " + std::string(selectedItem));
    Log::Inf("tileWidth = " + std::to_string(tileWidth));
    Log::Inf("tileHeight = " + std::to_string(tileHeight));
    Log::Inf("tileOffsetX = " + std::to_string(tileOffsetX));
    Log::Inf("tileOffsetY = " + std::to_string(tileOffsetY));
    Log::Inf("blacklistedTiles = " + std::to_string(blacklistedTiles.size()));
}

bool TilePlacer::IsFocused() {
    return ImGui::IsWindowFocused();
}

void TilePlacer::RenderWindow() {
    ImGui::Begin("Tile Editor");

    if (loadedAssets.size() == 0) {
        RefreshLoadedTiles();
    } else if (lastLoadedAssetSize != loadedAssets.size()) {
        RefreshLoadedTiles();
    }

    static const char* selectedItem = loadedAssets[0].c_str();

    if (!isInit) {
        LoadTileAsset(selectedItem);
        isInit = true;
    }

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

                LoadTileAsset(selectedItem);
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
    int tileSizeH = tileHeight;
    int tileSizeW = tileHeight;


    if(tileSizeH == 0 || tileSizeW == 0)
    {
      tileSizeH = 16;
      tileSizeW = 16;
    }

    int columns = selectedMafInf.width / tileSizeH;
    int rows = selectedMafInf.height / tileSizeW;
    int totalScanArea = columns * rows;


    ImVec2 availRegion = ImGui::GetContentRegionAvail();
    int slice = availRegion.x / 74;

    int placedCount = 0;
    for (int i = 0; i < totalScanArea; i++) {
        auto identifier = std::to_string(i);

        int currentColumn = i % columns;
        int currentRow = i / columns;

        if (blacklistedTiles.size() > 0) {
            if (std::find(blacklistedTiles.begin(), blacklistedTiles.end(), i) != blacklistedTiles.end()) {
                continue;
            }
        }

        if ((placedCount % slice) == 0) {
            ImGui::NewLine();
        }

        ImVec2 uv0 = ImVec2(EditorUtils::pixelCordToUvX2(currentColumn * tileSizeW, selectedMafInf.width), EditorUtils::pixelCordToUvY2(currentRow * tileSizeH, selectedMafInf.height));
        ImVec2 uv1 = ImVec2(EditorUtils::pixelCordToUvX2((currentColumn + 1) * tileSizeW, selectedMafInf.width), EditorUtils::pixelCordToUvY2((currentRow + 1) * tileSizeH, selectedMafInf.height));


        if (ImGui::ImageButton(identifier.c_str(), (void*)&selectedTextureId, ImVec2(64, 64), uv0, uv1)) {
            Log::Warn("Clicked on tile " + identifier);
            flecs::entity piece = Scene::CreateEntity("tile" + std::to_string(zIndexStart));

            piece.set<Sprite>({ selectedItem, zIndexStart, static_cast<float>(currentColumn * tileSizeW), static_cast<float>(currentRow * tileSizeH), (float)tileSizeH, (float)tileSizeW });
            piece.set<DragableComponent>({ true });
            piece.set<RectTransformC>({ glm::vec2(0, 0), glm::vec2(5, 5) });
            zIndexStart++;
        }
        placedCount++;
        ImGui::SameLine();
    }

    ImGui::End();
}
