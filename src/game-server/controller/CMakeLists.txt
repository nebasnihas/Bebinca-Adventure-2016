add_library(controller Controller.cpp)
target_link_libraries(controller gameModel)

set_property(TARGET controller PROPERTY CXX_STANDARD 14)