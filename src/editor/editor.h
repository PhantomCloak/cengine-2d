#pragma once

#include "../ecs/world.h"
#include "../eventmgr/EventBus.h"
#include "../render/render_primitives.h"
#include "../render/render.h"
#include "../components/Camera.h"
#include "../game/map.h"
#include "../events/KeyPressedEvent.h"
#include "editor_menu_bar.h"
#include "fileview.h"
#include "log_viewer.h"
#include "system_explorer.h"
#include "entity_inspector.h"
#include "viewport.h"
#include "tile_placer.h"
#include "tilemap_importer.h"
#include "scene_list.h"

class Editor {
    public:
    Editor() = default;
    void Init(CommancheRenderer* renderer);
    static glm::vec2 GetCursorPosition();
    static glm::vec2 GetViewPortSize();
    void Render();
    private:
    static std::unique_ptr<Editor> Instance;
    void Keybindings();
    FileView* fileView;
    SystemExplorer* explorer;
    std::unique_ptr<TilePlacer> tilePlacer;
    std::unique_ptr<SceneList> sceneList;
    std::unique_ptr<TileMapImporter> importer;
    std::unique_ptr<EditorViewPort> viewport;
    std::unique_ptr<LogView> logView;
    std::unique_ptr<EntityInspector> entityInspector;
    std::unique_ptr<CommancheRenderer> renderer;
    std::unique_ptr<EditorMenuBar> menuBar;
};
