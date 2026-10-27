#ifndef PAG_PRAC1_MODELO_H
#define PAG_PRAC1_MODELO_H

#include<vector>
#include<utility>
#include<string>

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
        //'colores' puede estar vacío; si no, debe haber un color por cada vértice
        Modelo(const std::vector<glm::vec3>& posiciones,
               const std::vector<glm::vec3>& colores,
               const std::vector<GLuint>& indices);
        ~Modelo();
        //Un Modelo no se copia (liberaría dos veces los mismos buffers), pero sí se mueve
        Modelo(const Modelo&) = delete;
        Modelo& operator=(const Modelo&) = delete;
        Modelo(Modelo&& otro) noexcept;
        Modelo& operator=(Modelo&& otro) noexcept;
        static Modelo triangulo();   //El triángulo de colores de las prácticas anteriores (te queremos, triángulo)
        static Modelo desdeArchivo(const std::string& ruta);   //Carga un OBJ con Assimp
        void dibujar() const;
        const glm::mat4& getMatrizModelado() const { return _matrizModelado; }
        void setMatrizModelado(const glm::mat4& m) { _matrizModelado = m; }

    };

}

#endif //PAG_PRAC1_MODELO_H