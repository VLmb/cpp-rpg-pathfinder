#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "../map-generation/MapGenerator.h"
#include "../model/Hero.h"
#include "MapColors.h"

class MapRenderer {
public:
    using Color = map_colors::Color;

private:
    int map_width;
    int map_height;

    std::vector<std::vector<Color>> map_basePixels;
    std::vector<std::vector<Color>> map_modifiedPixels;

    static Color getBiomeColor(map_generator::BiomeType type) {
        switch (type) {
            case map_generator::BiomeType::PLAIN: return map_colors::BIOME_PLAIN;
            case map_generator::BiomeType::FOREST: return map_colors::BIOME_FOREST;
            case map_generator::BiomeType::WINTER_PLAIN: return map_colors::BIOME_WINTER_PLAIN;
            case map_generator::BiomeType::WINTER_FOREST: return map_colors::BIOME_WINTER_FOREST;

            case map_generator::BiomeType::HILLS: return map_colors::BIOME_HILLS;
            case map_generator::BiomeType::HILLY_JUNGLE: return map_colors::BIOME_HILLY_JUNGLE;
            case map_generator::BiomeType::WINTER_HILLS: return map_colors::BIOME_WINTER_HILLS;
            case map_generator::BiomeType::HILLY_WINTER_FOREST: return map_colors::BIOME_HILLY_WINTER_FOREST;

            case map_generator::BiomeType::ROCKY_PEAKS: return map_colors::BIOME_ROCKY_PEAKS;
            case map_generator::BiomeType::SNOWY_ROCKS: return map_colors::BIOME_SNOWY_ROCKS;

            default: return map_colors::BIOME_UNKNOWN;
        }
    }

    static Color getHeroPathColor(const Hero& hero) {
        auto type = hero.getHeroType();

        if (type == HeroType::ORC)      return map_colors::HERO_ORC;
        if (type == HeroType::WOOD_ELF) return map_colors::HERO_WOOD_ELF;
        if (type == HeroType::GNOME)    return map_colors::HERO_GNOME;
        if (type == HeroType::HUMAN)    return map_colors::HERO_HUMAN;

        return map_colors::HERO_DEFAULT;
    }

    static Color blendColors(Color base, Color overlay, float alpha) {
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

        map_basePixels.resize(map_height, std::vector<Color>(map_width));

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
        Color borderColor = map_colors::GRAPH_VERTEX_BORDER;
        Color centerColor = map_colors::GRAPH_VERTEX_CENTER;

        int startX = vertice.x;
        int startY = vertice.y;

        for (int dy = 0; dy < vertexSize; ++dy) {
            for (int dx = 0; dx < vertexSize; ++dx) {
                int px = startX + dx;
                int py = startY + dy;

                if (px >= 0 && px < map_width && py >= 0 && py < map_height) {

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

    void cleanMap(std::string filename) {
        map_modifiedPixels = map_basePixels;
        saveModifiedMap(filename);
    }
};
