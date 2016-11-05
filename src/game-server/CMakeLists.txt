file(GLOB_RECURSE commands_src "commands/*.hpp" "commands/*.cpp")

add_executable(game-server
        GameServer.cpp
        Authenticator.cpp
        Controller.cpp
        GameFunctions.cpp GameFunctions.hpp
        Looper.cpp Looper.hpp
        ServerLoop.cpp ServerLoop.hpp
        ServerConfig.cpp ServerConfig.hpp
        ${commands_src}
        StringResourceImporter.cpp
        )

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

include_directories("."
        "commands"
        )

# This program requires boost
find_package(Boost 1.58 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(game-server ${Boost_LIBRARIES})

install(TARGETS game-server
RUNTIME DESTINATION bin
)

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${PROJECT_BINARY_DIR}/bin)
file(COPY config.yml DESTINATION ${PROJECT_BINARY_DIR}/bin)