//
// Created by lucar on 28/10/2026.
//

#ifndef PAG_PRAC1_MATERIAL_H
#define PAG_PRAC1_MATERIAL_H

#include <glm/glm.hpp>

namespace PAG {
    class Material {
    private:
        glm::vec3 _ambiente{0.1f, 0.1f, 0.2f}; //azul oscuro (no recibe luz directa)
        glm::vec3 _difuso{0.2f, 0.5f, 0.9f}; //azul (el color "de verdad")
        glm::vec3 _especular{1.0f, 1.0f, 1.0f}; //blanco (es el brillo)
        float _exponente = 32.0f; //brillo medio

    public:
        const glm::vec3& getAmbiente() { return _ambiente; }
        void setAmbiente(const glm::vec3& c) { _ambiente = c; }

        const glm::vec3& getDifuso() { return _difuso; }
        void setDifuso(const glm::vec3& c) { _difuso = c; }

        const glm::vec3& getEspecular() { return _especular; }
        void setEspecular(const glm::vec3& c) { _especular = c; }

        float getExponente() { return _exponente; }
        void setExponente(float e) { _exponente = e; }
    };
}
#endif //PAG_PRAC1_MATERIAL_H
