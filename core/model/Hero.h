#pragma once

#include "Enums.h"
#include "Grid.h"

class Hero {
private:
    int health;
    int baseSpeed;

    const int rockSpeed;
    const int vegetationSpeed;
    const int moistureSpeed;
public:
    Hero(int initialHealth, int initialSpeed,
        const int rockSpeed,
        const int vegetationSpeed,
        const int moistureSpeed)
        : health(initialHealth), baseSpeed(initialSpeed),
        rockSpeed(rockSpeed),
        vegetationSpeed(vegetationSpeed),
        moistureSpeed(moistureSpeed) {}

    double getSpeed(CellType terrain) {
        return (terrain.rockinessAvg * rockSpeed + 
                terrain.vegetationAvg * vegetationSpeed +
                terrain.moistureAvg * moistureSpeed) * baseSpeed / 3.0f;
    }

    int getHealth() const {
        return health;
    }

    void setHealth(int newHealth) {
        health = newHealth;
    }

    int getBaseSpeed() const {
        return baseSpeed;
    }
};

class Elf : public Hero {
public:
    Elf() : Hero(120, 1.0, 1.0, 2.5, 1.5) {}  // rock, veget, moister
};

class Dwarf : public Hero {
public:
    Dwarf() : Hero(150, 1.0, 2.8, 1.2, 1.0) {}  // rock, veget, moister
};

class Human : public Hero {
public:
    Human() : Hero(100, 1.0, 1.0, 1.0, 1.0) {}  // rock, veget, moister
};