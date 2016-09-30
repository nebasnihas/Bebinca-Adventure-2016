add_library(controller Controller.cpp BufferedMessageSender.cpp BufferedMessageSender.h MessageSender.h)
target_link_libraries(controller gameModel networking)

set_property(TARGET controller PROPERTY CXX_STANDARD 14)