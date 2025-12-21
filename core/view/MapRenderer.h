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
            case map_generator::BiomeType::WATER: return {20, 45, 80};
            case map_generator::BiomeType::SAND: return {180, 175, 150};
            case map_generator::BiomeType::PLAIN: return {55, 70, 35};
            case map_generator::BiomeType::FOREST: return {25, 50, 25};
            case map_generator::BiomeType::DENSE_FOREST: return {10, 30, 15};
            case map_generator::BiomeType::SWAMP: return {40, 50, 45};
            case map_generator::BiomeType::MOUNTAIN: return {60, 55, 50};
            case map_generator::BiomeType::SNOW: return {200, 210, 220};
            default: return {255, 0, 255};
        }
    }

    Color getHeroPathColor(const Hero& hero) const {

        auto type = hero.getHeroType();
        
        if (type == HeroType::Dwarf)    return {255, 165, 0};   // Оранжевый
        if (type == HeroType::Wood_Elf) return {0, 255, 200};   // Циан
        if (type == HeroType::Human)    return {220, 220, 220}; // Серебряный
        
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
        float alpha = 0.6f; // Прозрачность пути

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
                map_basePixels[y][x] = getBiomeColor(map[y][x].biome);
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
        Color centerColor = {255, 165, 0};

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
};