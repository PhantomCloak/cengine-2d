#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <list>
#include <map>
#include <typeindex>
#include <util.h>

class Event {
    public:
    Event() = default;
};

class IEventCallback {
    private:
    virtual void Call(Event& a) = 0;

    public:
    virtual ~IEventCallback() = default;
    void Execute(Event& e) {
        Call(e);
    }
};

template <typename TSrc, typename TEvent>
class EventCallback : public IEventCallback {
    private:
    typedef void (TSrc::*CallbackFunction)(TEvent&);
    TSrc* instance;
    CallbackFunction callback;
    virtual void Call(Event& e) override {
        std::invoke(callback, instance, static_cast<TEvent&>(e));
    }

    public:
    EventCallback(TSrc* instance, CallbackFunction callback) {
        this->instance = instance;
        this->callback = callback;
    }
    virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
    public:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subs;
    EventBus() {
    }
    ~EventBus() {
    }

    template <typename TEvent, typename TInstance>
    void SubscribeEvent(TInstance* instance, void (TInstance::*callback)(TEvent&)) {
        if (!subs[typeid(TEvent)].get()) {
            subs[typeid(TEvent)] = std::make_unique<HandlerList>();
        }

        auto subscriber = std::make_unique<EventCallback<TInstance, TEvent>>(instance, callback);
        subs[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename... TArgs>
    void EmitEvent(TArgs&&... args) {
        auto handlers = subs[typeid(TEvent)].get();

        if (!handlers)
            return;

        for (auto it = handlers->begin(); it != handlers->end(); it++) {
            auto handler = it->get();
            TEvent event(std::forward<TArgs>(args)...);
            handler->Execute(event);
        }
    }
};
#endif
