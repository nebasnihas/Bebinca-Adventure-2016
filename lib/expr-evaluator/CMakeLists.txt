
add_library(expr-evaluator
        ExprEvaluator.hpp
        ExprEvaluator.cpp
        )

set_target_properties(expr-evaluator
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        )

install(TARGETS expr-evaluator
        ARCHIVE DESTINATION lib
        )

