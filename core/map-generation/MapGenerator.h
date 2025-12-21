#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include "Perlin2D.h"

namespace  map_generator {

    enum class BiomeType {
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
        double rockiness;   // (R)
        double vegetation;  // (V)
        double moisture;    // (M)
        BiomeType biome;
    };

    // Логика определения биома
    inline BiomeType GetBiome(double r, double v, double m) {
        // 1. Уровень воды (объединили глубокую и обычную)
        if (r < 0.30f) return BiomeType::WATER;

        // 2. Горы и Высокогорье
        if (r > 0.75f) {
            if (m > 0.75f) return BiomeType::SNOW;    // Снежные пики
            return BiomeType::MOUNTAIN;               // Голые темные скалы
        }

        // 3. Болота (Низменности с высокой влажностью)
        // r < 0.55 гарантирует, что болота в низинах
        if (m > 0.70f && r < 0.55f) return BiomeType::SWAMP;

        // 4. Лесные массивы
        if (v > 0.75f) return BiomeType::DENSE_FOREST; // Чаща
        if (v > 0.45f) return BiomeType::FOREST;       // Обычный лес

        // 5. Открытые пространства (мало растительности)
        if (m < 0.30f) return BiomeType::SAND;         // Сухо = Песчаная пустошь

        // Дефолт: Равнина (Трава)
        return BiomeType::PLAIN;
    }

    inline constexpr double SIZE_T1 = 10.0;
    inline constexpr double SIZE_T2 = 30.0;

    inline constexpr double SCALE_T1 = 20.0;
    inline constexpr double SCALE_T2 = 30.0;
    inline constexpr double SCALE_T3 = 40.0;

    inline constexpr int FACTOR_T1 = 4;
    inline constexpr int FACTOR_T2 = 3;
    inline constexpr int FACTOR_T3 = 2;
    inline constexpr int GRID_FACTOR = 3;

    inline void calculateParams(int& modifiedWidth, int& modifiedHeight, double& scale) {
        int minSize = std::min(modifiedWidth, modifiedHeight);
        if (minSize < SIZE_T1) {
            scale = (scale == 0.0f) ? SCALE_T1 : scale;
            modifiedWidth *= FACTOR_T1 * GRID_FACTOR;
            modifiedHeight *= FACTOR_T1 * GRID_FACTOR;
        } else if (minSize < SIZE_T2) {
            scale = (scale == 0.0f) ? SCALE_T2 : scale;
            modifiedWidth *= FACTOR_T2 * GRID_FACTOR;
            modifiedHeight *= FACTOR_T2 * GRID_FACTOR;
        } else {
            scale = (scale == 0.0f) ? SCALE_T3 : scale;
            modifiedWidth *= FACTOR_T3 * GRID_FACTOR;
            modifiedHeight *= FACTOR_T3 * GRID_FACTOR;
        }
    }

    inline int getGridScale(int width, int height) {
        int minSize = std::min(width, height);
        if (minSize < SIZE_T1) {
            return FACTOR_T1;
        }
        if (minSize < SIZE_T2) {
            return FACTOR_T2;
        }

        return FACTOR_T3;
    }

    inline std::vector<std::vector<TerrainType>> GenerateMap(const int width, const int height, Perlin2D* perlin, double scale = 0.0f) {
        int modifiedWidth = width;
        int modifiedHeight = height;

        calculateParams(modifiedWidth, modifiedHeight, scale);

        std::vector<std::vector<TerrainType>> map(modifiedHeight, std::vector<TerrainType>(modifiedWidth, TerrainType()));

        // Границы для нормализации
        double minR = 1000.0f, maxR = -1000.0f;
        double minV = 1000.0f, maxV = -1000.0f;
        double minM = 1000.0f, maxM = -1000.0f;

        const int OFFSET_R = 0;
        const int OFFSET_V = 5200;
        const int OFFSET_M = 13900;

        // --- ПРОХОД 1 ---
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double rVal = perlin->Noise((x + OFFSET_R) / scale, (y + OFFSET_R) / scale, 4, 0.5f);
                double vVal = perlin->Noise((x + OFFSET_V) / scale, (y + OFFSET_V) / scale, 3, 0.4f);
                double mVal = perlin->Noise((x + OFFSET_M) / scale, (y + OFFSET_M) / scale, 2, 0.5f);

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
                double nR = (map[y][x].rockiness - minR) / (maxR - minR);
                double nV = (map[y][x].vegetation - minV) / (maxV - minV);
                double nM = (map[y][x].moisture - minM) / (maxM - minM);

                map[y][x].rockiness = nR;
                map[y][x].vegetation = nV;
                map[y][x].moisture = nM;
                map[y][x].biome = GetBiome(nR, nV, nM);
            }
        }
        return map;
    }
}
