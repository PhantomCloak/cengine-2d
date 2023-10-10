#pragma once
#include <string>

class EditorWindow{
  public:
    virtual void RenderWindow() = 0;
    std::string _labelPrefix(const char* const label);
};
