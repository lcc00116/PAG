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

    void Renderer::destruyeInstancia() {
        delete instancia;
        instancia = nullptr;
    }

    //Refresco de la escena
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_shaderProgram.getId() == 0 || _modelos.empty()) {
            return;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        _shaderProgram.usar();

        // Lo común a todos los modelos: proyección y visión de la cámara
        glm::mat4 proyeccionVision = _camera.matrizProyeccion() * _camera.matrizVision();

        for (const Modelo& modelo : _modelos) {
            // Cada modelo tiene su propia transformación de modelado
            _shaderProgram.setUniform("mModelViewProj",
                                      proyeccionVision * modelo.getMatrizModelado());
            modelo.dibujar();
        }
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
        _modelos.push_back(Modelo::triangulo());
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