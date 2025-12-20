# CMake generated Testfile for 
# Source directory: D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests
# Build directory: D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(GraphLogicTests "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/build/Debug/run_graph_tests.exe")
  set_tests_properties(GraphLogicTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;18;add_test;D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(GraphLogicTests "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/build/Release/run_graph_tests.exe")
  set_tests_properties(GraphLogicTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;18;add_test;D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(GraphLogicTests "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/build/MinSizeRel/run_graph_tests.exe")
  set_tests_properties(GraphLogicTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;18;add_test;D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(GraphLogicTests "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/build/RelWithDebInfo/run_graph_tests.exe")
  set_tests_properties(GraphLogicTests PROPERTIES  _BACKTRACE_TRIPLES "D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;18;add_test;D:/Programming/Prog_Cpp/lab-assignment-3d-term/cpp-rpg-pathfinder/tests/CMakeLists.txt;0;")
else()
  add_test(GraphLogicTests NOT_AVAILABLE)
endif()
