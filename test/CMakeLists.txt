set(target ${MP2_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../gtest/include")

add_executable(${target} ${srcs} ${hdrs})

target_link_libraries(${target} 
    gtest
    ${MP2_LIBRARY}
)

target_compile_options(${target} PRIVATE
    $<$<CONFIG:Debug>:/Od /Zi>
    $<$<CONFIG:Release>:/O2>
)