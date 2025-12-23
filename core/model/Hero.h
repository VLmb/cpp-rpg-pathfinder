#pragma once

#include <algorithm>
#include <string>
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

    double getSpeed(CellProperty terrain) const {
        return (terrain.temperatureAvg * temperaturePreference +
                terrain.altitudeAvg * altitudePreference +
                terrain.moistureAvg * moisturePreference) * baseSpeed / 3.0f;
    }

    double getSpeedByCastTerrain(CellProperty terrain) const {
        return (terrain.castTemperatureAvg * temperaturePreference +
                terrain.castAltitudeAvg * altitudePreference +
                terrain.castMoistureAvg * moisturePreference) * baseSpeed / 3.0f;
    }

    double getTimeToCross(CellProperty terrain) const {
        return 100.0f / getSpeed(terrain);
    }

    double getTimeToCrossCastTerrain(CellProperty terrain) const {
        return 100.0f / getSpeedByCastTerrain(terrain);
    }

    double getMaxTimeToCross() const {
        return 100.0f / (std::min({ temperaturePreference, altitudePreference, moisturePreference }) * baseSpeed);
    }

    double getMinTimeToCross() const {
        return 100.0f / (std::max({ temperaturePreference, altitudePreference, moisturePreference }) * baseSpeed);
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
    WoodElf() : Hero(120, 1.0, 0.0001f, 0.0001f, 10000.0f, HeroType::WOOD_ELF, "Лесной Эльф") {}
};

class Orc : public Hero {
public:
    Orc() : Hero(150, 1.0, 10000.0f, 0.0001f, 0.0001f, HeroType::ORC, "Орк") {}
};

class Gnome : public Hero {
public:
    Gnome(): Hero(150, 1.0, 0.0001f, 10000.0f, 0.0001f, HeroType::GNOME, "Горный Гном") {}
};

class Human : public Hero {
public:
    Human() : Hero(100, 10.0f, 1.0f, 1.0f, 1.0f, HeroType::HUMAN, "Человек") {}
};
