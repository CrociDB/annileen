#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL_opengl.h>

class Shader
{
private:
    std::string m_VertexCode;
    std::string m_FragmentCode;

    GLuint m_VertexShaderIndex;
    GLuint m_FragmentShaderIndex;
    GLuint m_ShaderProgram;

    std::string loadShaderCode(const char* filename);
    GLuint compileShader(std::string shaderCode, GLuint shaderType);

public:
    bool load(const char* vertexFilename, const char* fragmentFilename);
    bool use();

    Shader();
    ~Shader();
};

#endif
