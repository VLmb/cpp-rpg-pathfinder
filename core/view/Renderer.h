#pragma once

#include <iostream>
#include <fstream>

#include "../map-generation/MapGenerator.h"

struct Color { int r, g, b; };

// Функция, которая сопоставляет биом с цветом
Color GetBiomeColor(BiomeType type) {
    switch (type) {
        case DEEP_WATER: return {0, 0, 128};     // Темно-синий
        case WATER:      return {0, 100, 255};   // Синий
        case SAND:       return {240, 240, 64};  // Песочный
        case FOREST:     return {34, 139, 34};   // Лесной зеленый
        case SWAMP:      return {47, 79, 79};    // Болотный
        case MOUNTAIN:   return {139, 69, 19};   // Коричневый/Серый
        case SNOW:       return {255, 255, 255}; // Белый
        default:         return {255, 0, 255};   // Розовый (ошибка)
    }
}

void SaveMapToImage(const std::vector<std::vector<Tile>>& map, const std::string& filename) {
    int height = map.size();
    int width = map[0].size();

    std::ofstream img(filename);
    
    // Заголовок PPM: P3, ширина, высота, макс. яркость
    img << "P3\n" << width << " " << height << "\n255\n";

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color c = GetBiomeColor(map[y][x].biome);
            // Пишем R G B через пробел
            img << c.r << " " << c.g << " " << c.b << " ";
        }
        img << "\n";
    }
    
    img.close();
    std::cout << "Image saved to " << filename << std::endl;
}