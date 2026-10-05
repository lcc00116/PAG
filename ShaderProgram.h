#ifndef PAG_PRAC1_SHADERPROGRAM_H
#define PAG_PRAC1_SHADERPROGRAM_H

#include <string>
#include <glad/glad.h>

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

        void crear(const std::string& nombreBase);
        void usar() const;
    };

}

#endif