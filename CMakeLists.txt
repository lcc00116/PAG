    cmake_minimum_required(VERSION 4.0)
    project(PAG_prac1)

    set(CMAKE_CXX_STANDARD 20)

    add_executable(PAG_prac1 main.cpp)

    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)