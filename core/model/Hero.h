#pragma once

#include "Enums.h"

class Hero {
private:
    int health;
    int baseSpeed;
public:
    Hero(int initialHealth, int initialSpeed) : health(initialHealth), baseSpeed(initialSpeed) {}

    virtual double getSpeed(TerrainType terrain) const = 0;

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
    Elf() : Hero(120, 1.0) {}

    double getSpeed(TerrainType terrain) const override {
      switch (terrain)
        {
        case TerrainType::FOREST:
            return getBaseSpeed() * 1.5;
        case TerrainType::MOUNTAIN:
            return getBaseSpeed() * 0.5;
        case TerrainType::SWAMP:
            return getBaseSpeed() * 1.0;
        default:
            return getBaseSpeed();
        }
    }
};

class Dwarf : public Hero {
public:
    Dwarf() : Hero(150, 1.0) {}

    double getSpeed(TerrainType terrain) const override {
        switch (terrain)
        {
        case TerrainType::FOREST:
            return getBaseSpeed() * 0.5;
        case TerrainType::MOUNTAIN:
            return getBaseSpeed() * 1.5;
        case TerrainType::SWAMP:
            return getBaseSpeed() * 1.0;
        default:
            return getBaseSpeed();
        }
    }
};

class Human : public Hero {
public:
    Human() : Hero(100, 1.0) {}

    double getSpeed(TerrainType terrain) const override {
        switch (terrain)
        {
        case TerrainType::FOREST:
            return getBaseSpeed() * 1.0;
        case TerrainType::MOUNTAIN:
            return getBaseSpeed() * 1.0;
        case TerrainType::SWAMP:
            return getBaseSpeed() * 1.0;
        default:
            return getBaseSpeed();
        }
    }
};