set(target "gtest")

# Только gtest-all.cc
add_library(${target} STATIC
    gtest-all.cc
)

# Включаем директории с заголовками
target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(UNIX)
    target_link_libraries(${target} pthread)
endif()