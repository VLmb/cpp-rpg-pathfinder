cmake_minimum_required(VERSION 3.16)
project(MapPathfinder VERSION 1.0)

set(CMAKE_PREFIX_PATH "D:/Apps/QT/6.10.0/mingw_64")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Подключаем подпапки
add_subdirectory(core)
add_subdirectory(gui)