link_libraries(Sintese)

file(COPY "${CMAKE_SOURCE_DIR}/src/Shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(ModelViewer ModelViewer.cpp)

target_include_directories(ModelViewer PUBLIC "${CMAKE_SOURCE_DIR}/src")
