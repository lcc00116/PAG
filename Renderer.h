//
// Created by lucar on 21/09/2026.
//

#ifndef PAG_PRAC1_RENDERER_H
#define PAG_PRAC1_RENDERER_H

#include <string>

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        float _bgRed, _bgGreen, _bgBlue;
        Renderer();
    public:
        virtual ~Renderer();
        static Renderer& getInstancia();
        void refrescar();
        void redimensionar(int width, int height);
        void cambiarColorFondo(float r, float g, float b);
        void inicializarOpenGL();
        std::string consultarOpenGL();

    };
} // PAG

#endif //PAG_PRAC1_RENDERER_H