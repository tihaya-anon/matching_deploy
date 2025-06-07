add_library(match_lib
        decimal.cpp
        engine.cpp
)
target_include_directories(match_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
