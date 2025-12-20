#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Perlin2D.h"

enum BiomeType { 
    WATER, 
    SAND,          // Бледный пляж / Пустошь
    PLAIN,         // Темная трава / Равнина
    FOREST,        // Лес
    DENSE_FOREST,  // Густая чаща
    SWAMP,         // Топи
    MOUNTAIN,      // Скалы
    SNOW           // Снег
};

struct TerrainType {
    int x;
    int y;
    float rockiness;   // (R)
    float vegetation;  // (V)
    float moisture;    // (M)
    BiomeType biome;
};

// Логика определения биома
inline BiomeType GetBiome(float r, float v, float m) {
    // 1. Уровень воды (объединили глубокую и обычную)
    if (r < 0.30f) return WATER;

    // 2. Горы и Высокогорье
    if (r > 0.75f) {
        if (m > 0.75f) return SNOW;    // Снежные пики
        return MOUNTAIN;               // Голые темные скалы
    }

    // 3. Болота (Низменности с высокой влажностью)
    // r < 0.55 гарантирует, что болота в низинах
    if (m > 0.70f && r < 0.55f) return SWAMP;

    // 4. Лесные массивы
    if (v > 0.75f) return DENSE_FOREST; // Чаща
    if (v > 0.45f) return FOREST;       // Обычный лес

    // 5. Открытые пространства (мало растительности)
    if (m < 0.30f) return SAND;         // Сухо = Песчаная пустошь

    // Дефолт: Равнина (Трава)
    return PLAIN; 
}

inline std::vector<std::vector<TerrainType>> GenerateMap(int width, int height, Perlin2D& perlin) {
    std::vector<std::vector<TerrainType>> map(height, std::vector<TerrainType>(width));
    
    float scale = 40.0f; 

    // Границы для нормализации
    float minR = 1000.0f, maxR = -1000.0f;
    float minV = 1000.0f, maxV = -1000.0f;
    float minM = 1000.0f, maxM = -1000.0f;

    const int OFFSET_R = 0;
    const int OFFSET_V = 5200;
    const int OFFSET_M = 13900;

    // --- ПРОХОД 1 ---
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float rVal = perlin.Noise((x + OFFSET_R) / scale, (y + OFFSET_R) / scale, 4, 0.5f);
            float vVal = perlin.Noise((x + OFFSET_V) / scale, (y + OFFSET_V) / scale, 3, 0.4f);
            float mVal = perlin.Noise((x + OFFSET_M) / scale, (y + OFFSET_M) / scale, 2, 0.5f);

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

    if (maxR == minR) maxR += 0.001f;
    if (maxV == minV) maxV += 0.001f;
    if (maxM == minM) maxM += 0.001f;

    // --- ПРОХОД 2 ---
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float nR = (map[y][x].rockiness - minR) / (maxR - minR);
            float nV = (map[y][x].vegetation - minV) / (maxV - minV);
            float nM = (map[y][x].moisture - minM) / (maxM - minM);

            map[y][x].rockiness = nR;
            map[y][x].vegetation = nV;
            map[y][x].moisture = nM;
            map[y][x].biome = GetBiome(nR, nV, nM);
        }
    }
    return map;
}
