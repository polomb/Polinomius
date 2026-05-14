set(target ${MP2_LIBRARY})

file(GLOB srcs "*.cpp")
file(GLOB hdrs "${MP2_INCLUDE}/*.h*")

add_library(${target} STATIC ${srcs} ${hdrs})

# Настройки для Debug/Release
target_compile_options(${target} PRIVATE
    $<$<CONFIG:Debug>:/Od /Zi>
    $<$<CONFIG:Release>:/O2>
)

target_link_libraries(${target} ${LIBRARY_DEPS})