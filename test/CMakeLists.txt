set(target ${MP2_TESTS})

set(srcs
    test_main.cpp
    test_avl_tree.cpp
)

file(GLOB hdrs "*.h*")

add_executable(${target} ${srcs} ${hdrs})
target_link_libraries(${target} gtest ${MP2_LIBRARY})
target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/gtest ${MP2_INCLUDE})
add_test(${target} ${target})
