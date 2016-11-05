add_subdirectory(lib)
enable_testing()

#exe to run all tests
add_executable(runAllTests dummy.cpp dummy.cpp CombatTests.cpp)
target_link_libraries(runAllTests gtest gtest_main gmock_main glog game)
set_property(TARGET runAllTests PROPERTY CXX_STANDARD 14)

add_test(NAME runAllTests COMMAND runAllTests)