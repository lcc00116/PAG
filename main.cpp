#include <iostream>
#include <cstdlib> //para rand()
#include <ctime>   //para time(), que usamos al generar la semilla
#include <sstream>
#include <iomanip>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//Incluye GLAD siempre ANTES que GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GUI.h"
#include "Renderer.h"

//Esta función callback será llamada cuando GLFW produzca algún error
void error_callback(int errno, const char* desc) {
    std::string aux(desc);
    PAG::GUI::getInstancia().agregarMensaje("Error de GLFW número " + std::to_string(errno) + ": " + aux);
}

//Esta función callback será llamada cada vez que el área de dibujo
//OpenGL deba ser redibujada
void window_refresh_callback(GLFWwindow* window) {
    PAG::Renderer::getInstancia().refrescar();
    PAG::GUI::getInstancia().refrescar();

    glfwSwapBuffers(window);
    std::cout << "Refresh callback called" << std::endl;
}

//Esta función callback será llamada cada vez que se cambie el tamaño
//del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height ){
    PAG::Renderer::getInstancia().redimensionar(width, height);
    std::cout << "Resize callback called" << std::endl;
}

//Esta función callback será llamada cada vez que se pulse una tecla
//dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods ){
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if ( action == GLFW_PRESS ) {
        PAG::GUI::getInstancia().agregarMensaje("Tecla pulsada: " + std::to_string(key));
    } else if ( action == GLFW_RELEASE ) {
        PAG::GUI::getInstancia().agregarMensaje("Tecla soltada: " + std::to_string(key));
    }
}

//Esta función callback será llamada cada vez que se pulse algún botón
//del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if ( action == GLFW_PRESS ){
        PAG::GUI::getInstancia().agregarMensaje("Pulsado el botón: " + std::to_string(button));
        PAG::GUI::getInstancia().notificarBotonRaton(button, true);
    }
    else if ( action == GLFW_RELEASE ){
        PAG::GUI::getInstancia().agregarMensaje("Soltado el botón: " + std::to_string(button));
        PAG::GUI::getInstancia().notificarBotonRaton(button, false);
    }
}


//Función auxiliar que genera el color de forma aleatoria
float randomColorValue() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

//Esta función callback será llamada cada vez que se mueva la rueda
//del ratón sobre el área de dibujo OpenGL
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset ) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Movida la rueda del ratón " << xoffset
        << " unidades en horizontal y " << yoffset
        << " unidades en vertical";

    PAG::GUI::getInstancia().agregarMensaje(oss.str());

    if (yoffset != 0.0) {
        float r = randomColorValue();
        float g = randomColorValue();
        float b = randomColorValue();

        PAG::Renderer::getInstancia().cambiarColorFondo(r, g, b);
    }
}



int main() {
    // Este mensaje se guarda como el primero en la ventana de "Mensajes"
    PAG::GUI::getInstancia().agregarMensaje("Starting Application PAG");

    // Sembramos el generador de números aleatorios una única vez (TRABAJO AUTÓNOMO)
    srand(static_cast<unsigned int>(time(nullptr)));

    //Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback ( (GLFWerrorfun) error_callback );

    //Inicializamos GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "Failed to initialize GLFW" << std::endl; // No hay ventana aún, no se puede mostrar en ImGui
        return -1;
    }

    //Definimos las características que queremos que tenga el contexto gráfico
    //OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras
    //o el modo Core Profile

    glfwWindowHint(GLFW_SAMPLES, 4); //Activa antialising con 4 muestras

    //Estas 3 líneas activan un contexto OpenGL Core Profile 4.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Definimos el puntero para guardar la dirección de la ventana
    //de la aplicación y la creamos
    GLFWwindow* window;

    //Tamaño, título de la ventana, en ventana y no en pantalla completa,
    //sin compartir recursos con otras ventanas
    window = glfwCreateWindow(1024, 576, "PAG Introduction", nullptr, nullptr);

    //Comprobamos si la creación de la ventana ha ido bien
    if (window == nullptr) {
        std::cout << "Failed to open GLFW window" << std::endl; // Aún sin ventana, tampoco puede ir a ImGui
        glfwTerminate(); //Liberamos los recursos que ocupaba GLFW
        return -2;
    }

    //EL contexto OpenGL asociado a la ventana que acabamos de crear pasa a
    //ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(window);

    //Ahora inicializamos GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "GLAD inicialization failed" << std::endl; // Falla antes de tener ImGui listo
        glfwDestroyWindow(window); //Liberamos los recursos que ocupaba GLFW
        window = nullptr;
        glfwTerminate();
        return -3;
    }

    //Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    //3D construido. Ahora este mensaje va a la ventana de Mensajes, no a la consola
    PAG::GUI::getInstancia().agregarMensaje(PAG::Renderer::getInstancia().consultarOpenGL());

    //Inicializamos Dear ImGui
    PAG::GUI::getInstancia().inicializar(window);

    //Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );
    glfwSetScrollCallback ( window, scroll_callback );

    //Establecemos un gris medio como color con el que se borrará el frame buffer
    //No tiene por qué ejecutarse en cada paso por el ciclo de eventos
    PAG::Renderer::getInstancia().cambiarColorFondo(0.6f, 0.6f, 0.6f);

    //Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar
    //No tiene por qué ejecutarse en cada paso por el ciclo de eventos
    PAG::Renderer::getInstancia().inicializarOpenGL();

    //Creamos el shader program y el modelo (una sola vez, antes del ciclo de eventos)
    PAG::Renderer::getInstancia().creaShaderProgram();
    PAG::Renderer::getInstancia().creaModelo();

    //Ciclo de eventos de la aplicación. La condición de parada es que la
    //ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    //botón de cerrar la ventana
    while (!glfwWindowShouldClose(window)) {
        PAG::Renderer::getInstancia().refrescar();
        PAG::GUI::getInstancia().refrescar();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Una vez terminado el ciclo de eventos, liberar recursos, etc
    //Este mensaje se queda en consola: la ventana ya no se va a redibujar, no se vería
    std::cout << "Finishing application pag prueba" << std::endl;

    PAG::GUI::getInstancia().liberar();

    glfwDestroyWindow(window); //Cerramos y destruimos la ventana de la aplicación
    window = nullptr;
    glfwTerminate();

    return 0;
}