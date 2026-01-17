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


    Color lerpColor(Color c1, Color c2, double t) const {
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;

        return {
            static_cast<int>(c1.r + (c2.r - c1.r) * t),
            static_cast<int>(c1.g + (c2.g - c1.g) * t),
            static_cast<int>(c1.b + (c2.b - c1.b) * t)
        };
    }

    Color getNaturalBiomeColor(map_generator::BiomeType type) const {
        switch (type) {
            case map_generator::BiomeType::PLAIN: return {135, 171, 104};
            case map_generator::BiomeType::FOREST: return {0, 140, 0};
            case map_generator::BiomeType::WINTER_PLAIN: return {104, 171, 155};
            case map_generator::BiomeType::WINTER_FOREST: return {18, 127, 91};

            case map_generator::BiomeType::HILLS: return {92, 116, 67};
            case map_generator::BiomeType::HILLY_JUNGLE: return {0, 100, 0};
            case map_generator::BiomeType::WINTER_HILLS: return {72, 116, 104};
            case map_generator::BiomeType::HILLY_WINTER_FOREST: return {13, 85, 71};

            case map_generator::BiomeType::ROCKY_PEAKS: return {66, 66, 66};
            case map_generator::BiomeType::SNOWY_ROCKS: return {195, 196, 197};

            default:                               return {255, 0, 255};
        }
    }
    
    Color getHeroPathColor(const Hero& hero) const {

        auto type = hero.getHeroType();
        
        if (type == HeroType::ORC)    return {255, 0, 0};
        if (type == HeroType::WOOD_ELF) return {0, 0, 139};
        if (type == HeroType::GNOME)    return {0, 0, 0};
        if (type == HeroType::HUMAN)    return {255, 69, 0};
        
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
        float alpha = 0.75f;

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
    MapRenderer(const std::vector<std::vector<map_generator::PixelCell>>& map) {
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
                map_basePixels[y][x] = getNaturalBiomeColor(map[y][x].biome);
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
