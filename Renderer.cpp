//
// Created by lucar on 21/09/2026.
//

#include "Renderer.h"

#include "glad/glad.h"

namespace PAG {
    Renderer* Renderer::instancia = nullptr;

    //Constructor por defecto
    Renderer::Renderer() {
    }

    //Destructor
    Renderer::~Renderer() {
    }

    //Consulta el objeto único de la clase y devuelve su dirección de memoria
    Renderer& Renderer::getInstancia() {
        if (!instancia) {
            instancia = new Renderer;
        }
        return *instancia;
    }

    //Refresco de la escena
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //Cambio del tamaño de la ventana
    void Renderer::redimensionar(int width, int height) {
        glViewport(0, 0, width, height);
    }

    //Cambia el color de la ventana
    void Renderer::cambiarColorFondo(float r, float g, float b) {
        _bgRed = r;
        _bgGreen = g;
        _bgBlue = b;
        glClearColor(_bgRed, _bgGreen, _bgBlue, 1.0f);
    }

    void Renderer::inicializarOpenGL() {
        glEnable(GL_DEPTH_TEST);
    }

    std::string Renderer::consultarOpenGL() {
        std::string info;
        info += reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        info += "\n";
        info += reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        info += "\n";
        info += reinterpret_cast<const char*>(glGetString(GL_VERSION));
        info += "\n";
        info += reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        return info;
    }

    void Renderer::creaShaderProgram() {
        // Código fuente de los shaders (de momento, como texto dentro del método)
        std::string miVertexShader =
            "#version 410\n"
            "layout (location = 0) in vec3 posicion;\n"
            "void main ()\n"
            "{ gl_Position = vec4 ( posicion, 1 );\n"
            "}\n";

        std::string miFragmentShader =
            "#version 410\n"
            "out vec4 colorFragmento;\n"
            "void main ()\n"
            "{ colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
            "}\n";

        // Vertex shader: se crea, se le pasa el código y se compila
        idVS = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* fuenteVS = miVertexShader.c_str();
        glShaderSource(idVS, 1, &fuenteVS, nullptr);
        glCompileShader(idVS);

        // Fragment shader: mismos tres pasos
        idFS = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fuenteFS = miFragmentShader.c_str();
        glShaderSource(idFS, 1, &fuenteFS, nullptr);
        glCompileShader(idFS);

        // Shader program: se crea, se le añaden los dos shaders y se enlaza
        idSP = glCreateProgram();
        glAttachShader(idSP, idVS);
        glAttachShader(idSP, idFS);
        glLinkProgram(idSP);
    }

} // PAG