//
// Created by lucar on 11/10/2026.
//

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

namespace PAG {

    namespace {
        // Rota el vector v 'angulo' radianes alrededor de 'eje'
        glm::vec3 rotar(const glm::vec3& v, float angulo, const glm::vec3& eje) {
            return glm::vec3(glm::rotate(glm::mat4(1.0f), angulo, eje) * glm::vec4(v, 0.0f));
        }

        // Elevación máxima permitida, para evitar mirar recto arriba o abajo
        const float LIMITE_ELEVACION = glm::radians(89.0f);
    }

    glm::vec3 Camera::direccion() const {
        return glm::normalize(_puntoMira - _posicion);
    }

    glm::vec3 Camera::ejeU() const {
        return glm::normalize(glm::cross(direccion(), _arriba));
    }

    glm::vec3 Camera::ejeV() const {
        return glm::cross(ejeU(), direccion());
    }

    glm::mat4 Camera::matrizVision() const {
        return glm::lookAt(_posicion, _puntoMira, _arriba);
    }

    glm::mat4 Camera::matrizProyeccion() const {
        float aspecto = static_cast<float>(_ancho) / static_cast<float>(_alto);
        // glm::perspective pide el ángulo vertical; lo calculamos a partir del horizontal
        float fovX = glm::radians(_fovX);
        float fovY = 2.0f * std::atan(std::tan(fovX / 2.0f) / aspecto);
        return glm::perspective(fovY, aspecto, _zNear, _zFar);
    }

    void Camera::setViewport(int ancho, int alto) {
        if (ancho > 0 && alto > 0) {   // Una ventana minimizada llega con tamaño 0
            _ancho = ancho;
            _alto = alto;
        }
    }

    void Camera::setFovX(float grados) {
        _fovX = glm::clamp(grados, 5.0f, 120.0f);
    }

    void Camera::pan(float grados) {
        glm::vec3 d = _puntoMira - _posicion;
        d = rotar(d, glm::radians(grados), ejeV());
        _puntoMira = _posicion + d;
    }

    void Camera::tilt(float grados) {
        glm::vec3 d = _puntoMira - _posicion;
        float dist = glm::length(d);
        if (dist < 1e-6f) return;

        float altura = std::asin(glm::clamp(d.y / dist, -1.0f, 1.0f));
        float nueva = glm::clamp(altura + glm::radians(grados), -LIMITE_ELEVACION, LIMITE_ELEVACION);

        d = rotar(d, nueva - altura, ejeU());
        _puntoMira = _posicion + d;
    }

    void Camera::dolly(float derecha, float adelante) {
        glm::vec3 f = _puntoMira - _posicion;
        f.y = 0.0f;                                   // Nos quedamos en el plano XZ
        if (glm::length(f) < 1e-6f) return;
        f = glm::normalize(f);
        glm::vec3 r = glm::normalize(glm::cross(f, _arriba));

        glm::vec3 desplazamiento = r * derecha + f * adelante;
        _posicion += desplazamiento;
        _puntoMira += desplazamiento;                 // El punto al que mira también se mueve
    }

    void Camera::crane(float arriba) {
        glm::vec3 desplazamiento = _arriba * arriba;
        _posicion += desplazamiento;
        _puntoMira += desplazamiento;
    }

    void Camera::orbit(float latitud, float longitud) {
        glm::vec3 r = _posicion - _puntoMira;         // Del punto de mira a la cámara
        float dist = glm::length(r);
        if (dist < 1e-6f) return;

        // Longitud: giro alrededor del eje Y de la escena
        r = rotar(r, glm::radians(longitud), _arriba);

        // Latitud: giro alrededor del eje u, limitando la elevación de la cámara
        float altura = std::asin(glm::clamp(r.y / dist, -1.0f, 1.0f));
        float nueva = glm::clamp(altura + glm::radians(latitud), -LIMITE_ELEVACION, LIMITE_ELEVACION);

        glm::vec3 eje = glm::cross(-r, _arriba);
        if (glm::length(eje) > 1e-6f) {
            r = rotar(r, -(nueva - altura), glm::normalize(eje));
        }

        _posicion = _puntoMira + r;
    }

    void Camera::zoom(float grados) {
        setFovX(_fovX + grados);
    }

}