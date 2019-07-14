add_library(FunctionIdentifier MODULE src/FunctionIdentifierPass.cpp)

include_directories(include)

target_compile_features(FunctionIdentifier PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(FunctionIdentifier PROPERTIES
        COMPILE_FLAGS "-g"
        )

if(APPLE)
    set_target_properties(FunctionIdentifier PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
