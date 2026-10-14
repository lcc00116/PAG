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

    Modelo::Modelo(Modelo&& otro) noexcept
    : idVAO(std::exchange(otro.idVAO, 0)),
      idVBO(std::exchange(otro.idVBO, 0)),
      idVBOColores(std::exchange(otro.idVBOColores, 0)),
      idIBO(std::exchange(otro.idIBO, 0)),
      numIndices(std::exchange(otro.numIndices, 0)),
      _matrizModelado(otro._matrizModelado) {}

    Modelo& Modelo::operator=(Modelo&& otro) noexcept {
        if (this != &otro) {
            // Intercambiamos: lo que tenía este objeto lo liberará 'otro' al destruirse
            std::swap(idVAO, otro.idVAO);
            std::swap(idVBO, otro.idVBO);
            std::swap(idVBOColores, otro.idVBOColores);
            std::swap(idIBO, otro.idIBO);
            std::swap(numIndices, otro.numIndices);
            std::swap(_matrizModelado, otro._matrizModelado);
        }
        return *this;
    }
    void Modelo::dibujar() const {
        // El IBO está guardado dentro del VAO, así que basta con activar el VAO
        glBindVertexArray(idVAO);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    }

    Modelo Modelo::triangulo() {
        std::vector<glm::vec3> posiciones = { {-0.5f, -0.5f, 0.0f},
                                              { 0.5f, -0.5f, 0.0f},
                                              { 0.0f,  0.5f, 0.0f} };

        std::vector<glm::vec3> colores = { {1.0f, 0.0f, 0.0f},
                                           {0.0f, 1.0f, 0.0f},
                                           {0.0f, 0.0f, 1.0f} };

        std::vector<GLuint> indices = { 0, 1, 2 };

        return Modelo(posiciones, colores, indices);
    }
} // PAG