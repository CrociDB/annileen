#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <glm.hpp>

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
    void use();
    
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);

    Shader();
    ~Shader();
};

#endif
