cmake_minimum_required(VERSION 3.16)

add_compile_options(-g -rdynamic -Werror)

set (CMAKE_CXX_STANDARD 17)

file(GLOB ROOT_SOURCE src/*.cpp)
file(GLOB ROOT_HEADER src/*.hpp)
set(GLAD_SOURCE external/glad/src/glad.c)
set(GLAD_INCLUDE external/glad/include/glad/glad.h external/glad/include/KHR/khrplatform.h)

project(Sintese VERSION 1.0.1 DESCRIPTION "Código para carregar um modelo destinado ao EP de computação gráfica")

set(SOURCES ${ROOT_SOURCE} ${ROOT_HEADER} ${GLAD_SOURCE} ${GLAD_INCLUDE})

add_executable(${PROJECT_NAME} ${SOURCES})

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_INSTALL OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/assimp)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE "external")

target_link_libraries(${PROJECT_NAME} glfw glm assimp OpenGL::GL)

#add_subdirectory(tests)
