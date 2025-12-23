#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include "Perlin2D.h"
#include <iostream>

namespace  map_generator {

    enum class BiomeType {
        PLAIN,
        FOREST,
        WINTER_PLAIN,
        WINTER_FOREST,

        HILLS,
        HILLY_JUNGLE,
        WINTER_HILLS,
        HILLY_WINTER_FOREST,

        ROCKY_PEAKS,
        SNOWY_ROCKS
    };

    struct TerrainType {
        int x;
        int y;

        double temperature;
        double altitude;
        double moisture;

        double castTemperature;
        double castAltitude;
        double castMoisture;

        BiomeType biome;
    };

    inline double getCastTemperature(double temperature) {
        if (temperature > 0.50) {
            return 0.9;
        }
        return 0.1;
    }

    inline double getCastAltitude(double altitude) {
        if (altitude < 0.33) {
            return 0.1;
        }
        if (altitude < 0.75) {
            return 0.5;
        }
        return 0.9;
    }

    inline double getCastMoisture(double moisture) {
        if (moisture > 0.5) {
            return 0.9;
        }
        return 0.1;
    }

    // Логика определения биома
    inline BiomeType getBiomeType(
        double temperature,
        double altitude,
        double moisture) {

        BiomeType type;

         if (altitude < 0.33) {
            if (temperature > 0.50) {
                if (moisture > 0.50) {
                    type = BiomeType::FOREST;
                } else {
                    type = BiomeType::PLAIN;
                }
            } else {
                if (moisture > 0.50) {
                    type = BiomeType::WINTER_FOREST;
                } else {
                    type = BiomeType::WINTER_PLAIN;
                }
            }
        }
        else if (altitude < 0.75) {
            if (temperature > 0.50) {
                if (moisture > 0.50) {
                    type = BiomeType::HILLY_JUNGLE;
                } else {
                    type = BiomeType::HILLS;
                }
            } else {
                if (moisture > 0.50) {
                    type = BiomeType::HILLY_WINTER_FOREST;
                } else {
                    type = BiomeType::WINTER_HILLS;
                }
            }
        }
        else {
            if (temperature > 0.50) {
                if (moisture > 0.50) {
                    type = BiomeType::ROCKY_PEAKS;
                } else {
                    type = BiomeType::ROCKY_PEAKS;
                }
            } else {
                if (moisture > 0.75) {
                    type = BiomeType::SNOWY_ROCKS;
                } else {
                    type = BiomeType::ROCKY_PEAKS;
                }
            }
        }
        return type;
    }

    inline constexpr double SIZE_T1 = 10.0;
    inline constexpr double SIZE_T2 = 30.0;

    inline constexpr double SCALE_T1 = 50.0;
    inline constexpr double SCALE_T2 = 100.0;
    inline constexpr double SCALE_T3 = 150.0;

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

    inline int getMultFromCheckpointToPixels(int width, int height) {
        return getGridScale(width, height) * GRID_FACTOR;
    }

    inline std::vector<std::vector<TerrainType>> generateMap(const int width, const int height, Perlin2D* perlin, double scale = 0.0f) {
        int modifiedWidth = width;
        int modifiedHeight = height;

        calculateParams(modifiedWidth, modifiedHeight, scale);

        std::vector<std::vector<TerrainType>> map(modifiedHeight, std::vector<TerrainType>(modifiedWidth, TerrainType()));

        // Границы для нормализации
        double minT = 1000.0f, maxT = -1000.0f;
        double minA = 1000.0f, maxA = -1000.0f;
        double minM = 1000.0f, maxM = -1000.0f;

        const int OFFSET_R = 0;
        const int OFFSET_V = 5500;
        const int OFFSET_M = 12000;

        // --- ПРОХОД 1 ---
        for (int y = 0; y < modifiedHeight; y++) {
            for (int x = 0; x < modifiedWidth; x++) {
                double tVal = perlin->Noise((x + OFFSET_R) / scale, (y + OFFSET_R) / scale, 3, 0.5f);
                double aVal = perlin->Noise((x + OFFSET_V) / scale, (y + OFFSET_V) / scale, 5, 0.4f);
                double mVal = perlin->Noise((x + OFFSET_M) / scale, (y + OFFSET_M) / scale, 2, 0.5f);

                if (tVal < minT) minT = tVal; if (tVal > maxT) maxT = tVal;
                if (aVal < minA) minA = aVal; if (aVal > maxA) maxA = aVal;
                if (mVal < minM) minM = mVal; if (mVal > maxM) maxM = mVal;

                map[y][x].temperature = tVal;
                map[y][x].altitude = aVal;
                map[y][x].moisture = mVal;
                map[y][x].x = x;
                map[y][x].y = y;
            }
        }

        if (maxT == minT) maxT += 0.001f;
        if (maxA == minA) maxA += 0.001f;
        if (maxM == minM) maxM += 0.001f;

        std::cout << "min and max V " << minA<< ", " << maxA << ", " << std::endl;
        std::cout << "min and max R " << minT<< ", " << maxT << ", " << std::endl;
        std::cout << "min and max M " << minM<< ", " << maxM << ", " << std::endl;

        // --- ПРОХОД 2 ---
        for (int y = 0; y < modifiedHeight; y++) {
            for (int x = 0; x < modifiedWidth; x++) {
                double nT = (map[y][x].temperature - minT) / (maxT - minT);
                double nA = (map[y][x].altitude - minA) / (maxA - minA);
                double nM = (map[y][x].moisture - minM) / (maxM - minM);

                map[y][x].temperature = nT;
                map[y][x].altitude = nA;
                map[y][x].moisture = nM;
                map[y][x].biome = getBiomeType(nT, nA, nM);
                map[y][x].castAltitude = getCastAltitude(nA);
                map[y][x].castTemperature = getCastTemperature(nT);
                map[y][x].castMoisture = getCastTemperature(nM);
            }
        }
        return map;
    }
}
