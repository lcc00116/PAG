#include "Renderer.h"

#include <cstdlib>
#include <utility>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include "glad/glad.h"

namespace PAG {
    Renderer* Renderer::instancia = nullptr;

    namespace {
        // Devuelve un número aleatorio entre min y max
        float aleatorio(float min, float max) {
            return min + (max - min) * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        }
    }

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
        Modelo nuevo = Modelo::triangulo();

        // El primer modelo se queda en el centro; los demás se reparten por la escena
        if (!_modelos.empty()) {
            glm::vec3 posicion(aleatorio(-1.0f, 1.0f),    //x
                               aleatorio(-0.2f, 0.2f),    //y
                               aleatorio(-1.0f, 0.0f));   //z
            nuevo.setMatrizModelado(glm::translate(glm::mat4(1.0f), posicion));
        }

        _modelos.push_back(std::move(nuevo));
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

    void Renderer::eliminarModelo(std::size_t indice) {
        if (indice >= _modelos.size()) {
            throw std::out_of_range("No existe ese modelo");
        }
        _modelos.erase(_modelos.begin() + indice);
    }

    void Renderer::trasladarModelo(std::size_t indice, float x, float y, float z) {
        Modelo& modelo = _modelos.at(indice);   //Lanza std::out_of_range si no existe

        //La traslación va a la IZQUIERDA de la matriz actual, así que se mueve
        //según los ejes de la escena, y no según los del modelo
        modelo.setMatrizModelado(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                                 * modelo.getMatrizModelado());
    }

} // PAG