add_library(game Area.cpp
            Character.cpp
            Entity.cpp
            GameModel.cpp
            Inventory.cpp
            GameDataImporter.cpp
            Attributes.cpp
            )

set_property(TARGET game PROPERTY CXX_STANDARD 14)

install(TARGETS game
        ARCHIVE DESTINATION lib
        )

add_subdirectory(protocols)