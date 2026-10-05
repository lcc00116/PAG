#include "ShaderProgram.h"

#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>

namespace PAG {

ShaderProgram::~ShaderProgram() {
    if (idVS != 0) {
        glDeleteShader(idVS);
    }
    if (idFS != 0) {
        glDeleteShader(idFS);
    }
    if (idSP != 0) {
        glDeleteProgram(idSP);
    }
}

void ShaderProgram::compilarShader(GLuint id, const std::string& etapa) {
    glCompileShader(id);

    GLint ok = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE) {
        GLint longitud = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitud);
        std::vector<GLchar> log(longitud > 0 ? longitud : 1);
        glGetShaderInfoLog(id, static_cast<GLsizei>(log.size()), nullptr, log.data());
        throw std::runtime_error("Error compilando el " + etapa + ":\n" + log.data());
    }
}

void ShaderProgram::enlazarPrograma(GLuint id) {
    glLinkProgram(id);

    GLint ok = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (ok == GL_FALSE) {
        GLint longitud = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &longitud);
        std::vector<GLchar> log(longitud > 0 ? longitud : 1);
        glGetProgramInfoLog(id, static_cast<GLsizei>(log.size()), nullptr, log.data());
        throw std::runtime_error("Error enlazando el shader program:\n" + std::string(log.data()));
    }
}

std::string ShaderProgram::leeArchivo(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    std::ostringstream contenido;
    contenido << archivo.rdbuf();

    return contenido.str();
}

void ShaderProgram::crear(const std::string& nombreBase) {

    // Si ya había un programa cargado, lo liberamos antes de crear uno nuevo
    if (idVS != 0) { glDeleteShader(idVS); idVS = 0; }
    if (idFS != 0) { glDeleteShader(idFS); idFS = 0; }
    if (idSP != 0) { glDeleteProgram(idSP); idSP = 0; }
    
    std::string miVertexShader = leeArchivo(nombreBase + "-vs.glsl");
    std::string miFragmentShader = leeArchivo(nombreBase + "-fs.glsl");

    idVS = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* fuenteVS = miVertexShader.c_str();
    glShaderSource(idVS, 1, &fuenteVS, nullptr);
    compilarShader(idVS, "vertex shader");

    idFS = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fuenteFS = miFragmentShader.c_str();
    glShaderSource(idFS, 1, &fuenteFS, nullptr);
    compilarShader(idFS, "fragment shader");

    idSP = glCreateProgram();
    glAttachShader(idSP, idVS);
    glAttachShader(idSP, idFS);
    enlazarPrograma(idSP);
}

void ShaderProgram::usar() const {
    glUseProgram(idSP);
}

}