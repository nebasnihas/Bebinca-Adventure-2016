
add_executable(game-server
        gameServer.cpp
)

target_link_libraries(game-server
  networking
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

