add_library (texture TextureManager)

target_link_libraries (texture PUBLIC game)

target_include_directories (texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
