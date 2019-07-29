add_library(StructAccessIdentifier MODULE src/StructAccessIdentifier.cpp)

include_directories(include)

target_compile_features(StructAccessIdentifier PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(StructAccessIdentifier PROPERTIES
        COMPILE_FLAGS "-g"
        )

if(APPLE)
    set_target_properties(StructAccessIdentifier PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
