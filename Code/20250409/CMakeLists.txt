cmake_minimum_required(VERSION 3.10)

project(Lab4_Iluminar VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX AND NOT APPLE)
    set(OpenGL_GL_PREFERENCE GLVND)
endif()

if(WIN32)
    set(GLEW_DIR "D:/GLEW/glew-2.1.0-win32/glew-2.1.0")
    set(GLFW_DIR "D:/GLFW/glfw-3.4.bin.WIN64")
    set(GLM_DIR "D:/GLM/glm-1.0.1-light")
    set(SOIL2_DIR "D:/SOIL2/SOIL2-master")

    find_library(GLEW_LIBRARY NAMES glew32 PATHS "${GLEW_DIR}/lib/Release/x64")
    find_library(GLFW_LIBRARY NAMES glfw3 PATHS "${GLFW_DIR}/lib-mingw-w64")

    set(GLEW_DLL "${GLEW_DIR}/bin/Release/x64/glew32.dll")
    set(GLFW_DLL "${GLFW_DIR}/lib-mingw-w64/glfw3.dll")

    add_subdirectory(${SOIL2_DIR} ${CMAKE_BINARY_DIR}/soil2)

    find_file(GCC_DLL NAMES libgcc_s_seh-1.dll PATHS "${MINGW_DIR}/bin")
    find_file(STDCPP_DLL NAMES libstdc++-6.dll PATHS "${MINGW_DIR}/bin")

    set(GLEW_INCLUDE_DIRS "${GLEW_DIR}/include")
    set(GLFW3_INCLUDE_DIRS "${GLFW_DIR}/include")
    set(GLM_INCLUDE_DIRS "${GLM_DIR}")
    set(SOIL2_INCLUDE_DIRS "${SOIL2_DIR}/src")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm REQUIRED)

    if(APPLE)
        find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
        find_library(SOIL2_LIBRARY NAMES soil2 SOIL2 PATHS /usr/local/lib REQUIRED)
        find_path(SOIL2_INCLUDE_DIRS NAMES soil2/SOIL2.h PATHS /usr/local/include REQUIRED)
    else()
        find_package(SOIL2 REQUIRED)
    endif()
endif()

set(PROJECT_SOURCES
    src/main.cpp
    src/Utils.cpp
    src/Torus.cpp

    inc/Utils.h
    inc/Torus.h
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

if(UNIX)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${OPENGL_INCLUDE_DIRS}
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLEW_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${SOIL2_INCLUDE_DIRS}
)

include_directories(
    inc
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        soil2

        # gdi32
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GCC_DLL}"
        "${STDCPP_DLL}"
        "${GLFW_DLL}"
        "${GLEW_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying DLLs to output directory"
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        GLEW::GLEW
        glfw
    )

    if(APPLE)
        target_link_libraries(${PROJECT_NAME}
            ${COREFOUNDATION_LIBRARY}
            ${SOIL2_LIBRARY}
        )
    else()
        target_link_libraries(${PROJECT_NAME}
            soil2
            X11
            GLX
        )
    endif()
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
