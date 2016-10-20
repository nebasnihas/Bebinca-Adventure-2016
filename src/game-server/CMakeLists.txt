
add_executable(game-server
        GameServer.cpp
        Authenticator.cpp
        Command.cpp
        Controller.cpp
        DisplayMessageBuilder.cpp DisplayMessageBuilder.hpp
        GameFunctions.cpp GameFunctions.hpp Looper.cpp Looper.hpp ServerLoop.cpp ServerLoop.hpp MessageBuilder.hpp)

target_link_libraries(game-server
        networking
        game
        protocols
        glog
        )

set_target_properties(game-server
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        PREFIX ""
        )

# This program requires boost
find_package(Boost 1.58 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(game-server ${Boost_LIBRARIES})

install(TARGETS game-server
        RUNTIME DESTINATION bin
        )

add_custom_command(TARGET game-server PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:game-server>)