#ifndef COMPONENT_H
#define COMPONENT_H

struct ComponentBase {
  protected:
    static int nextId;
};

template <typename T>
class Component : public ComponentBase {
public:
  static int GetId() {
    static auto id = nextId++;
    return id;
  }

};
#endif
