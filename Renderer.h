#ifndef PAG_PRAC1_RENDERER_H
#define PAG_PRAC1_RENDERER_H

#include <string>
#include <glad/glad.h>
#include <vector>
#include "ShaderProgram.h"
#include "Camera.h"
#include "MovimientoCamara.h"
#include "Modelo.h"
#include "ModoVisualizacion.h"

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        float _bgRed, _bgGreen, _bgBlue;

        std::vector<Modelo> _modelos;
        ModoVisualizacion _modo = ModoVisualizacion::Solido;
        void anadirModelo(Modelo nuevo);     //Coloca y añade un modelo a la escena

        PAG::ShaderProgram _shaderProgram;
        Camera _camera;
        MovimientoCamara _movimiento = MovimientoCamara::Orbit;

        Renderer();

    public:
        virtual ~Renderer();
        static Renderer& getInstancia();
        static void destruyeInstancia();
        void refrescar();
        void redimensionar(int width, int height);
        void cambiarColorFondo(float r, float g, float b);
        void inicializarOpenGL();
        std::string consultarOpenGL();
        void creaShaderProgram(const std::string& nombreBase);
        void creaModelo();
        void cargarModelo(const std::string& ruta);
        void setModo(ModoVisualizacion modo) { _modo = modo; }
        ModoVisualizacion getModo() const { return _modo; }
        void eliminarModelo(std::size_t indice);
        void trasladarModelo(std::size_t indice, float x, float y, float z);
        void rotarModelo(std::size_t indice, float grados, const glm::vec3& eje);
        void escalarModelo(std::size_t indice, float factor);
        std::size_t numModelos() const {return _modelos.size();}
        void moverCamara(MovimientoCamara tipo, float a, float b = 0.0f);
        void arrastrarRaton(float dx, float dy);
        void moverPasoCamara(int dirX, int dirY);
        void seleccionarMovimiento(MovimientoCamara m) { _movimiento = m; }
        MovimientoCamara getMovimiento() const { return _movimiento; }
        float getFovCamara() const { return _camera.getFovX(); }
        void setFovCamara(float grados) { _camera.setFovX(grados); }
    };
} // PAG

#endif //PAG_PRAC1_RENDERER_H