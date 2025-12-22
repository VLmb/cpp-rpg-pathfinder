file(GLOB_RECURSE CORE_SOURCES "*.cpp")
file(GLOB_RECURSE CORE_HEADERS "*.h")

# 2. Исключаем main.cpp, чтобы не конфликтовал с GUI
list(FILTER CORE_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

# 3. Создаем библиотеку
# Важно передать и SOURCES и HEADERS, чтобы файлы отображались в IDE
add_library(CoreLogic STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# 4. Настраиваем пути для #include "..."
target_include_directories(CoreLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Отключаем MOC/UIC для этой библиотеки, если она не использует виджеты и сигналы/слоты
set_target_properties(CoreLogic PROPERTIES
        AUTOMOC OFF
        AUTOUIC OFF
        AUTORCC OFF
)