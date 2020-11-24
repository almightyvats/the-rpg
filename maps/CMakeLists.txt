add_library (maps Map)

target_link_libraries (maps PUBLIC texture)

target_include_directories (maps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})