# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "gui\\CMakeFiles\\MapPathfinderApp_autogen.dir\\AutogenUsed.txt"
  "gui\\CMakeFiles\\MapPathfinderApp_autogen.dir\\ParseCache.txt"
  "gui\\MapPathfinderApp_autogen"
  )
endif()
