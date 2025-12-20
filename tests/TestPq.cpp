#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../core/data-structure/PriorityQueue.h"
#include "TestUtils.h"

void testIntegers() {
    std::cout << "\n--- Running testIntegers ---\n";
    PriorityQueue<int, int> pq;
    
    pq.enqueue(10, 50);
    pq.enqueue(20, 10);
    pq.enqueue(30, 30);
    pq.enqueue(40, 5);  // Самый маленький приоритет

    assert_custom(pq.count() == 4, "Count check");
    assert_custom(!pq.isEmpty(), "Is not empty check");

    int val1 = pq.extractMin();
    assert_custom(val1 == 40, "Extract 1st (priority 5)");

    int val2 = pq.extractMin();
    assert_custom(val2 == 20, "Extract 2nd (priority 10)");

    int val3 = pq.extractMin();
    assert_custom(val3 == 30, "Extract 3rd (priority 30)");

    int val4 = pq.extractMin();
    assert_custom(val4 == 10, "Extract 4th (priority 50)");

    assert_custom(pq.isEmpty(), "Queue is empty at the end");
}

struct TestState {
    int id;
    std::string name;
};

void testComplexStructs() {
    std::cout << "\n--- Running testComplexStructs ---\n";
    PriorityQueue<TestState, double> pq;

    pq.enqueue({1, "Start"}, 10.5);
    pq.enqueue({2, "Finish"}, 2.1); // min
    pq.enqueue({3, "Middle"}, 5.5);

    TestState res1 = pq.extractMin();
    assert_custom(res1.id == 2 && res1.name == "Finish", "Struct extract 1st");
    
    TestState res2 = pq.extractMin();
    assert_custom(res2.id == 3, "Struct extract 2nd");
    
    TestState res3 = pq.extractMin();
    assert_custom(res3.id == 1, "Struct extract 3rd");
}

void testExceptions() {
    std::cout << "\n--- Running testExceptions ---\n";
    PriorityQueue<int, int> pq;
    bool caught = false;

    try {
        pq.extractMin();
    } catch (const std::out_of_range& e) {
        caught = true;
    } catch (...) {
        // Не тот тип исключения
    }

    assert_custom(caught, "ExtractMin throws out_of_range on empty");
}

void runPqTests() {
    std::cout << "\n=== Running PriorityQueue Tests ===\n";
    testIntegers();
    testComplexStructs();
}