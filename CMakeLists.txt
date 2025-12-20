cmake_minimum_required(VERSION 3.14)
project(GraphRPG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматический поиск Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Подключаем подпапки
add_subdirectory(core)
add_subdirectory(app)