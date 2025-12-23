#include "PathfinderManager.h"

int main() {
    const std::string filename = "map.ppm";
    PathfinderManager manager(50, 32, filename);
    manager.saveMapToFile();

    Point a = {1, 1};
    Point b = {15, 15};
    Point c = {20, 30};
    Point d = {40, 30};
    Point e = {45, 13};
    Point f = {44, 30};
    Point g = {32, 8};
    //
    manager.addVertexToCheckpointGraph(a);
    manager.addVertexToCheckpointGraph(b);
    manager.addVertexToCheckpointGraph(c);
    manager.removeVertexFromCheckpointGraph(c);
    manager.addVertexToCheckpointGraph(d);
    manager.addVertexToCheckpointGraph(e);
    manager.addVertexToCheckpointGraph(f);
    manager.addVertexToCheckpointGraph(g);

    manager.addEdgeToCheckpointGraph(a, e);
    manager.addEdgeToCheckpointGraph(b, c);
    manager.addEdgeToCheckpointGraph(b, d);
    manager.addEdgeToCheckpointGraph(d, e);
    manager.addEdgeToCheckpointGraph(f, g);
    manager.addEdgeToCheckpointGraph(d, g);
    //
    WoodElf elf;
    Orc orc;
    Human human;
    Gnome gnome;
    //
    // manager.findPathAndDraw(a,e,human,"test.ppm");
    auto t1 = manager.findPathAndDraw(a,e,gnome);
    auto t2 = manager.findPathAndDraw(a,e,gnome, true);
    std::cout << t1 << " " << t2 << std::endl;

    PathfinderManager manager2(10, 10, filename);
    manager2.printGridDigit();


    //
    // manager.renderAndSaveCurrentMap("map3.ppm");
}