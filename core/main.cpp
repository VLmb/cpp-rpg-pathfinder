#include "PathfinderManager.h"

int main() {
    const std::string filename = "map.ppm";
    PathfinderManager manager(50, 40, filename);
    manager.saveMapToFile();
    manager.generateCheckpointGraph(5);
    //
    WoodElf elf;
    Orc orc;
    Human human;
    Gnome gnome;
    //
    // manager.findPathAndDraw(a,e,human,"test.ppm");
    // auto t1 = manager.findPathAndDraw(a,e,gnome);
    // manager.findPathAndDraw(a,e,elf);
    // manager.findPathAndDraw(a,e,human);
    // manager.findPathAndDraw(a,e,orc);



    //
    // manager.renderAndSaveCurrentMap("map3.ppm");
}