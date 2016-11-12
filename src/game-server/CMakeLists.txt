add_library(string-resource-importer
        StringResourceImporter.cpp GameStrings.hpp)

set_target_properties(string-resource-importer
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

target_link_libraries(string-resource-importer
        yaml-cpp
        )

file(GLOB_RECURSE commands_src "commands/*.hpp" "commands/*.cpp")
add_library(game-server-lib
        Authenticator.cpp
        Controller.cpp
        GameFunctions.cpp GameFunctions.hpp
        Looper.cpp Looper.hpp
        ServerLoop.cpp ServerLoop.hpp
        ServerConfig.cpp ServerConfig.hpp
        ${commands_src}
        PigLatinDecorator.cpp PigLatinDecorator.hpp
        MessageIO.hpp
        ConnectionManager.hpp PlayerRole.hpp AccountInfo.hpp)

set_target_properties(game-server-lib
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

target_link_libraries(game-server-lib
        networking
        game
        string-resource-importer
        protocols
        glog
        )

find_package(Boost 1.58 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(game-server-lib ${Boost_LIBRARIES})


add_executable(game-server GameServer.cpp)
target_link_libraries(game-server game-server-lib)

set_target_properties(game-server
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        PREFIX ""
        )

include_directories("."
        "commands"
        )

install(TARGETS game-server
RUNTIME DESTINATION bin
)

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${PROJECT_BINARY_DIR}/bin)
file(COPY config.yml DESTINATION ${PROJECT_BINARY_DIR}/bin)