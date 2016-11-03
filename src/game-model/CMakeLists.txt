add_library(game Area.cpp
            Character.cpp
            Object.cpp
            GameModel.cpp
            Inventory.cpp
        Resets.cpp
            GameDataImporter.cpp
            Attributes.cpp
        )

set_property(TARGET game PROPERTY CXX_STANDARD 14)

#add_executable(game-data-importer
#        GameDataImporter.cpp
#        GameModel.cpp
#        Area.cpp
#        Character.cpp
#        Object.cpp
#        Inventory.cpp
#        Attributes.cpp
#        )

#set_target_properties(game-data-importer
#        PROPERTIES
#        LINKER_LANGUAGE CXX
#        CXX_STANDARD 14
#        PREFIX ""
#        )

add_executable(game-model-test
        GameModelTest.cpp
        GameModel.cpp
        Area.cpp
        Character.cpp
        Object.cpp
        Inventory.cpp
        Attributes.cpp
        )

set_target_properties(game-model-test
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        PREFIX ""
        )

install(TARGETS game
        ARCHIVE DESTINATION lib
        )

add_subdirectory(protocols)
