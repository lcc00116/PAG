//
// Created by lucar on 14/10/2026.
//

#include <stdexcept>
#include "Modelo.h"

namespace PAG {
    Modelo::Modelo(const std::vector<glm::vec3>& posiciones,
               const std::vector<glm::vec3>& colores,
               const std::vector<GLuint>& indices) {
        // 1) Comprobamos los datos ANTES de tocar OpenGL
        if (posiciones.empty() || indices.empty()) {
            throw std::invalid_argument("Modelo sin vértices o sin índices");
        }
        if (!colores.empty() && colores.size() != posiciones.size()) {
            throw std::invalid_argument("Debe haber un color por cada vértice");
        }

        numIndices = static_cast<GLsizei>(indices.size());

        // 2) Creamos y activamos el VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        // 3) Creamos el VBO de posiciones y le subimos los datos
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, posiciones.size() * sizeof(glm::vec3),
                     posiciones.data(), GL_STATIC_DRAW);

        // 4) Le decimos a OpenGL cómo leerlo: atributo 0, 3 floats por vértice
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);

        // 5) VBO de colores, solo si nos han dado colores
        if (!colores.empty()) {
            glGenBuffers(1, &idVBOColores);
            glBindBuffer(GL_ARRAY_BUFFER, idVBOColores);
            glBufferData(GL_ARRAY_BUFFER, colores.size() * sizeof(glm::vec3),
                         colores.data(), GL_STATIC_DRAW);

            // Atributo 1: 3 floats por vértice (r, g, b)
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
            glEnableVertexAttribArray(1);
        }

        // 6) IBO con los índices (qué vértices forman cada triángulo)
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     indices.data(), GL_STATIC_DRAW);
    }

    Modelo::~Modelo() {
        // Solo borramos lo que se llegó a crear (un id distinto de 0)
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }
        if (idVBOColores != 0) {
            glDeleteBuffers(1, &idVBOColores);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
    }
} // PAG