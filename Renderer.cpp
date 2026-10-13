#include "Renderer.h"

#include "glad/glad.h"

namespace PAG {
    Renderer* Renderer::instancia = nullptr;

    //Constructor por defecto
    Renderer::Renderer() {
    }

    //Destructor
    Renderer::~Renderer() {
        // ShaderProgram se destruye solo (es un atributo por valor, no un puntero);
        // su propio destructor libera idVS, idFS e idSP.
        // Aquí solo liberamos lo que sigue gestionando Renderer directamente.
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
        if (idVBOColores != 0) {
            glDeleteBuffers(1, &idVBOColores);
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
        delete instancia;
        instancia = nullptr;
    }

    //Refresco de la escena
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_shaderProgram.getId() == 0 || idVAO == 0) {
            return;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        _shaderProgram.usar();
        _shaderProgram.setUniform("mModelViewProj", _camera.matrizProyeccion() * _camera.matrizVision());
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    //Cambio del tamaño de la ventana
    void Renderer::redimensionar(int width, int height) {
        glViewport(0, 0, width, height);
        _camera.setViewport(width, height);
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
        glEnable(GL_MULTISAMPLE);
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

    void Renderer::creaShaderProgram(const std::string& nombreBase) {
        ShaderProgram nuevo;
        nuevo.crear(nombreBase);            // si falla, lanza excepción y _shaderProgram no se toca
        _shaderProgram = std::move(nuevo);  // solo llega aquí si todo ha ido bien
    }

    void Renderer::creaModelo() {
        GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,
                                0.5f, -0.5f, 0.0f,
                                0.0f,  0.5f, 0.0f };
        GLuint indices[] = { 0, 1, 2 };

        GLfloat colores[] = { 1.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f,
                               0.0f, 0.0f, 1.0f };

        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &idVBOColores);
        glBindBuffer(GL_ARRAY_BUFFER, idVBOColores);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colores, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

    void Renderer::moverCamara(MovimientoCamara tipo, float a, float b) {
        switch (tipo) {
            case MovimientoCamara::Pan:   _camera.pan(a); break;
            case MovimientoCamara::Tilt:  _camera.tilt(a); break;
            case MovimientoCamara::Dolly: _camera.dolly(a, b); break;  // a = derecha, b = adelante
            case MovimientoCamara::Crane: _camera.crane(a); break;
            case MovimientoCamara::Orbit: _camera.orbit(a, b); break;  // a = latitud, b = longitud
            case MovimientoCamara::Zoom:  _camera.zoom(a); break;
        }
    }

    // Convierte el arrastre del ratón (en píxeles) según el movimiento seleccionado
    void Renderer::arrastrarRaton(float dx, float dy) {
        const float g = 0.2f;    // grados por píxel
        const float u = 0.01f;   // unidades de escena por píxel
        switch (_movimiento) {
            case MovimientoCamara::Pan:   moverCamara(_movimiento, dx * g); break;
            case MovimientoCamara::Tilt:  moverCamara(_movimiento, dy * g); break;
            case MovimientoCamara::Dolly: moverCamara(_movimiento, -dx * u, dy * u); break;
            case MovimientoCamara::Crane: moverCamara(_movimiento, dy * u); break;
            case MovimientoCamara::Orbit: moverCamara(_movimiento, dy * g, -dx * g); break;
            case MovimientoCamara::Zoom:  moverCamara(_movimiento, dy * g); break;
        }
    }

    // dirX: -1 = izquierda, +1 = derecha; dirY: +1 = arriba, -1 = abajo
    void Renderer::moverPasoCamara(int dirX, int dirY) {
        const float paso = 2.0f;     // grados por pulsación
        const float lineal = 0.1f;   // unidades de escena por pulsación
        switch (_movimiento) {
            case MovimientoCamara::Pan:   moverCamara(_movimiento, -dirX * paso); break;
            case MovimientoCamara::Tilt:  moverCamara(_movimiento, dirY * paso); break;
            case MovimientoCamara::Dolly: moverCamara(_movimiento, dirX * lineal, dirY * lineal); break;
            case MovimientoCamara::Crane: moverCamara(_movimiento, dirY * lineal); break;
            case MovimientoCamara::Orbit: moverCamara(_movimiento, dirY * paso, dirX * paso); break;
            case MovimientoCamara::Zoom:  moverCamara(_movimiento, -dirY * paso); break;
        }
    }

} // PAG