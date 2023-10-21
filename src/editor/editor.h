#pragma once

#include "../render/render_primitives.h"
#include "../render/render.h"
#include "animation_designer.h"
#include "editor_menu_bar.h"
#include "fileview.h"
#include "log_viewer.h"
#include "system_explorer.h"
#include "entity_inspector.h"
#include "viewport.h"
#include "tile_placer.h"
#include "tilemap_importer.h"
#include "scene_list.h"
#include "animation_editor.h"

class Editor {
    public:
    Editor() = default;
    void Init(CommancheRenderer* renderer);
    static glm::vec2 GetCursorPosition();
    static glm::vec2 GetViewPortSize();
    void Render();
    std::shared_ptr<TileMapImporter> importer;
    std::shared_ptr<EditorViewPort> viewport;
    std::shared_ptr<SceneList> sceneList;
    static std::shared_ptr<Editor> Instance;
    private:
    void Keybindings();
    FileView* fileView;
    SystemExplorer* explorer;
    std::shared_ptr<TilePlacer> tilePlacer;
    std::shared_ptr<AnimationDesigner> animDesigner;
    std::shared_ptr<LogView> logView;
    std::shared_ptr<EntityInspector> entityInspector;
    std::unique_ptr<CommancheRenderer> renderer;
    std::shared_ptr<EditorMenuBar> menuBar;
    std::shared_ptr<AnimationEditor> animationEditor;
};
