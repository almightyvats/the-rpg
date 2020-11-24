add_library (objects GameObject)

target_link_libraries (objects PUBLIC texture)

target_include_directories (objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})