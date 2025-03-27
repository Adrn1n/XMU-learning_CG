cmake_minimum_required(VERSION 3.10)

project(Lab3_Space-and-shuttle VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL2 REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${SOIL2_INCLUDE_DIRS}
)

include_directories(
    inc
)

set(PROJECT_SOURCES
    src/main.cpp
    src/Utils.cpp
    src/Sphere.cpp
    src/ImportedModel.cpp

    inc/Utils.h
    inc/Sphere.h
    inc/ImportedModel.h
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}

    # ${GLFW3_LIBS}
    glfw3

    # ${SOIL2_LIBRARIES}
    soil2
    X11
    GLX
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
