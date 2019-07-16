add_library(LogMemAccess MODULE src/LogMemAccess.cpp)

include_directories(include)

target_compile_features(LogMemAccess PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(LogMemAccess PROPERTIES
        COMPILE_FLAGS "-g"
        )

if(APPLE)
    set_target_properties(LogMemAccess PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
