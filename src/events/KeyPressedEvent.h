#pragma once

#include "../eventmgr/EventBus.h"
#include "SDL_keycode.h"

class KeyPressedEvent : public Event {
    public:
    SDL_Keycode symbol;
    KeyPressedEvent(SDL_Keycode symbol)
    : symbol(symbol) {
    }
};

class MousePressedEvent : public Event {
    public:
    SDL_Keycode symbol;
    MousePressedEvent(SDL_Keycode symbol)
    : symbol(symbol) {
    }
};
