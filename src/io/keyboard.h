#pragma once

#include <queue>

struct KeyEvent {
    int keyCode;
    bool isPressed;
};

class Keyboard {
    public:
    static void Setup(void* windowPtr);
    static bool IsKeyPressed(int keyCode);
    static bool IsKeyPressing(int keyCode);
    static std::vector<KeyEvent> GetPressedKeys();
    static void Poll();
    static void FlushPressedKeys();

    private:
};


enum KeyCode {
    Key_UNKNOWN = -1,
    Key_LArrow = 263,
    Key_UArrow=   265,
    Key_RArrow = 262,
    Key_DArrow = 264,
    Key_SPACE = 32,
    Key_APOSTROPHE = 39,
    Key_COMMA = 44,
    Key_MINUS = 45,
    Key_PERIOD = 46,
    Key_SLASH = 47,
    Key_CTRL = 341,
    Key_0 = 48,
    Key_1 = 49,
    Key_2 = 50,
    Key_3 = 51,
    Key_4 = 52,
    Key_5 = 53,
    Key_6 = 54,
    Key_7 = 55,
    Key_8 = 56,
    Key_9 = 57,
    Key_A = 65,
    Key_B = 66,
    Key_C = 67,
    Key_D = 68,
    Key_E = 69,
    Key_F = 70,
    Key_G = 71,
    Key_H = 72,
    Key_I = 73,
    Key_J = 74,
    Key_K = 75,
    Key_L = 76,
    Key_M = 77,
    Key_N = 78,
    Key_O = 79,
    Key_P = 80,
    Key_Q = 81,
    Key_R = 82,
    Key_S = 83,
    Key_T = 84,
    Key_U = 85,
    Key_V = 86,
    Key_W = 87,
    Key_X = 88,
    Key_Y = 89,
    Key_Z = 90
};
