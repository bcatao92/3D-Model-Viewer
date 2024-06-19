cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB ROOT_SOURCE ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB ROOT_HEADER ${CMAKE_SOURCE_DIR}/src/*.hpp)
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)
set(GLAD_INCLUDE ${CMAKE_SOURCE_DIR}/external/glad/include/glad/glad.h ${CMAKE_SOURCE_DIR}/external/glad/include/KHR/khrplatform.h)
set(STB_INCLUDE ${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.h)
set(STB_SOURCE ${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.cpp)


project(Sintese VERSION 1.0.1 DESCRIPTION "Código para carregar um modelo destinado ao EP de computação gráfica")

set(SOURCES ${ROOT_SOURCE} ${ROOT_HEADER} ${GLAD_SOURCE} ${GLAD_INCLUDE} ${STB_INCLUDE} ${STB_SOURCE} ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})


target_compile_definitions(${PROJECT_NAME} PRIVATE -DGLFW_INCLUDE_NONE)    

if(CMAKE_SYSTEM MATCHES Windows)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_WIN32)
    message("windows")
endif()

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_INSTALL OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/assimp)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/external")

target_link_libraries(${PROJECT_NAME} glfw glm assimp OpenGL::GL ${CMAKE_DL_LIBS})

file(COPY ${CMAKE_SOURCE_DIR}/src/Shaders DESTINATION ${CMAKE_BINARY_DIR})

#add_subdirectory(tests)
