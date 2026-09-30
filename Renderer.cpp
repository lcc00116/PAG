//
// Created by lucar on 21/09/2026.
//

#include "Renderer.h"

#include "glad/glad.h"

#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>

namespace PAG {
    Renderer* Renderer::instancia = nullptr;

    //Constructor por defecto
    Renderer::Renderer() {
    }

    //Destructor
    Renderer::~Renderer() {
        // Solo se libera lo que realmente se creó (identificador distinto de 0)
        if (idVS != 0) {
            glDeleteShader(idVS);
        }
        if (idFS != 0) {
            glDeleteShader(idFS);
        }
        if (idSP != 0) {
            glDeleteProgram(idSP);
        }
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
    }

    //Consulta el objeto único de la clase y devuelve su dirección de memoria
    Renderer& Renderer::getInstancia() {
        if (!instancia) {
            instancia = new Renderer;
        }
        return *instancia;
    }

    void Renderer::destruyeInstancia() {
        delete instancia;      // Esto ejecuta el destructor
        instancia = nullptr;
    }

    //Refresco de la escena
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (idSP == 0 || idVAO == 0) {
            return;
        }

        // Rellena los triángulos (en vez de dibujar solo líneas o puntos)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Activa el shader program y la geometría que se van a usar
        glUseProgram(idSP);
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);

        // Dibuja: triángulos, 3 índices, de tipo unsigned int, empezando en el primero
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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
        glEnable(GL_MULTISAMPLE);// Activa el antialiasing pedido a GLFW con GLFW_SAMPLES
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

    void Renderer::compilarShader(GLuint id, const std::string& etapa) {
        glCompileShader(id);

        GLint ok = GL_FALSE;
        glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (ok == GL_FALSE) {
            GLint longitud = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitud);
            std::vector<GLchar> log(longitud > 0 ? longitud : 1);
            glGetShaderInfoLog(id, static_cast<GLsizei>(log.size()), nullptr, log.data());
            throw std::runtime_error("Error compilando el " + etapa + ":\n" + log.data());
        }
    }

    void Renderer::enlazarPrograma(GLuint id) {
        glLinkProgram(id);

        GLint ok = GL_FALSE;
        glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (ok == GL_FALSE) {
            GLint longitud = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &longitud);
            std::vector<GLchar> log(longitud > 0 ? longitud : 1);
            glGetProgramInfoLog(id, static_cast<GLsizei>(log.size()), nullptr, log.data());
            throw std::runtime_error("Error enlazando el shader program:\n" + std::string(log.data()));
        }
    }

    std::string Renderer::leeArchivo(const std::string& ruta) {
        std::ifstream archivo(ruta);
        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
        }

        std::ostringstream contenido;
        contenido << archivo.rdbuf();

        return contenido.str();
    }

    void Renderer::creaShaderProgram(const std::string& nombreBase) {
        std::string miVertexShader = leeArchivo(nombreBase + "-vs.glsl");
        std::string miFragmentShader = leeArchivo(nombreBase + "-fs.glsl");

        // Vertex shader: se crea, se le pasa el código y se compila
        idVS = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* fuenteVS = miVertexShader.c_str();
        glShaderSource(idVS, 1, &fuenteVS, nullptr);
        compilarShader(idVS, "vertex shader");

        // Fragment shader: mismos tres pasos
        idFS = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fuenteFS = miFragmentShader.c_str();
        glShaderSource(idFS, 1, &fuenteFS, nullptr);
        compilarShader(idFS, "fragment shader");

        // Shader program: se crea, se le añaden los dos shaders y se enlaza
        idSP = glCreateProgram();
        glAttachShader(idSP, idVS);
        glAttachShader(idSP, idFS);
        enlazarPrograma(idSP);
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