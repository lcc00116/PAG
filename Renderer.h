//
// Created by lucar on 21/09/2026.
//

#ifndef PAG_PRAC1_RENDERER_H
#define PAG_PRAC1_RENDERER_H

#include <string>
#include <glad/glad.h>

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        float _bgRed, _bgGreen, _bgBlue;

        GLuint idVS = 0;   // Identificador del vertex shader
        GLuint idFS = 0;   // Identificador del fragment shader
        GLuint idSP = 0;   // Identificador del shader program
        GLuint idVAO = 0;  // Identificador del vertex array object
        GLuint idVBO = 0;  // Identificador del vertex buffer object
        GLuint idIBO = 0;  // Identificador del index buffer object

        Renderer();
    public:
        virtual ~Renderer();
        static Renderer& getInstancia();
        void refrescar();
        void redimensionar(int width, int height);
        void cambiarColorFondo(float r, float g, float b);
        void inicializarOpenGL();
        std::string consultarOpenGL();
        void creaShaderProgram();

    };
} // PAG

#endif //PAG_PRAC1_RENDERER_H