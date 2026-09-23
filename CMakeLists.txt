cmake_minimum_required(VERSION 3.20)
project(SkyRay LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        6.0
  GIT_SHALLOW    ON
)
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui.git
  GIT_TAG        5.0
  GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(raylib raygui)
target_compile_definitions(raylib PRIVATE SUPPORT_FILEFORMAT_HDR=1)
set_source_files_properties(
    ${raylib_SOURCE_DIR}/src/rtextures.c
    TARGET_DIRECTORY raylib
    PROPERTIES COMPILE_OPTIONS "-O0"
)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(SkyRay ${SOURCES})
target_include_directories(SkyRay SYSTEM PRIVATE
  ${raygui_SOURCE_DIR}/src
  ${raygui_SOURCE_DIR}/styles
)
target_include_directories(SkyRay PRIVATE include 
  # ${raygui_SOURCE_DIR}/src
  # ${raygui_SOURCE_DIR}/styles
)
target_link_libraries(SkyRay PRIVATE raylib)
