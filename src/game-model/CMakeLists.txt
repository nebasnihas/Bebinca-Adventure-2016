add_library(game Area.cpp
            Character.cpp
            Entity.cpp
            GameModel.cpp
            Inventory.cpp
            GameDataImporter.cpp
            Attributes.cpp
        ExpressionExtractor.hpp)

find_package(Boost 1.58 COMPONENTS regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(game ${Boost_LIBRARIES})

set_property(TARGET game PROPERTY CXX_STANDARD 14)

install(TARGETS game
        ARCHIVE DESTINATION lib
        )

add_subdirectory(protocols)