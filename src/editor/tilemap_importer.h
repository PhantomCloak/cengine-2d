#pragma once
#include "editor_window.h"
#include <functional>

class TileMapImporter : EditorWindow {
    public:
    void OpenImporter(std::function<void(bool)> successCallback = nullptr);
    void RenderWindow() override;
};
