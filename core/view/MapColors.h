#pragma once

namespace map_colors {

    struct Color {
        int r, g, b;
    };

    inline constexpr Color BIOME_PLAIN {135, 171, 104};
    inline constexpr Color BIOME_FOREST {0, 140, 0};
    inline constexpr Color BIOME_WINTER_PLAIN {104, 171, 155};
    inline constexpr Color BIOME_WINTER_FOREST {18, 127, 91};

    inline constexpr Color BIOME_HILLS {92, 116, 67};
    inline constexpr Color BIOME_HILLY_JUNGLE {0, 100, 0};
    inline constexpr Color BIOME_WINTER_HILLS {72, 116, 104};
    inline constexpr Color BIOME_HILLY_WINTER_FOREST {13, 85, 71};

    inline constexpr Color BIOME_ROCKY_PEAKS {66, 66, 66};
    inline constexpr Color BIOME_SNOWY_ROCKS {195, 196, 197};

    inline constexpr Color BIOME_UNKNOWN {0, 0, 0};

    inline constexpr Color HERO_ORC {255, 0, 0};
    inline constexpr Color HERO_WOOD_ELF {0, 0, 139};
    inline constexpr Color HERO_GNOME {0, 0, 0};
    inline constexpr Color HERO_HUMAN {255, 69, 0};

    inline constexpr Color HERO_DEFAULT {255, 50, 50};

    inline constexpr Color GRAPH_VERTEX_BORDER {255, 0, 0};
    inline constexpr Color GRAPH_VERTEX_CENTER {255, 255, 0};
}
