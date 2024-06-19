cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

file(GLOB ROOT_SOURCE src/*.cpp)
file(GLOB ROOT_HEADER src/*.hpp)
set(INCLUDE_FOLDER ${CMAKE_SOURCE_DIR}/external)
set(GLAD_SOURCE ${INCLUDE_FOLDER}/glad/src/glad.c)
set(GLAD_INCLUDE ${INCLUDE_FOLDER}/glad/include/glad/glad.h ${INCLUDE_FOLDER}/glad/include/KHR/khrplatform.h)
set(STB_INCLUDE ${INCLUDE_FOLDER}/stb_image/stb_image.h)
set(STB_SOURCE ${INCLUDE_FOLDER}/stb_image/stb_image.cpp)


project(Sintese VERSION 1.0.1 DESCRIPTION "Código para carregar um modelo destinado ao EP de computação gráfica")

set(SOURCES ${ROOT_SOURCE} ${ROOT_HEADER} ${GLAD_SOURCE} ${GLAD_INCLUDE} ${STB_INCLUDE} ${STB_SOURCE} src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})


target_compile_definitions(${PROJECT_NAME} PRIVATE -DGLFW_INCLUDE_NONE)    

if(CMAKE_SYSTEM MATCHES Windows)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_WIN32)
    message("windows")
endif()

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_INSTALL OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(external/assimp)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_FOLDER}" "${INCLUDE_FOLDER}/assimp/include" "${INCLUDE_FOLDER}/glm" "${INCLUDE_FOLDER}/glad/include" "${INCLUDE_FOLDER}/glfw/include")

target_link_libraries(${PROJECT_NAME} glfw glm assimp OpenGL::GL ${CMAKE_DL_LIBS})

file(COPY src/Shaders DESTINATION ${CMAKE_BINARY_DIR})

#add_subdirectory(tests)
