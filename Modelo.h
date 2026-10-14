#ifndef PAG_PRAC1_MODELO_H
#define PAG_PRAC1_MODELO_H

#include<vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace PAG {

    class Modelo {
    private:
        GLuint idVAO = 0;          // Identificador del VAO
        GLuint idVBO = 0;          // Identificador del VBO de posiciones
        GLuint idVBOColores = 0;   // Identificador del VBO de colores
        GLuint idIBO = 0;          // Identificador del IBO (los índices)
        GLsizei numIndices = 0;    // Cuántos índices hay, para saber cuántos dibujar
        glm::mat4 _matrizModelado{1.0f};   // Cómo se coloca en la escena


    public:
        // 'colores' puede estar vacío; si no, debe haber un color por cada vértice
        Modelo(const std::vector<glm::vec3>& posiciones,
               const std::vector<glm::vec3>& colores,
               const std::vector<GLuint>& indices);
        ~Modelo();
    };

}

#endif //PAG_PRAC1_MODELO_H