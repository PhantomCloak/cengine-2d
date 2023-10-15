#pragma once
#include "editor_window.h"
#include <memory>
#include <unordered_map>

#define EDITOR_SHOW_MAP_EDITOR 1
#define EDITOR_SHOW_SAVE_DIALOG 2
#define EDITOR_SHOW_LOAD_DIALOG 3
#define EDITOR_SYSTEM_EXPLORER_DIALOG 4

class Editor;

class EditorMenuBar : public EditorWindow {
  public:
    EditorMenuBar(std::shared_ptr<Editor> editor);
    void RenderWindow() override;
    bool IsFocused() override;
    std::unordered_map<int, bool> enabledWindows;
  private:
    std::shared_ptr<Editor> editorInstance;
    void LoadDialog();
    void FileMenu();
    void EntitiesMenu();
    void AssetsMenu();
    void SaveDialog();
};
