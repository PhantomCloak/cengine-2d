#ifndef POOL_H
#define POOL_H

#include <vector>

class IPool {
    public:
    virtual ~IPool(){};
};

template <typename T>
class Pool : public IPool {
    private:
    std::vector<T> components;

    public:
    Pool(int size = 100) {
        Resize(size);
    }

    virtual ~Pool() = default;

    bool IsEmpty() const {
        return components.empty();
    }
    int GetSize() const {
        return components.size();
    }
    void Resize(int n) {
        components.resize(n);
    }
    void Clear() {
        components.clear();
    }

    void Add(int index, T component) {
        components.push_back(component);
    }

    void Set(int index, T component) {
        components[index] = component;
    }

    T& Get(int index) {
        return static_cast<T&>(components[index]);
    }

    T& operator[](int index) {
        return static_cast<T&>(components[index]);
    }
};
#endif
