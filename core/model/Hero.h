#pragma once

#include "Grid.h"

enum class HeroType {
    ORC,
    WOOD_ELF,
    HUMAN,
    GNOME
};

class Hero {
private:
    double health;
    double baseSpeed;

    HeroType type;

    const double temperaturePreference;
    const double altitudePreference;
    const double moisturePreference;

protected:
    Hero(double initialHealth, double initialSpeed,
        const double temperaturePreference,
        const double altitudePreference,
        const double moisturePreference,
        HeroType type
        )
        : health(initialHealth), baseSpeed(initialSpeed),
        temperaturePreference(temperaturePreference),
        altitudePreference(altitudePreference),
        moisturePreference(moisturePreference),
        type(type)
    {}
public:
    virtual ~Hero() = default;

    double getSpeed(CellProperty terrain) {
        return (terrain.rockinessAvg * temperaturePreference +
                terrain.vegetationAvg * altitudePreference +
                terrain.moistureAvg * moisturePreference) * baseSpeed / 3.0f;
    }

    double getTimeToCross(CellProperty terrain) {
        return 1.0f / getSpeed(terrain);
    }

    double getMaxTimeToCross() {
        return 1.0f / (std::min({ temperaturePreference, altitudePreference, moisturePreference }) * baseSpeed);
    }

    double getMinTimeToCross() {
        return 1.0f / (std::max({ temperaturePreference, altitudePreference, moisturePreference }) * baseSpeed);
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
    WoodElf() : Hero(120, 1.0, 3.0f, 1.0f, 12.0f, HeroType::WOOD_ELF) {}  // rock, veget, moister
};

class Orc : public Hero {
public:
    Orc() : Hero(150, 1.0, 12.0f, 3.0f, 1.0f, HeroType::ORC) {}  // rock, veget, moister
};

class Gnome : public Hero {
public:
    Gnome(): Hero(150, 1.0, 100.0f, 1.0f, 1000.0f, HeroType::ORC) {}
};

class Human : public Hero {
public:
    Human() : Hero(100, 5.0f, 5.0f, 5.0f, 1.0f, HeroType::HUMAN) {}  // rock, veget, moister
};