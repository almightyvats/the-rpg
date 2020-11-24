add_library (ecs INTERFACE
)

target_link_libraries (ecs INTERFACE vectors texture game)

target_include_directories (ecs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})