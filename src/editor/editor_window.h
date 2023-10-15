#pragma once
#include <string>

class EditorWindow {
    public:
    virtual ~EditorWindow() = default;
    virtual void RenderWindow() = 0;
    virtual bool IsFocused() = 0;
    std::string _labelPrefix(const char* const label);
};
