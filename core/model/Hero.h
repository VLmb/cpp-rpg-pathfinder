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
    const std::string heroName;

    const double temperaturePreference;
    const double altitudePreference;
    const double moisturePreference;

protected:
    Hero(double initialHealth, double initialSpeed,
        const double temperaturePreference,
        const double altitudePreference,
        const double moisturePreference,
        HeroType type,
        const std::string heroName
        )
        : health(initialHealth), baseSpeed(initialSpeed),
        temperaturePreference(temperaturePreference),
        altitudePreference(altitudePreference),
        moisturePreference(moisturePreference),
        type(type),
        heroName(heroName)
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

    const std::string& getHeroName() const {
        return heroName;
    }
};

class WoodElf : public Hero {
public:
    WoodElf() : Hero(120, 1.0, 1.0f, 30.0f, 100.0f, HeroType::WOOD_ELF, "Лесной Эльф") {}  // rock, veget, moister
};

class Orc : public Hero {
public:
    Orc() : Hero(150, 1.0, 100.0f, 1.0f, 1.0f, HeroType::ORC, "Орк") {}  // rock, veget, moister
};

class Gnome : public Hero {
public:
    Gnome(): Hero(150, 1.0, 1.0f, 100.0f, 50.0f, HeroType::GNOME, "Горный Гном") {}
};

class Human : public Hero {
public:
    Human() : Hero(100, 5.0f, 50.0f, 50.0f, 50.0f, HeroType::HUMAN, "Человек") {}  // rock, veget, moister
};