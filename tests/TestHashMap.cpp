#pragma once

#include <iostream>
#include <string>

#include "../core/data-structure/HashMap.h"
#include "TestUtils.h"

void testHashMapPutGet() {
    std::cout << "\n--- Running testHashMapPutGet ---\n";
    HashMap<std::string, int> map;

    bool inserted = map.put("a", 1);
    auto* valPtr = map.getPtr("a");

    assert_custom(inserted, "Insert new key returns true");
    assert_custom(valPtr && *valPtr == 1, "GetPtr returns stored value");
    assert_custom(map.size() == 1, "Size is 1 after one insert");
}

void testHashMapOverwriteAndContains() {
    std::cout << "\n--- Running testHashMapOverwriteAndContains ---\n";
    HashMap<std::string, int> map;

    map.put("k1", 5);
    bool insertedAgain = map.put("k1", 10);
    auto* valPtr = map.getPtr("k1");

    assert_custom(!insertedAgain, "Overwriting returns false");
    assert_custom(valPtr && *valPtr == 10, "Overwritten value updated");
    assert_custom(map.contains("k1"), "Contains finds existing key");
}

void testHashMapRemove() {
    std::cout << "\n--- Running testHashMapRemove ---\n";
    HashMap<std::string, int> map;
    map.put("keep", 1);
    map.put("drop", 2);

    bool removed = map.remove("drop");
    bool removedMissing = map.remove("missing");

    assert_custom(removed, "Remove existing key returns true");
    assert_custom(!removedMissing, "Remove missing key returns false");
    assert_custom(!map.contains("drop"), "Removed key no longer present");
    assert_custom(map.size() == 1, "Size reflects removal");
}

void runHashMapTests() {
    std::cout << "\n=== Running HashMap Tests ===\n";
    testHashMapPutGet();
    testHashMapOverwriteAndContains();
    testHashMapRemove();
}
