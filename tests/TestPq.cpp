#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include "../core/data-structure/PriorityQueue.h"
#include "TestUtils.h"

void testIntegers() {
    std::cout << "\n--- Running testIntegers ---\n";

    PriorityQueue<int> pq([](const int& a, const int& b) -> bool {
        return a < b;
    });

    pq.enqueue(10);
    pq.enqueue(20);
    pq.enqueue(30);
    pq.enqueue(5);  
    pq.enqueue(40);

    assert_custom(pq.count() == 5, "Count check");
    assert_custom(!pq.isEmpty(), "Is not empty check");

    int v1 = pq.extractMin();
    assert_custom(v1 == 5, "Extract 1st (min value 5)");

    int v2 = pq.extractMin();
    assert_custom(v2 == 10, "Extract 2nd (value 10)");

    int v3 = pq.extractMin();
    assert_custom(v3 == 20, "Extract 3rd (value 20)");

    int v4 = pq.extractMin();
    assert_custom(v4 == 30, "Extract 4th (value 30)");

    int v5 = pq.extractMin();
    assert_custom(v5 == 40, "Extract 5th (value 40)");

    assert_custom(pq.isEmpty(), "Queue is empty at the end");
}

struct TestState {
    int id;
    std::string name;
};

struct StateWithPriority {
    TestState state;
    double priority;
};

void testComplexStructs() {
    std::cout << "\n--- Running testComplexStructs ---\n";

    PriorityQueue<StateWithPriority> pq([](const StateWithPriority& a, const StateWithPriority& b) -> bool {
        return a.priority < b.priority;
    });

    pq.enqueue(StateWithPriority{ TestState{1, "Start"}, 10.5 });
    pq.enqueue(StateWithPriority{ TestState{2, "Finish"}, 2.1 });  // min
    pq.enqueue(StateWithPriority{ TestState{3, "Middle"}, 5.5 });

    StateWithPriority r1 = pq.extractMin();
    assert_custom(r1.state.id == 2 && r1.state.name == "Finish", "Struct extract 1st (priority 2.1)");

    StateWithPriority r2 = pq.extractMin();
    assert_custom(r2.state.id == 3 && r2.priority == 5.5, "Struct extract 2nd (priority 5.5)");

    StateWithPriority r3 = pq.extractMin();
    assert_custom(r3.state.id == 1 && r3.priority == 10.5, "Struct extract 3rd (priority 10.5)");

    assert_custom(pq.isEmpty(), "Queue is empty at the end");
}

void testExceptions() {
    std::cout << "\n--- Running testExceptions ---\n";

    PriorityQueue<int> pq;
    bool caught = false;

    try {
        pq.extractMin();
    } catch (const std::out_of_range&) {
        caught = true;
    } catch (...) {
        // не тот тип исключения
    }

    assert_custom(caught, "ExtractMin throws out_of_range on empty");
}

void runPqTests() {
    std::cout << "\n=== Running PriorityQueue Tests ===\n";
    testIntegers();
    testComplexStructs();
    testExceptions();
}
