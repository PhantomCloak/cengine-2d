#pragma once

struct Projectile {
    bool friendly;
    int hitPercentageDmg;
    int duration;
    int startTime;

    Projectile(bool friendly = false, int hitPercentageDmg = 0, int duration = 0) {
        this->friendly = friendly;
        this->hitPercentageDmg = hitPercentageDmg;
        this->duration = duration;
        //startTime = SDL_GetTicks();
    }
};
