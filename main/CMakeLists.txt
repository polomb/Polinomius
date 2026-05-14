set(target "polinomic_cmd")

file(GLOB srcs "*.cpp")
file(GLOB hdrs "*.h*")

add_executable(${target} ${srcs} ${hdrs})

target_link_libraries(${target} ${MP2_LIBRARY})

# Настройки для Debug/Release
target_compile_options(${target} PRIVATE
    $<$<CONFIG:Debug>:/Od /Zi>
    $<$<CONFIG:Release>:/O2>
)