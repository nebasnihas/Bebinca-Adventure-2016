
add_library(ascii-printer
        AsciiPrinter.cpp
        AsciiPrinter.hpp
        )

set_target_properties(networking
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

install(TARGETS ascii-printer
        ARCHIVE DESTINATION lib
        )

