//
// Created by lucar on 11/10/2026.
//

#ifndef PAG_PRAC1_CAMERA_H
#define PAG_PRAC1_CAMERA_H

#include <glm/glm.hpp>

namespace PAG {

    class Camera {
    private:
        glm::vec3 _posicion{0.0f, 0.0f, 2.0f};
        glm::vec3 _puntoMira{0.0f, 0.0f, 0.0f};
        glm::vec3 _arriba{0.0f, 1.0f, 0.0f};
        float _fovX = 60.0f;   // Ángulo de visión horizontal, en grados
        float _zNear = 0.1f;
        float _zFar = 100.0f;
        int _ancho = 1024;     // Tamaño del viewport
        int _alto = 576;

        glm::vec3 direccion() const;   // Hacia dónde mira (normalizado)
        glm::vec3 ejeU() const;        // Eje local "derecha"
        glm::vec3 ejeV() const;        // Eje local "arriba"

    public:
        Camera() = default;

        glm::mat4 matrizVision() const;
        glm::mat4 matrizProyeccion() const;

        void setViewport(int ancho, int alto);
        float getFovX() const { return _fovX; }
        void setFovX(float grados);

        // Movimientos (los ángulos se indican en grados)
        void pan(float grados);                     // + = hacia la izquierda
        void tilt(float grados);                    // + = hacia arriba
        void dolly(float derecha, float adelante);  // Traslación en el plano XZ
        void crane(float arriba);                   // Traslación en el eje Y
        void orbit(float latitud, float longitud);  // Latitud + = sube, longitud + = a la derecha
        void zoom(float grados);                    // + = abre el ángulo (aleja)
    };

}


#endif //PAG_PRAC1_CAMERA_H