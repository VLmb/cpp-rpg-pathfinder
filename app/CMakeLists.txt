find_package(Qt5 COMPONENTS Widgets Required)
# Или Qt6, если у тебя новая версия:
# find_package(Qt6 COMPONENTS Widgets Required)

add_executable(GraphRPGApp
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
)

# Линкуем библиотеку CoreLogic и Qt
target_link_libraries(GraphRPGApp PRIVATE Qt5::Widgets CoreLogic)
# Для Qt6: target_link_libraries(GraphRPGApp PRIVATE Qt6::Widgets CoreLogic)