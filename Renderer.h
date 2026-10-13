#ifndef PAG_PRAC1_RENDERER_H
#define PAG_PRAC1_RENDERER_H

#include <string>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Camera.h"
#include "MovimientoCamara.h"

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        float _bgRed, _bgGreen, _bgBlue;

        GLuint idVAO = 0;         // Identificador del vertex array object
        GLuint idVBO = 0;         // Identificador del vertex buffer object
        GLuint idIBO = 0;         // Identificador del index buffer object
        GLuint idVBOColores = 0;  // Identificador del VBO de colores (versión no entrelazada)

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