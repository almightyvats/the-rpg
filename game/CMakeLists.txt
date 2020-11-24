add_library (game RpgGame)

target_link_libraries (game PUBLIC collision ecs maps)

target_include_directories (game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
