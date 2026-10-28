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
        GUI.h
        ShaderProgram.cpp
        ShaderProgram.h
        MovimientoCamara.h
        Camera.cpp
        Camera.h
        Modelo.cpp
        Modelo.h
        ModoVisualizacion.h
        Material.h)

#Copia la carpeta de modelos al directorio de ejecución en cada compilación
add_custom_target(copiar_modelos ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/modelos ${CMAKE_BINARY_DIR}/modelos)
add_dependencies(${PROJECT_NAME} copiar_modelos)

#Copia los shaders al directorio donde se ejecuta el programa
configure_file(${CMAKE_SOURCE_DIR}/pag03-vs.glsl ${CMAKE_BINARY_DIR}/pag03-vs.glsl COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/pag03-fs.glsl ${CMAKE_BINARY_DIR}/pag03-fs.glsl COPYONLY)

#Indica directorios (adicionales a los por defeto) para buscar includes
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include
        imgui/include )

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glm::glm assimp::assimp)