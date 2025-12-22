find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Создаем исполняемый файл приложения
qt_add_executable(MapPathfinderApp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# Подключаем вашу библиотеку CoreLib
target_link_libraries(MapPathfinderApp PRIVATE Qt6::Widgets CoreLogic)