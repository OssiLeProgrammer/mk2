cmake_minimum_required(VERSION 3.15)

project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

if (EMSCRIPTEN)
    add_executable(MyProject ${SOURCE_DIR}/Main.cpp ${SOURCE_DIR}/Shader.cpp ${SOURCE_DIR}/Camera.cpp)
else()
    add_executable(MyProject ${SOURCE_DIR}/Main.cpp ${SOURCE_DIR}/Shader.cpp ${SOURCE_DIR}/Camera.cpp)
endif()

if (EMSCRIPTEN)
    #set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(MyProject PRIVATE -sMODULARIZE=1 -sEXPORT_NAME=CMODULE -sUSE_GLFW=3 -sFULL_ES3=1 -sEXPORTED_FUNCTIONS=['_main','_wait','_runAgain','_nfEnd','_restart'] -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','abort'] -sEXIT_RUNTIME=1 --preload-file=${SOURCE_DIR}/Vert.shader@Vert.shader --preload-file=${SOURCE_DIR}/Frag.shader@Frag.shader)
else()
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    target_link_libraries(MyProject PRIVATE glfw glad::glad)
endif()


target_compile_options(MyProject PRIVATE -Wall -Wextra -Wpedantic)