set(target "polinomic_gui")

file(GLOB SOURCES "*.cpp")

qt6_add_executable(${target} ${SOURCES})

target_link_libraries(${target} PRIVATE
    polinomic
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(${target} PRIVATE
    ${MP2_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    set_target_properties(${target} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Автоматически находим путь к Qt
    get_target_property(Qt6_Core_location Qt6::Core LOCATION)
    get_filename_component(QT_BIN_DIR ${Qt6_Core_location} DIRECTORY)
    get_filename_component(QT_DIR ${QT_BIN_DIR} DIRECTORY)
    set(QT_PLUGINS_DIR "${QT_DIR}/plugins")
    
    # Копируем DLL в зависимости от конфигурации (Debug/Release)
    add_custom_command(TARGET ${target} POST_BUILD
        # Qt6Core - Release: Qt6Core.dll, Debug: Qt6Cored.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/$<$<CONFIG:Debug>:Qt6Cored>$<$<CONFIG:Release>:Qt6Core>.dll"
            "$<TARGET_FILE_DIR:${target}>"
        # Qt6Gui
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/$<$<CONFIG:Debug>:Qt6Guid>$<$<CONFIG:Release>:Qt6Gui>.dll"
            "$<TARGET_FILE_DIR:${target}>"
        # Qt6Widgets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/$<$<CONFIG:Debug>:Qt6Widgetsd>$<$<CONFIG:Release>:Qt6Widgets>.dll"
            "$<TARGET_FILE_DIR:${target}>"
        # Создаём папку platforms
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${target}>/platforms"
        # Копируем qwindows.dll (Release) или qwindowsd.dll (Debug)
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLUGINS_DIR}/platforms/$<$<CONFIG:Debug>:qwindowsd>$<$<CONFIG:Release>:qwindows>.dll"
            "$<TARGET_FILE_DIR:${target}>/platforms/"
    )
endif()