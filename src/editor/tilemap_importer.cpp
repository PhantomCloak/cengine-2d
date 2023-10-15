#include "tilemap_importer.h"
#include "../assetmgr/AssetManager.h"
#include "../io/filesystem.h"
#include "NMB/NMB.h"
#include "imgui.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

std::function<void(int)> textureImportCallback;

int textureIdImp;
int textureWidth;
int textureHeight;
int originalWidth;
int originalHeight;

int tileWidth = 32;  
int tileHeight = 32;
int offsetX = 0;   
int offsetY = 0;  

int textureId;

std::string filePathRootResult;
std::string filePathResult;
std::string fileNameResult;
std::string fileExtensionResult;

std::set<int> clickedTileIndices;

bool TileMapImporter::IsFocused() {
    return ImGui::IsWindowFocused();
}

void TileMapImporter::OpenImporter(std::function<void(bool)> successCallback) {

    FileSys::OpenFilePickerDialog([](std::string filePath) {
        if (filePath.empty()) {
            return;
        }

        filePathResult = filePath;
        fileNameResult = FileSys::GetFileName(filePath);
        fileExtensionResult = FileSys::GetFileExtension(filePath);

        if (fileExtensionResult == "png" || fileExtensionResult == "jpg") {
            filePathRootResult = FileSys::GetParentDirectory(filePath);
            textureId = AssetManager::AddTexture(fileNameResult, filePath);
        }
    });

    textureIdImp = textureId;

    TextureInf selectedMafInf = AssetManager::GetTextureInf(AssetManager::GetTexture(textureId));

    textureWidth = selectedMafInf.width;
    textureHeight = selectedMafInf.height;

    tileHeight = selectedMafInf.height;
    tileWidth = selectedMafInf.width;

    originalWidth = selectedMafInf.width;
    originalHeight = selectedMafInf.height;

    if(successCallback != nullptr)
      textureImportCallback = successCallback;
}

void TileMapImporter::RenderWindow() {
    static bool open = true;

    float aspectRatio = (float)textureWidth / (float)textureHeight;

    if (textureWidth < 512) {
        textureWidth = 512;
        textureHeight = static_cast<int>(textureWidth / aspectRatio);
    }
    if (textureHeight < 512) {
        textureHeight = 512;
        textureWidth = static_cast<int>(textureHeight * aspectRatio);
    }

    float scaleX = (float)textureWidth / originalWidth;
    float scaleY = (float)textureHeight / originalHeight;

    float windowPadding = 20.0f;
    ImGui::SetNextWindowSize(ImVec2(textureWidth + windowPadding, textureHeight + 150));

    if (ImGui::Begin("TileMap Importer", &open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize)) {
        ImGui::InputInt(_labelPrefix("Tile Width: ").c_str(), &tileWidth);
        ImGui::InputInt(_labelPrefix("Tile Height: ").c_str(), &tileHeight);
        ImGui::InputInt(_labelPrefix("Offset X").c_str(), &offsetX);
        ImGui::InputInt(_labelPrefix("Offset Y").c_str(), &offsetY);

        ImGui::Spacing();

        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::Image((void*)&textureIdImp, ImVec2(textureWidth, textureHeight));

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        for (int x = offsetX; x <= textureWidth; x += tileWidth * scaleX) {
            draw_list->AddLine(ImVec2(p.x + x, p.y), ImVec2(p.x + x, p.y + textureHeight), IM_COL32(200, 0, 0, 255));
        }
        for (int y = offsetY; y <= textureHeight; y += tileHeight * scaleY) {
            draw_list->AddLine(ImVec2(p.x, p.y + y), ImVec2(p.x + textureWidth, p.y + y), IM_COL32(200, 0, 0, 255));
        }

        int columns = textureWidth / (tileWidth * scaleX);
        int rows = textureHeight / (tileHeight * scaleY);
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                float x = p.x + col * tileWidth * scaleX;
                float y = p.y + row * tileHeight * scaleY;

                if (ImGui::IsMouseHoveringRect(ImVec2(x, y), ImVec2(x + tileWidth * scaleX, y + tileHeight * scaleY)) && ImGui::IsMouseClicked(0)) {
                    int index = row * columns + col;

                    if (clickedTileIndices.find(index) != clickedTileIndices.end()) {
                        clickedTileIndices.erase(index);
                    } else
                        clickedTileIndices.insert(index);
                }
            }
        }

        for (auto index : clickedTileIndices) {
            int row = index / columns;
            int col = index % columns;

            float x = p.x + col * tileWidth * scaleX;
            float y = p.y + row * tileHeight * scaleY;

            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + tileWidth * scaleX, y + tileHeight * scaleY), IM_COL32(255, 0, 0, 127)); 
        }
    }

    if (ImGui::Button("Save")) {
        json::JSON mapMeta = json::Array();
        json::JSON emptyIdices = json::Array();

        mapMeta["tileWidth"] = tileWidth;
        mapMeta["tileHeight"] = tileHeight;
        mapMeta["offsetX"] = offsetX;
        mapMeta["offsetY"] = offsetY;

        for (auto index : clickedTileIndices) {
            emptyIdices.append(index);
        }

        mapMeta["emptyIndices"] = emptyIdices;

        std::string fileCopyPathDestination = AssetManager::GetAssetRootPath() + "tile_maps/" + fileNameResult;

        FileSys::CCopyFile(filePathResult, fileCopyPathDestination);

        FileSys::OpenFileSaveDialog(fileNameResult + ".json", FileSys::GetParentDirectory(fileCopyPathDestination), [&](std::string filePath) {
            std::ofstream file(filePath);
            file << mapMeta.dump(5);
            file.close();

            
            NMB::show( "Sucess", "Asset successfully imported.", NMB::Icon::ICON_INFO );
            Log::Inf("Asset successfully imported to " + filePath);
            textureImportCallback(1);
        });
    }

    ImGui::End();
}
