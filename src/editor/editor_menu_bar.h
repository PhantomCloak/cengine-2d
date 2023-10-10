#pragma once
#include "editor_window.h"

class EditorMenuBar : EditorWindow {
  public:
    void RenderWindow() override;
  private:
    void LoadDialog();
    void FileMenu();
    void EntitiesMenu();
    void AssetsMenu();
    void SaveDialog();
};
