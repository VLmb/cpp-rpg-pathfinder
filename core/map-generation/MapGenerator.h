#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include "Perlin2D.h"
#include "MapConfig.h"

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

    struct PixelCell {
        double temperature;
        double altitude;
        double moisture;

        BiomeType biome;
    };

    inline double getCastTemperature(double temperature) {
        if (temperature > map_generator_config::SIMPLIFIED_TEMPERATURE_THRESHOLD) {
            return map_generator_config::SIMPLIFIED_HIGH_VALUE;
        }
        return map_generator_config::SIMPLIFIED_LOW_VALUE;
    }

    inline double getCastAltitude(double altitude) {
        if (altitude < map_generator_config::SIMPLIFIED_ALTITUDE_LOW_THRESHOLD) {
            return map_generator_config::SIMPLIFIED_LOW_VALUE;
        }
        if (altitude < map_generator_config::SIMPLIFIED_ALTITUDE_MID_THRESHOLD) {
            return map_generator_config::SIMPLIFIED_MID_VALUE;
        }
        return map_generator_config::SIMPLIFIED_HIGH_VALUE;
    }

    inline double getCastMoisture(double moisture) {
        if (moisture > map_generator_config::SIMPLIFIED_MOISTURE_THRESHOLD) {
            return map_generator_config::SIMPLIFIED_HIGH_VALUE;
        }
        return map_generator_config::SIMPLIFIED_LOW_VALUE;
    }

    inline BiomeType getBiomeType(
        double temperature,
        double altitude,
        double moisture) {

        BiomeType type;

         if (altitude < map_generator_config::ALTITUDE_LOW_THRESHOLD) {
            if (temperature > map_generator_config::TEMPERATURE_WARM_THRESHOLD) {
                if (moisture > map_generator_config::MOISTURE_WET_THRESHOLD) {
                    type = BiomeType::FOREST;
                } else {
                    type = BiomeType::PLAIN;
                }
            } else {
                if (moisture > map_generator_config::MOISTURE_WET_THRESHOLD) {
                    type = BiomeType::WINTER_FOREST;
                } else {
                    type = BiomeType::WINTER_PLAIN;
                }
            }
        }
        else if (altitude < map_generator_config::ALTITUDE_MID_THRESHOLD) {
            if (temperature > map_generator_config::TEMPERATURE_WARM_THRESHOLD) {
                if (moisture > map_generator_config::MOISTURE_WET_THRESHOLD) {
                    type = BiomeType::HILLY_JUNGLE;
                } else {
                    type = BiomeType::HILLS;
                }
            } else {
                if (moisture > map_generator_config::MOISTURE_WET_THRESHOLD) {
                    type = BiomeType::HILLY_WINTER_FOREST;
                } else {
                    type = BiomeType::WINTER_HILLS;
                }
            }
        }
        else {
            if (temperature > map_generator_config::TEMPERATURE_WARM_THRESHOLD) {
                if (moisture > map_generator_config::MOISTURE_WET_THRESHOLD) {
                    type = BiomeType::ROCKY_PEAKS;
                } else {
                    type = BiomeType::ROCKY_PEAKS;
                }
            } else {
                if (moisture > map_generator_config::MOISTURE_SNOWY_ROCKS_THRESHOLD) {
                    type = BiomeType::SNOWY_ROCKS;
                } else {
                    type = BiomeType::ROCKY_PEAKS;
                }
            }
        }
        return type;
    }

    inline constexpr double SIZE_T1 = map_generator_config::SIZE_THRESHOLD_SMALL;
    inline constexpr double SIZE_T2 = map_generator_config::SIZE_THRESHOLD_MEDIUM;

    inline constexpr double SCALE_T1 = map_generator_config::DEFAULT_SCALE_SMALL;
    inline constexpr double SCALE_T2 = map_generator_config::DEFAULT_SCALE_MEDIUM;
    inline constexpr double SCALE_T3 = map_generator_config::DEFAULT_SCALE_LARGE;

    inline constexpr int FACTOR_T1 = map_generator_config::PIXEL_UPSCALE_SMALL;
    inline constexpr int FACTOR_T2 = map_generator_config::PIXEL_UPSCALE_MEDIUM;
    inline constexpr int FACTOR_T3 = map_generator_config::PIXEL_UPSCALE_LARGE;
    inline constexpr int GRID_FACTOR = map_generator_config::GRID_BLOCK_SIZE;

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

    inline const auto& normalize = [](double val, double mmin, double mmax) -> double {
            return (val - mmin) / (mmax - mmin);
        };

    inline std::vector<std::vector<PixelCell>> generateNaturalMap(const int width, const int height, Perlin2D* perlin, double scale = 0.0f) {
        int modifiedWidth = width;
        int modifiedHeight = height;

        calculateParams(modifiedWidth, modifiedHeight, scale);

        std::vector<std::vector<PixelCell>> map(modifiedHeight, std::vector<PixelCell>(modifiedWidth, PixelCell()));

        double minT = 1000.0f, maxT = -1000.0f;
        double minA = 1000.0f, maxA = -1000.0f;
        double minM = 1000.0f, maxM = -1000.0f;

        const int OFFSET_R = map_generator_config::OFFSET_TEMPERATURE;
        const int OFFSET_V = map_generator_config::OFFSET_ALTITUDE;
        const int OFFSET_M = map_generator_config::OFFSET_MOISTURE;

        for (int y = 0; y < modifiedHeight; y++) {
            for (int x = 0; x < modifiedWidth; x++) {
                double tVal = perlin->Noise((x + OFFSET_R) / scale, (y + OFFSET_R) / scale, map_generator_config::TEMP_OCTAVES, map_generator_config::TEMP_PERSISTENCE);
                double aVal = perlin->Noise((x + OFFSET_V) / scale, (y + OFFSET_V) / scale, map_generator_config::ALT_OCTAVES, map_generator_config::ALT_PERSISTENCE);
                double mVal = perlin->Noise((x + OFFSET_M) / scale, (y + OFFSET_M) / scale, map_generator_config::MOIST_OCTAVES, map_generator_config::MOIST_PERSISTENCE);

                if (tVal < minT) minT = tVal; if (tVal > maxT) maxT = tVal;
                if (aVal < minA) minA = aVal; if (aVal > maxA) maxA = aVal;
                if (mVal < minM) minM = mVal; if (mVal > maxM) maxM = mVal;

                map[y][x].temperature = tVal;
                map[y][x].altitude = aVal;
                map[y][x].moisture = mVal;
            }
        }

        if (maxT == minT) maxT += map_generator_config::NORMALIZE_EPS;
        if (maxA == minA) maxA += map_generator_config::NORMALIZE_EPS;
        if (maxM == minM) maxM += map_generator_config::NORMALIZE_EPS;

        for (int y = 0; y < modifiedHeight; y++) {
            for (int x = 0; x < modifiedWidth; x++) {
                double nT = normalize(map[y][x].temperature, minT, maxT);
                double nA = normalize(map[y][x].altitude, minA, maxA);
                double nM = normalize(map[y][x].moisture, minM, maxM);

                map[y][x].temperature = nT;
                map[y][x].altitude = nA;
                map[y][x].moisture = nM;
                map[y][x].biome = getBiomeType(nT, nA, nM);
            }
        }
        return map;
    }

    inline std::vector<std::vector<PixelCell>> generateSimplifiedMap(const int width, const int height, Perlin2D* perlin, double scale = 0.0f) {
        int modifiedWidth = width;
        int modifiedHeight = height;

        calculateParams(modifiedWidth, modifiedHeight, scale);

        std::vector<std::vector<PixelCell>> map(modifiedHeight, std::vector<PixelCell>(modifiedWidth, PixelCell()));

        double mmin = 1000.0f, mmax = -1000.0f;

        std::vector<std::vector<double>> castMap(modifiedHeight, std::vector<double>(modifiedWidth));
        for (int y = 0; y < modifiedHeight; y++) {
            for (int x = 0; x < modifiedWidth; x++) {
                double val = perlin->Noise(
                    x / (scale * map_generator_config::SIMPLIFIED_SCALE_X_MULT),
                    y / (scale * map_generator_config::SIMPLIFIED_SCALE_Y_MULT),
                    map_generator_config::SIMPLIFIED_OCTAVES,
                    map_generator_config::SIMPLIFIED_PERSISTENCE
                );

                mmin = std::min(mmin, val);
                mmax = std::max(mmax, val);

                castMap[y][x] = val;
            }
        }

        if (mmin == mmax) {
            mmax += map_generator_config::NORMALIZE_EPS;
        }

        for (int y = 0; y < modifiedHeight; y++) {
            for (int x = 0; x < modifiedWidth; x++) {
                castMap[y][x] = normalize(castMap[y][x], mmin, mmax);

                if (castMap[y][x] > map_generator_config::SIMPLIFIED_CLASS_HIGH_THRESHOLD) {
                    map[y][x].temperature = 1.0;
                    map[y][x].altitude = 0.1;
                    map[y][x].moisture = 0.1;
                    map[y][x].biome = getBiomeType(
                        map[y][x].temperature,
                        map[y][x].altitude,
                        map[y][x].moisture
                        );
                } else if (castMap[y][x] > map_generator_config::SIMPLIFIED_CLASS_MID_THRESHOLD) {
                    map[y][x].temperature = 0.1;
                    map[y][x].altitude = 1.0;
                    map[y][x].moisture = 0.1;
                    map[y][x].biome = getBiomeType(
                        map[y][x].temperature,
                        map[y][x].altitude,
                        map[y][x].moisture
                        );
                } else {
                    map[y][x].temperature = 0.1;
                    map[y][x].altitude = 0.1;
                    map[y][x].moisture = 1.0;
                    map[y][x].biome = getBiomeType(
                        map[y][x].temperature,
                        map[y][x].altitude,
                        map[y][x].moisture
                        );
                }
            }
        }
        return map;
    }
}
