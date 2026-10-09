cmake_minimum_required(VERSION 3.22)
project(RType LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

foreach(option FT_DISABLE_ZLIB FT_DISABLE_BZIP2 FT_DISABLE_PNG FT_DISABLE_HARFBUZZ FT_DISABLE_BROTLI)
    set(${option} ON CACHE BOOL "" FORCE)
endforeach()

FetchContent_Declare(
    FreeType
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(FreeType)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)
if (WIN32)
    # set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/Debug/freetyped.lib CACHE STRING "" FORCE)
else()
    set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/libfreetype.a CACHE STRING "" FORCE)
endif()

set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include CACHE STRING "" FORCE)

set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

set(OPENAL_LIBRARY "" CACHE STRING "" FORCE)
set(OPENAL_INCLUDE_DIR "" CACHE STRING "" FORCE)

FetchContent_Declare(
    ASIO
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-18-1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ASIO)

add_subdirectory(utils)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(ecs)

foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set_target_properties(ecs PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${config} "${CMAKE_BINARY_DIR}")
    set_target_properties(client server PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${config} "${CMAKE_SOURCE_DIR}")
endforeach()
set_target_properties(ecs PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
