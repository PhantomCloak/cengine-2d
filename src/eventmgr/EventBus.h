#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "../log/log.h"
#include <list>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <util.h>

class IEvent {
    public:
    virtual void Clear(){};
    virtual ~IEvent(){};
};

template <typename T>
class EventQueue : public IEvent {
    public:
    std::vector<T> events;

    EventQueue() = default;
    void Clear() override {
        events.clear();
    }

    void Add(T event) {
        events.push_back(event);
    }

    virtual ~EventQueue() {
        Clear();
    }
};

class EventBus {
    public:
    std::unordered_map<std::type_index, IEvent*> queueStorage;

    EventBus() = default;
    ~EventBus() = default;

    template <typename TEvent, typename... TArgs>
    void PushEvent(TArgs&&... args) {
        const std::type_info& type = typeid(TEvent);
        if (queueStorage.find(std::type_index(type)) == queueStorage.end()) {
            queueStorage[std::type_index(type)] = new EventQueue<TEvent>();
        }

        auto v = queueStorage.at(std::type_index(type));
        EventQueue<TEvent>* ref = static_cast<EventQueue<TEvent>*>(v);
        TEvent event(std::forward<TArgs>(args)...);
        ref->Add(event);
    }


    template <typename TEvent>
    std::vector<TEvent> GetEvents() const {
        const std::type_info& type = typeid(TEvent);
        if (queueStorage.find(std::type_index(type)) == queueStorage.end()) {
            Log::Warn(std::string("an system tried to access evet type that hasn't registered yet ") + type.name());
            return std::vector<TEvent>();
        }
        auto v = queueStorage.at(std::type_index(type));
        EventQueue<TEvent>* ref = static_cast<EventQueue<TEvent>*>(v);
        return ref->events;
    }

    void ClearEvents() {
        for (auto queue : queueStorage) {
            queue.second->Clear();
        }
    }
};

#endif
