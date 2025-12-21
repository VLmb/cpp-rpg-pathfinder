#pragma once

#include <iostream>

#include "../core/view/Renderer.h"
#include "../core/map-generation/MapGenerator.h"
#include "../core/map-generation/Perlin2D.h"
// #include "TestPq.cpp"
// #include "TestAStar.cpp"
// #include "TestGraph.cpp"

void runGraphTests();
void runPqTests();
void runAStarTests();

int main() {
    std::cout << "========== STARTING ALL TESTS ==========\n";

    runPqTests();
    runGraphTests();
    runAStarTests();

    std::cout << "\n========== ALL TESTS FINISHED ==========\n";

    Perlin2D perlin(12345);

    int width = 10 * 12;
    int height = 8 * 12;
    SaveMapToImage(GenerateMap(width, height, perlin), "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/map.ppm");
    
    return 0;
}
