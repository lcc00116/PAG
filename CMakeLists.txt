    cmake_minimum_required(VERSION 4.0)
    project(PAG_prac1)

    set(CMAKE_CXX_STANDARD 20)

    #Define variables agrupando nombres de archivo
    file( GLOB MY_FILES *.cpp ) #Código fuente del proyecto
    file( GLOB GLAD_FILES GLAD/src/*.c ) #Código fuente de GLAD
    file( GLOB IMGUI_FILES imgui/src/*.cpp )


    #Indica qué archivos hay que compilar para generar el ejecutable
    add_executable( ${PROJECT_NAME} ${MY_FILES}
                                    ${GLAD_FILES}
                                    ${IMGUI_FILES}
            Renderer.cpp
            Renderer.h
            GUI.cpp
            GUI.h)

    #Indica directorios (adicionales a los por defeto) para buscar includes
    target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include
                                                    imgui/include )

    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)