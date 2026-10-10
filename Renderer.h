#ifndef PAG_PRAC1_RENDERER_H
#define PAG_PRAC1_RENDERER_H

#include <string>
#include <glad/glad.h>
#include "ShaderProgram.h"

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
    };
} // PAG

#endif //PAG_PRAC1_RENDERER_H