#pragma once

#include <vector>
#include <iostream>
#include <algorithm> // для std::max, std::min
#include <cmath>     // для защиты от деления на ноль

#include "Perlin2D.h"

// Добавил PLAIN (Равнина), так как SAND (Песок) — это слишком специфично
enum BiomeType { 
    DEEP_WATER, 
    WATER, 
    SAND,      // Пустыня / Пляж
    PLAIN,     // Трава / Равнина (легко ходить)
    FOREST,    // Лес
    DENSE_FOREST, // Густой лес / Джунгли
    SWAMP,     // Болото
    MOUNTAIN,  // Горы
    SNOW       // Снежные пики
};

struct TerrainType {
    int x;
    int y;
    
    float rockiness;   // (R) Рельеф, высота, камни
    float vegetation;  // (V) Плотность леса, кустов
    float moisture;    // (M) Влажность, болотистость, вязкость

    BiomeType biome;   // Визуальный тип поверхности
};

// Логика определения биома по 3 параметрам
BiomeType GetBiome(float r, float v, float m) {
    // 1. Уровень моря (определяется Rockiness/Height)
    if (r < 0.25f) return DEEP_WATER;
    if (r < 0.35f) return WATER;

    // 2. Зоны высокой скалистости (Горы)
    if (r > 0.80f) {
        if (m > 0.7f) return SNOW;    // Влажно и высоко = Снег
        return MOUNTAIN;              // Сухо и высоко = Голые скалы
    }

    // 3. Болотистые зоны (Низкая скалистость, высокая влажность)
    // r < 0.6 гарантирует, что болота не заберутся на высокие холмы
    if (m > 0.75f && r < 0.6f) return SWAMP;

    // 4. Лесные зоны (Зависят от Растительности)
    if (v > 0.8f) return DENSE_FOREST; // Джунгли/Чаща
    if (v > 0.5f) return FOREST;       // Обычный лес

    // 5. Открытые пространства (мало растительности)
    if (m < 0.25f) return SAND;        // Сухо и пусто = Пустыня
    if (r < 0.45f && m > 0.4f) return PLAIN; // Равнина у воды

    // Дефолт (каменистая равнина или редкий лес)
    return PLAIN; 
}

std::vector<std::vector<TerrainType>> GenerateMap(int width, int height, Perlin2D& perlin) {
    std::vector<std::vector<TerrainType>> map(height, std::vector<TerrainType>(width));
    
    // Масштаб можно вынести в аргументы. 
    // Чем больше, тем крупнее пятна лесов и гор.
    float scale = 40.0f; 

    // Границы для нормализации
    float minR = 1000.0f, maxR = -1000.0f;
    float minV = 1000.0f, maxV = -1000.0f;
    float minM = 1000.0f, maxM = -1000.0f;

    // --- ПРОХОД 1: Генерация сырых данных ---
    // Используем разные смещения (offsets), чтобы R, V и M были независимы.
    // Если не делать смещение, то там где Гора (R), там всегда будет и Лес (V), и Вода (M).
    
    const int OFFSET_R = 0;
    const int OFFSET_V = 5200;  // Просто большое число
    const int OFFSET_M = 13900; // Другое большое число

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Rockiness
            float rVal = perlin.Noise((x + OFFSET_R) / scale, (y + OFFSET_R) / scale, 2, 0.5f);
            
            // Vegetation
            float vVal = perlin.Noise((x + OFFSET_V) / scale, (y + OFFSET_V) / scale, 2, 0.4f);
            
            // Moisture (2 октавы, влажность меняется плавно)
            float mVal = perlin.Noise((x + OFFSET_M) / scale, (y + OFFSET_M) / scale, 2, 0.5f);

            // Обновляем Min/Max
            if (rVal < minR) minR = rVal; if (rVal > maxR) maxR = rVal;
            if (vVal < minV) minV = vVal; if (vVal > maxV) maxV = vVal;
            if (mVal < minM) minM = mVal; if (mVal > maxM) maxM = mVal;

            map[y][x].rockiness = rVal;
            map[y][x].vegetation = vVal;
            map[y][x].moisture = mVal;
            map[y][x].x = x;
            map[y][x].y = y;
        }
    }

    // Защита от деления на 0
    if (maxR == minR) maxR += 0.001f;
    if (maxV == minV) maxV += 0.001f;
    if (maxM == minM) maxM += 0.001f;

    // --- ПРОХОД 2: Нормализация и Биомы ---
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Приводим всё к диапазону [0.0 ... 1.0]
            float nR = (map[y][x].rockiness - minR) / (maxR - minR);
            float nV = (map[y][x].vegetation - minV) / (maxV - minV);
            float nM = (map[y][x].moisture - minM) / (maxM - minM);

            map[y][x].rockiness = nR;
            map[y][x].vegetation = nV;
            map[y][x].moisture = nM;

            map[y][x].biome = GetBiome(nR, nV, nM);
        }
    }

    std::cout << "Map generated with 3 layers (Rock, Veg, Moist).\n";
    return map;
}