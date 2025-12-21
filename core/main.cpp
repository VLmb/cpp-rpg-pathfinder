#include "PathfinderManager.h"

int main() {
    PathfinderManager manager(25, 16);

    Point a = {1, 1};
    Point b = {15, 15};
    Point c = {12, 8};
    Point d = {20, 1};
    Point e = {22, 15};

    manager.addVertexToCheckpointGraph(a);
    manager.addVertexToCheckpointGraph(b);
    manager.addVertexToCheckpointGraph(c);
    manager.addVertexToCheckpointGraph(d);
    manager.addVertexToCheckpointGraph(e);

    manager.addEdgeToCheckpointGraph(a, b);
    manager.addEdgeToCheckpointGraph(b, c);
    manager.addEdgeToCheckpointGraph(c, d);
    manager.addEdgeToCheckpointGraph(d, e);

    WoodElf elf;
    Dwarf dwarf;
    Human human;

    manager.findPathAndDraw(a,e,elf,"map-with-path.ppm");
    manager.findPathAndDraw(a,e,dwarf,"map-with-path.ppm");
    manager.findPathAndDraw(a,e,human,"map-with-path.ppm");

    manager.renderAndSaveCurrentMap("map.ppm");
}