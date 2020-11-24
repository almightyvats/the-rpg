add_library (collision Collision)

target_link_libraries (collision ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ecs)
target_include_directories(
    collision PUBLIC
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS}
)
target_include_directories (collision PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})