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

    HeroType type;

    const double rockSpeed;
    const double vegetationSpeed;
    const double moistureSpeed;

protected:
    Hero(double initialHealth, double initialSpeed,
        const double rockSpeed,
        const double vegetationSpeed,
        const double moistureSpeed,
        HeroType type
        )
        : health(initialHealth), baseSpeed(initialSpeed),
        rockSpeed(rockSpeed),
        vegetationSpeed(vegetationSpeed),
        moistureSpeed(moistureSpeed),
        type(type)
    {}
public:
    virtual ~Hero() = default;

    double getSpeed(CellProperty terrain) {
        return (terrain.rockinessAvg * rockSpeed + 
                terrain.vegetationAvg * vegetationSpeed +
                terrain.moistureAvg * moistureSpeed) * baseSpeed / 3.0f;
    }

    double getTimeToCross(CellProperty terrain) {
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

    const HeroType getHeroType() const {
        return type;
    }
};

class WoodElf : public Hero {
public:
    WoodElf() : Hero(120, 1.0, 1.0, 2.5, 1.5, HeroType::Wood_Elf) {}  // rock, veget, moister
};

class Dwarf : public Hero {
public:
    Dwarf() : Hero(150, 1.0, 2.8, 1.2, 1.0, HeroType::Dwarf) {}  // rock, veget, moister
};

class Human : public Hero {
public:
    Human() : Hero(100, 1.0, 1.0, 1.0, 1.0, HeroType::Human) {}  // rock, veget, moister
};