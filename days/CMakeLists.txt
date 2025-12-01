file(GLOB_RECURSE DAY_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*/solution.cpp
)

add_library(aoc_days STATIC ${DAY_SOURCES})

target_include_directories(aoc_days PUBLIC ${CMAKE_SOURCE_DIR}/include)
