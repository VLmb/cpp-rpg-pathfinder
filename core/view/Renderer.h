// #pragma once

// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include "../map-generation/MapGenerator.h"

// // Подключаем наш генератор, чтобы видеть TerrainType и Enum
// // Предполагаем, что MapGenerator.h лежит в правильном месте
// // #include "../map-generation/MapGenerator.h" 

// struct Color { int r, g, b; };

// // Функция цветов: Мрачная / Реалистичная палитра
// Color GetBiomeColor(BiomeType type) {
//     switch (type) {
//         // Вода: Темно-синяя, почти черная в глубине
//         case WATER:      return {20, 45, 80};   

//         // Песок: Бледный, серовато-бежевый (не желтый!)
//         case SAND:       return {180, 175, 150}; 

//         // Равнина: Темно-оливковая, грязная трава
//         case PLAIN:      return {55, 70, 35};   

//         // Лес: Темно-хвойный
//         case FOREST:     return {25, 50, 25};   

//         // Чаща: Почти черная зелень
//         case DENSE_FOREST: return {10, 30, 15}; 

//         // Болото: Мрачный серо-зеленый
//         case SWAMP:      return {40, 50, 45};   

//         // Горы: Темно-серый камень, сланец
//         case MOUNTAIN:   return {60, 55, 50};   

//         // Снег: Светло-серый (не идеально белый, чтобы не резало глаза)
//         case SNOW:       return {200, 210, 220}; 

//         // Ошибка (Розовый)
//         default:         return {255, 0, 255};   
//     }
// }

// // Принимаем TerrainType, так как именно он используется в GenerateMap
// void SaveMapToImage(const std::vector<std::vector<TerrainType>>& map, const std::string& filename) {
//     int height = map.size();
//     int width = map[0].size();

//     std::ofstream img(filename);
    
//     // PPM Header
//     img << "P3\n" << width << " " << height << "\n255\n";

//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             Color c = GetBiomeColor(map[y][x].biome);
//             img << c.r << " " << c.g << " " << c.b << " ";
//         }
//         img << "\n";
//     }
    
//     img.close();
//     std::cout << "Image saved to " << filename << std::endl;
// }
