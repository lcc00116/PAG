#include <iostream>

//Incluye GLAD siempre ANTES que GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::cout << "Starting Application PAG" << std::endl;

    //Inicializamos GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "Failed to initialize GLFW" << std::endl;
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
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate(); //Liberamos los recursos que ocupaba GLFW
        return -2;
    }

    //EL contexto OpenGL asociado a la ventana que acabamos de crear pasa a
    //ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(window);

    //Ahora inicializamos GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "GLAD inicialization failed" << std::endl;
        glfwDestroyWindow(window); //LIberamos los recursos que ocupaba GLFW
        window = nullptr;
        glfwTerminate();
        return -3;
    }

    //Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    //3D construido
    std::cout << glGetString (GL_RENDERER) << std::endl
              << glGetString (GL_VENDOR) << std::endl
              << glGetString (GL_VERSION) << std::endl
              << glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //Establecemos un gris medio como color con el que se borrará el frame buffer
    //No tiene por qué ejecutarse en cada paso por el ciclo de eventos
    glClearColor(0.6, 0.6, 0.6, 1.0);

    //Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar
    //No tiene por qué ejecutarse en cada paso por el ciclo de eventos
    glEnable (GL_DEPTH_TEST);

    //Ciclo de eventos de la aplicación. La condición de parada es que la
    //ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    //botón de cerrar la ventana
    while (!glfwWindowShouldClose(window)) {
        //Borra los buffers (color y profundidad)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //GLFW usa un doble buffer para que no haya parpadeo. Esta orden
        //intercambia el buffer back (en el que se ha estado dibujando) por el
        //que se mostraba hasta ahora (front)
        glfwSwapBuffers(window);
        //Obtiene y organiza los eventos pendientes, tales como pulsaciones
        //de teclas o de ratón, etc. Siempre al final de cada iteración del
        //ciclo de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Una vez terminado el ciclo de eventos, liberar recursos, etc
    std::cout << "Finishing application pag prueba" << std::endl;

    glfwDestroyWindow(window); //Cerramos y destruimos la ventana de la aplicación
    window = nullptr;
    glfwTerminate();

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}