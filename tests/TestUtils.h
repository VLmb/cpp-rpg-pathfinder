#pragma once
#include <iostream>
#include <string>

// inline позволяет включать эту функцию в несколько файлов без ошибок линковки
inline void assert_custom(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASSED] " << testName << "\n";
    } else {
        std::cout << "!!! [FAILED] " << testName << "\n";
    }
}