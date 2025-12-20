#include <iostream>

#include "../core/view/Renderer.h"
#include "../core/map-generation/MapGenerator.h"
#include "../core/map-generation/Perlin2D.h"

void runGraphTests();
void runPqTests();

int main() {
    std::cout << "========== STARTING ALL TESTS ==========\n";

    runPqTests();
    runGraphTests();

    std::cout << "\n========== ALL TESTS FINISHED ==========\n";

    Perlin2D perlin(12345);

    SaveMapToImage(GenerateMap(1000, 600, perlin), "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/map.ppm");
    
    return 0;
}