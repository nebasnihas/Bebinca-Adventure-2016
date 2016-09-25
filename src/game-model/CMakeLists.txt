add_library(gameModel Area.cpp Character.cpp Entity.cpp GameModel.cpp Command.cpp Command.hpp)

set_property(TARGET gameModel PROPERTY CXX_STANDARD 14)