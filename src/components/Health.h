#pragma once

struct Health {
    int healthPercentage;

    Health(int healthPercentage = 100) {
        this->healthPercentage = healthPercentage;
    }
};
