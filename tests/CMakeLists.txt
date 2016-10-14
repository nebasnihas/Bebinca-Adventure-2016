add_subdirectory(lib)
enable_testing()

#exe to run all tests
add_executable(runAllTests dummy.cpp dummy.cpp)
target_link_libraries(runAllTests gtest gtest_main gmock_main glog)
set_property(TARGET runAllTests PROPERTY CXX_STANDARD 14)

add_test(NAME runAllTests COMMAND runAllTests)