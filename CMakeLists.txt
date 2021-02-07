cmake_minimum_required(VERSION 3.10)

project(rpg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
FIND_PACKAGE(Protobuf REQUIRED)

file(GLOB_RECURSE rpg_srcs rpg/*.cpp rpg/*.hpp)

add_subdirectory(vendor/rapidjson)

add_library( rpg_engine ${rpg_srcs})

target_link_libraries (
        rpg_engine PUBLIC
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${Protobuf_LIBRARIES}
        )

target_include_directories(
        rpg_engine PUBLIC
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${Protobuf_INCLUDE_DIRS}
        .
)

add_executable(rpg app/main.cpp)
target_link_libraries(rpg PUBLIC rpg_engine)










