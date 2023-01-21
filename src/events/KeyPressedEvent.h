#pragma once

#include "../eventmgr/EventBus.h"

class KeyPressEvent {
    public:
    const uint8_t* keyboard;
    //KeyPressEvent(const uint8_t* keyboard)
    //: keyboard(keyboard) {
    //}
};

class KeyPressUpEvent {
    public:
    int key;
    //KeyPressUpEvent(SDL_Keycode key)
    //: key(key) {
    //}
};

class MousePressedEvent {
    public:
    int symbol;
    //MousePressedEvent(SDL_Keycode symbol)
    //: symbol(symbol) {
    //}
};
