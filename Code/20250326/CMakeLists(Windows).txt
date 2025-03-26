cmake_minimum_required(VERSION 3.10)

project(Lab3_Space-and-shuttle VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(GLEW_DIR "D:/GLEW/glew-2.1.0-win32/glew-2.1.0")
set(GLFW_DIR "D:/GLFW/glfw-3.4.bin.WIN64")
set(GLM_DIR "D:/GLM/glm-1.0.1-light")
set(SOIL2_DIR "D:/SOIL2/SOIL2-master")

include_directories(
    "${GLEW_DIR}/include"
    "${GLFW_DIR}/include"
    "${GLM_DIR}"
    "${SOIL2_DIR}/src"
)

find_library(GLEW_LIBRARY NAMES glew32 PATHS "${GLEW_DIR}/lib/Release/x64")
find_library(GLFW_LIBRARY NAMES glfw3 PATHS "${GLFW_DIR}/lib-mingw-w64")

find_file(GCC_DLL NAMES libgcc_s_seh-1.dll PATHS "${MINGW_DIR}/bin")
find_file(STDCPP_DLL NAMES libstdc++-6.dll PATHS "${MINGW_DIR}/bin")

set(GLEW_DLL "${GLEW_DIR}/bin/Release/x64/glew32.dll")
set(GLFW_DLL "${GLFW_DIR}/lib-mingw-w64/glfw3.dll")

add_subdirectory(${SOIL2_DIR} ${CMAKE_BINARY_DIR}/soil2)

include_directories(inc)

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

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
