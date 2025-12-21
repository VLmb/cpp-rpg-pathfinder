
add_library(CoreLogic STATIC
        model/Enums.h
        model/Hero.h
        model/LinkedList.h
        model/AbstractGraph.h
        model/CheckpointGraph.h
        model/Grid.h
        data-structure/PriorityQueue.h
        data-structure/DynamicArray.h
        data-structure/LinkedList.h
        data-structure/HashMap.h
)

# Говорим, где искать .h файлы
target_include_directories(CoreLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
