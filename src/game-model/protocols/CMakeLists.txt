add_library(protocols
        RequestMessage.cpp
        ResponseMessage.cpp
        DisplayMessage.cpp
        PlayerCommand.cpp
        Authentication.cpp
        )

target_link_libraries(protocols
        yaml-cpp
        string-resource-importer
        )

set_target_properties(protocols
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

install(TARGETS protocols
        ARCHIVE DESTINATION lib
        )
