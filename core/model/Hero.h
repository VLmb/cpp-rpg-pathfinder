#pragma once

#include "Grid.h"

enum class HeroType {
    Wood_Elf,
    Dwarf,
    Human
};

class Hero {
private:
    double health;
    double baseSpeed;

    const double rockSpeed;
    const double vegetationSpeed;
    const double moistureSpeed;
public:
    Hero(double initialHealth, double initialSpeed,
        const double rockSpeed,
        const double vegetationSpeed,
        const double moistureSpeed)
        : health(initialHealth), baseSpeed(initialSpeed),
        rockSpeed(rockSpeed),
        vegetationSpeed(vegetationSpeed),
        moistureSpeed(moistureSpeed) {}

    virtual HeroType getHeroType() const = 0;

    double getSpeed(CellType terrain) {
        return (terrain.rockinessAvg * rockSpeed + 
                terrain.vegetationAvg * vegetationSpeed +
                terrain.moistureAvg * moistureSpeed) * baseSpeed / 3.0f;
    }

    double getTimeToCross(CellType terrain) {
        return 1.0f / getSpeed(terrain);
    }

    double getMaxTimeToCross() {
        return 1.0f / (std::min({ rockSpeed, vegetationSpeed, moistureSpeed }) * baseSpeed);
    }

    double getMinTimeToCross() {
        return 1.0f / (std::max({ rockSpeed, vegetationSpeed, moistureSpeed }) * baseSpeed);
    }

    double getHealth() const {
        return health;
    }

    void setHealth(double newHealth) {
        health = newHealth;
    }

    double getBaseSpeed() const {
        return baseSpeed;
    }
};

class Elf : public Hero {
public:
    Elf() : Hero(120, 1.0, 1.0, 2.5, 1.5) {}  // rock, veget, moister

    HeroType getHeroType() const override {
        return HeroType::Wood_Elf;
    }
};

class Dwarf : public Hero {
public:
    Dwarf() : Hero(150, 1.0, 2.8, 1.2, 1.0) {}  // rock, veget, moister

    HeroType getHeroType() const override {
        return HeroType::Dwarf;
    }
};

class Human : public Hero {
public:
    Human() : Hero(100, 1.0, 1.0, 1.0, 1.0) {}  // rock, veget, moister

    HeroType getHeroType() const override {
        return HeroType::Human;
    }
};