#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../map-generation/MapGenerator.h"
#include "../model/Hero.h" 

class MapRenderer {
public:
    struct Color { 
        int r, g, b; 
    };

private:
    int map_width;
    int map_height;

    std::vector<std::vector<Color>> map_basePixels;
    std::vector<std::vector<Color>> map_modifiedPixels;

    Color getBiomeColor(map_generator::BiomeType type) const {
        switch (type) {
            // Вода: Синий (классический)
            case map_generator::BiomeType::WATER: return {30, 140, 250};

                // Песок: Светлый бежевый
            case map_generator::BiomeType::SAND: return {240, 230, 140};

                // Равнина: Светло-светло зеленый (самая низкая и светлая зелень)
            case map_generator::BiomeType::PLAIN: return {46, 139, 87};

                // Лес: Настоящий насыщенный зеленый (средняя высота)
            case map_generator::BiomeType::FOREST: return {85, 107, 47};

                // Холмы: Темно-зеленый (выше леса -> темнее)
            case map_generator::BiomeType::HILLS: return {0, 82, 0};

                // Болото: Коричневый / Грязно-бурый
            case map_generator::BiomeType::SWAMP: return {0, 60, 40};

                // Горы: Темно-серый (самая высокая и самая темная точка)
            case map_generator::BiomeType::MOUNTAIN: return {100, 100, 100};

            default: return {255, 0, 255};
        }
    }

    Color lerpColor(Color c1, Color c2, double t) const {
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;

        return {
            static_cast<int>(c1.r + (c2.r - c1.r) * t),
            static_cast<int>(c1.g + (c2.g - c1.g) * t),
            static_cast<int>(c1.b + (c2.b - c1.b) * t)
        };
    }

    enum class SurfaceType {
    PLAIN,
    FOREST,
    WINTER_PLAIN,
    WINTER_FOREST,

    HILLS,
    HILLY_JUNGLE,
    WINTER_HILLS,
    HILLY_WINTER_FOREST,

    ROCKY_PEAKS,
    SNOWY_ROCKS,

    LAKE
    };

    Color getNaturalBiomeColor(SurfaceType type) const {
        switch (type) {
            case SurfaceType::PLAIN:               return {140, 190, 100};
            case SurfaceType::FOREST:              return {20,  125,  40};
            case SurfaceType::WINTER_PLAIN:        return {170, 200, 210};
            case SurfaceType::WINTER_FOREST:       return {30,  110,  70};

            case SurfaceType::HILLS:               return {115, 155,  80};
            case SurfaceType::HILLY_JUNGLE:        return {10,  100,  30};
            case SurfaceType::WINTER_HILLS:        return {140, 170, 185};
            case SurfaceType::HILLY_WINTER_FOREST: return {22,   85,  55};

            case SurfaceType::ROCKY_PEAKS:         return {77, 76, 76};
            case SurfaceType::SNOWY_ROCKS:         return {228, 234, 243};

            case SurfaceType::LAKE:                return {34,  120, 196};

            default:                               return {255, 0, 255};
        }
    }

    Color getNaturalColor(const map_generator::TerrainType& cell) const {
        double temperature = cell.temperature;
        double altitude    = cell.altitude;
        double moisture    = cell.moisture;

        SurfaceType type;

        if (moisture > 0.95) {
            type = SurfaceType::LAKE;
        }
        else if (altitude < 0.33) {
            if (temperature > 0.50) {
                if (moisture > 0.50) {
                    type = SurfaceType::FOREST;
                } else {
                    type = SurfaceType::PLAIN;
                }
            } else {
                if (moisture > 0.50) {
                    type = SurfaceType::WINTER_FOREST;
                } else {
                    type = SurfaceType::WINTER_PLAIN;
                }
            }
        }
        else if (altitude < 0.75) {
            if (temperature > 0.50) {
                if (moisture > 0.50) {
                    type = SurfaceType::HILLY_JUNGLE;
                } else {
                    type = SurfaceType::HILLS;
                }
            } else {
                if (moisture > 0.50) {
                    type = SurfaceType::HILLY_WINTER_FOREST;
                } else {
                    type = SurfaceType::WINTER_HILLS;
                }
            }
        }
        else {
            if (temperature > 0.50) {
                if (moisture > 0.50) {
                    type = SurfaceType::ROCKY_PEAKS;
                } else {
                    type = SurfaceType::ROCKY_PEAKS;
                }
            } else {
                if (moisture > 0.50) {
                    type = SurfaceType::SNOWY_ROCKS;
                } else {
                    type = SurfaceType::ROCKY_PEAKS;
                }
            }
        }

        return getNaturalBiomeColor(type);
    }



    Color getHeroPathColor(const Hero& hero) const {

        auto type = hero.getHeroType();
        
        if (type == HeroType::ORC)    return {199, 21, 133};
        if (type == HeroType::WOOD_ELF) return {255, 0, 0};
        if (type == HeroType::HUMAN)    return {0, 0, 255};
        if (type == HeroType::GNOME)    return {0, 0, 0};
        
        return {255, 50, 50}; // Красный дефолт
    }

    Color blendColors(Color base, Color overlay, float alpha) const {
        return {
            static_cast<int>(base.r * (1.0f - alpha) + overlay.r * alpha),
            static_cast<int>(base.g * (1.0f - alpha) + overlay.g * alpha),
            static_cast<int>(base.b * (1.0f - alpha) + overlay.b * alpha)
        };
    }

    void writeBufferToFile(const std::vector<std::vector<Color>>& buffer, const std::string& filename) const {
        std::ofstream img(filename);
        if (!img.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        img << "P3\n" << map_width << " " << map_height << "\n255\n";

        for (int y = 0; y < map_height; ++y) {
            for (int x = 0; x < map_width; ++x) {
                const Color& c = buffer[y][x];
                img << c.r << " " << c.g << " " << c.b << " ";
            }
            img << "\n";
        }
        img.close();
        std::cout << "Image saved to " << filename << std::endl;
    }

    void drawPathCell(std::vector<std::vector<Color>>& buffer, int gridX, int gridY, int factor, Color color) const {
        int startX = gridX * factor;
        int startY = gridY * factor;
        float alpha = 0.5f; // Прозрачность пути

        for (int dy = 0; dy < factor; ++dy) {
            for (int dx = 0; dx < factor; ++dx) {
                int px = startX + dx;
                int py = startY + dy;

                if (px < map_width && py < map_height) {
                    buffer[py][px] = blendColors(buffer[py][px], color, alpha);
                }
            }
        }
    }


public:
    MapRenderer(const std::vector<std::vector<map_generator::TerrainType>>& map) {
        if (map.empty()) {
            map_height = 0; map_width = 0;
            return;
        }
        map_height = map.size();
        map_width = map[0].size();

        // Инициализируем буфер
        map_basePixels.resize(map_height, std::vector<Color>(map_width));

        // Заполняем цветами биомов (делаем это 1 раз!)
        for (int y = 0; y < map_height; ++y) {
            for (int x = 0; x < map_width; ++x) {
                // map_basePixels[y][x] = getBiomeColor(map[y][x].biome);
                map_basePixels[y][x] = getNaturalColor(map[y][x]);
            }
        }

        map_modifiedPixels = map_basePixels;
    }

    void saveBaseMap(const std::string& filename) const {
        writeBufferToFile(map_basePixels, filename);
    }

    void saveModifiedMap(const std::string& filename) const {
        writeBufferToFile(map_modifiedPixels, filename);
    }

    void drawPath(
        const std::vector<Point>& gridPath, 
        const Hero& hero, 
        int factor
    ) {

        Color pathColor = getHeroPathColor(hero);

        for (const auto& point : gridPath) {
            drawPathCell(map_modifiedPixels, point.x, point.y, factor, pathColor);
        }
    }

    void drawGraphVertices(
        const Point vertice,
        int vertexSize
    ) {
        Color borderColor = {255, 0, 0};
        Color centerColor = {255, 255, 0};

        // Incoming point is in grid coordinates; convert to pixel space.
        int startX = vertice.x;
        int startY = vertice.y;

        // Рисуем квадрат vertexSize x vertexSize от (startX, startY)
        for (int dy = 0; dy < vertexSize; ++dy) {
            for (int dx = 0; dx < vertexSize; ++dx) {
                int px = startX + dx;
                int py = startY + dy;

                // Проверка границ карты (чтобы не вылететь за массив)
                if (px >= 0 && px < map_width && py >= 0 && py < map_height) {

                    // Логика рамки: если это крайние пиксели квадрата
                    if (dx == 0 || dx == vertexSize - 1 || dy == 0 || dy == vertexSize - 1) {
                        map_modifiedPixels[py][px] = borderColor;
                    } else {
                        map_modifiedPixels[py][px] = centerColor;
                    }
                }
            }
        }
    }

    void undrawGraphVertices(
        const Point vertice,
        int vertexSize
    ) {
        int startX = vertice.x;
        int startY = vertice.y;

        for (int dy = 0; dy < vertexSize; ++dy) {
            for (int dx = 0; dx < vertexSize; ++dx) {
                int px = startX + dx;
                int py = startY + dy;

                if (px >= 0 && px < map_width && py >= 0 && py < map_height) {
                    map_modifiedPixels[py][px] = map_basePixels[py][px];
                }
            }
        }
    }

    void cleanMap(std::string& filename) {
        map_modifiedPixels = map_basePixels;
        saveModifiedMap(filename);
    }
};
