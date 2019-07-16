add_library(GetLoopExitingBBs MODULE src/GetLoopExitingBBs.cpp)

include_directories(include)

target_compile_features(GetLoopExitingBBs PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(GetLoopExitingBBs PROPERTIES
        COMPILE_FLAGS "-g"
        )

if(APPLE)
    set_target_properties(GetLoopExitingBBs PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
