add_library(match_lib
        decimal.cc
        engine.cc
)
target_include_directories(match_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
