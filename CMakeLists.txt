cmake_minimum_required(VERSION 3.10)

project(rpg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_subdirectory(collision)
add_subdirectory(vectors)
add_subdirectory(ecs)
add_subdirectory(maps)
add_subdirectory(game)
add_subdirectory(texture)
add_subdirectory(objects)

set(
    SOURCE_FILES 
    main.cpp
    collision
    game
    texture
    maps
    vectors
    ecs
    objects
    vendor/rapidjson/document
    vendor/rapidjson/writer
    vendor/rapidjson/stringbuffer
)
set(
    GAME_LIBS
    collision
    game
    texture
    maps
    vectors
    ecs
    objects
)

add_executable(rpg ${SOURCE_FILES})

target_link_libraries (
    rpg LINK_PUBLIC 
    ${GAME_LIBS} ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES})

target_include_directories(
    rpg PUBLIC
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS}
)









