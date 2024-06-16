link_libraries(Sintese)

file(COPY "shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(renderCallTest renderCallTest.cpp)
add_executable(window windowTest.cpp)
