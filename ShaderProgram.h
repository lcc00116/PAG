#ifndef PAG_PRAC1_SHADERPROGRAM_H
#define PAG_PRAC1_SHADERPROGRAM_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utility>

namespace PAG {

    class ShaderProgram {
    private:
        GLuint idVS = 0;
        GLuint idFS = 0;
        GLuint idSP = 0;

        static void compilarShader(GLuint id, const std::string& etapa);
        static void enlazarPrograma(GLuint id);
        static std::string leeArchivo(const std::string& ruta);

    public:
        ShaderProgram() = default;
        ~ShaderProgram();
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&& otro) noexcept;
        ShaderProgram& operator=(ShaderProgram&& otro) noexcept;

        GLuint getId() const { return idSP; }

        void crear(const std::string& nombreBase);
        void usar() const;
        void setUniform(const std::string& nombre, const glm::mat4& m) const;
    };

}

#endif