add_library(string-resource-importer
        StringResourceImporter.cpp ../../include/GameStrings.hpp)

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
        ConnectionManager.hpp PlayerRole.hpp AccountInfo.hpp
        HelpCommand.cpp HelpCommand.hpp
        CommandInfoRequestHandler.cpp CommandInfoRequestHandler.hpp)

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

#Copy data
file(GLOB data RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/data/*)
add_custom_target(data_target ALL)
foreach(file ${data})
    set(SRC "${CMAKE_SOURCE_DIR}/${file}")
    set(DST "${PROJECT_BINARY_DIR}/bin/${file}")
    add_custom_command(
            TARGET data_target
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SRC} ${DST}
            )
endforeach(file)

add_custom_target(config ALL)
add_custom_command(
        TARGET config
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/config.yml ${PROJECT_BINARY_DIR}/bin/config.yml
        )