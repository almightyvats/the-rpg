cmake_minimum_required(VERSION 3.10)



project(rpg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include_directories(
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


set(
    SOURCE_FILES 
    rpg.cpp
    RpgGame 
    TextureManager
    GameObject
    Map
    ECS/ECS
    ECS/Components
    ECS/TransformComponent
    ECS/SpriteComponent
    ECS/KeyboardController
    ECS/ColliderComponent
    Vector2D
    Collision
)
add_executable(rpg ${SOURCE_FILES})
target_link_libraries(rpg ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})









