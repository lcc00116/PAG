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

    void Renderer::creaModelo() {
        // Tres vértices (x, y, z) ya en coordenadas canónicas (entre -1 y 1)
        GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,
                                0.5f, -0.5f, 0.0f,
                                0.0f,  0.5f, 0.0f };
        // Un triángulo formado por los vértices 0, 1 y 2
        GLuint indices[] = { 0, 1, 2 };

        // VAO: se crea y se activa. Todo lo que sigue queda registrado en él
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        // VBO: se crea, se activa y se copian los vértices a la GPU
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        // Cómo leer el VBO: atributo 0, 3 valores float por vértice, sin normalizar,
        // saltando 3 floats entre un vértice y el siguiente, empezando en el byte 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        // IBO: se crea, se activa y se copian los índices a la GPU
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

} // PAG