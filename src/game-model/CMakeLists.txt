add_library(game Area.cpp
            Character.cpp
            Object.cpp
            GameModel.cpp
            Inventory.cpp
            Resets.cpp
            GameDataImporter.cpp
            Attributes.cpp
            combat/CombatManager.cpp
            combat/CombatInstance.cpp
            combat/CharacterInstance.cpp
            combat/CombatAction.cpp
            Spell.cpp)

set_property(TARGET game PROPERTY CXX_STANDARD 14)

install(TARGETS game
        ARCHIVE DESTINATION lib
        )

add_subdirectory(protocols)