cmake_minimum_required(VERSION 3.25)
project(EchoGL VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive- /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror>
)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Python3 REQUIRED COMPONENTS Development)

qt_standard_project_setup()

add_subdirectory(third_party/sqlite3)
add_subdirectory(third_party/pybind11)

add_subdirectory(src/data)
add_subdirectory(src/providers)
add_subdirectory(src/core)
add_subdirectory(src/ui)

qt_add_executable(EchoGL
    src/main.cpp
)

target_link_libraries(EchoGL PRIVATE
    echogl_ui
    echogl_core
    echogl_providers
    echogl_data
    Qt6::Widgets
)

enable_testing()
add_subdirectory(tests)