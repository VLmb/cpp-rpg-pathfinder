#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Graph.h" 
#include "TestUtils.h"

void testVertices() {
    std::cout << "\n--- Running testVertices ---\n";
    Graph g(10, 10); 

    Point validP = { 5, 5 };
    Point invalidP = { 15, 5 };

    bool added = g.addVertex(validP);
    assert_custom(added == true, "Add valid vertex");

    bool addedInvalid = g.addVertex(invalidP);
    assert_custom(addedInvalid == false, "Add out-of-bounds vertex should fail");

    assert_custom(g.addVertex(validP), "Add existing vertex again");

    bool removed = g.removeVertex(validP);
    assert_custom(removed == true, "Remove existing vertex");

    bool removedAgain = g.removeVertex(validP);
    assert_custom(removedAgain == true, "Remove non-existing vertex (check logic)"); 
}

void testEdges() {
    std::cout << "\n--- Running testEdges ---\n";
    Graph g(5, 5);

    Point p1 = { 0, 0 };
    Point p2 = { 0, 1 };
    Point p3 = { 2, 2 };

    g.addVertex(p1);
    g.addVertex(p2);

    bool edgeAdded = g.addEdge(p1, p2);
    assert_custom(edgeAdded == true, "Add edge between valid vertices");
    assert_custom(g.hasEdge(p1, p2), "Check hasEdge matches");
    assert_custom(g.hasEdge(p2, p1), "Check undirected edge (reverse)");


    bool badEdge = g.addEdge(p1, p3);
    assert_custom(badEdge == false, "Add edge to non-vertex should fail");

    bool edgeRemoved = g.removeEdge(p1, p2);
    assert_custom(edgeRemoved == true, "Remove existing edge");
    assert_custom(!g.hasEdge(p1, p2), "Edge is gone");
}

void testNeighborsAndCleanup() {
    std::cout << "\n--- Running testNeighborsAndCleanup ---\n";
    Graph g(4, 4);

    Point top = { 1, 0 };
    Point left = { 0, 1 };
    Point right = { 2, 1 };

    g.addVertex(top);
    g.addVertex(left);
    g.addVertex(right);

    g.addEdge(top, left);
    g.addEdge(top, right);

    std::vector<Point> nbs;
    g.neighbors(top, nbs);
    
    bool countCheck = (nbs.size() == 2); 
    assert_custom(countCheck, "Vertex 'top' has 2 neighbors");

    bool hasLeft = false, hasRight = false;
    for(auto p : nbs) {
        if(p.x == left.x && p.y == left.y) hasLeft = true;
        if(p.x == right.x && p.y == right.y) hasRight = true;
    }
    assert_custom(hasLeft && hasRight, "Neighbors contents are correct");

    g.removeVertex(left);
    
    assert_custom(!g.hasEdge(top, left), "Edge top-left removed automatically");
    
    std::vector<Point> nbsAfter;
    g.neighbors(top, nbsAfter);
    assert_custom(nbsAfter.size() == 1, "Top now has only 1 neighbor");
}

void runGraphTests() {
    std::cout << "\n=== Running Graph Tests ===\n";
    testVertices();
    testEdges();
    testNeighborsAndCleanup();
}