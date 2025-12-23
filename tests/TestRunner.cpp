// #pragma once

#include <iostream>

#include "../core/map-generation/Perlin2D.h"
#include "../core/view/MapRenderer.h"
#include "TestGraph.cpp"
#include "TestHashMap.cpp"
#include "TestMapPathfinder.cpp"
#include "TestPq.cpp"

// void runGraphTests();
// void runPqTests();
// void runMapPathfinderTests();
// void runHashMapTests();

int main() {
    std::cout << "========== STARTING ALL TESTS ==========\n";

    runPqTests();
    runGraphTests();
    runHashMapTests();
    runMapPathfinderTests();

    std::cout << "\n========== ALL TESTS FINISHED ==========\n";

    Perlin2D perlin(12345);

    int width = 10 * 12;
    int height = 8 * 12;
    // SaveMapToImage(generateNaturalMap(width, height, perlin), "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/map.ppm");
    
    return 0;
}
