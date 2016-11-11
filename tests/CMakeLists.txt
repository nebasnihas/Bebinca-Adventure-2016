add_subdirectory(lib)
enable_testing()

include_directories("../src/game-server/commands")
include_directories("../src/game-server")

find_package(Boost 1.58 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#exe to run all tests
add_executable(runAllTests
        dummy.cpp
        CommandConfigTests.cpp
        lib/ExpressionExtractorTests.cpp SpellImportingTest.cpp)

target_link_libraries(runAllTests
        gtest
        gtest_main
        gmock_main
        game
        game-server-lib)
set_property(TARGET runAllTests PROPERTY CXX_STANDARD 14)

add_test(NAME runAllTests COMMAND runAllTests)
