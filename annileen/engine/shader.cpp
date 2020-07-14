#include "shader.h"

#include <fstream>
#include <streambuf>

namespace annileen
{
    Shader::Shader()
    {

    }

    Shader::~Shader()
    {
        bgfx::destroy(m_ProgramHandle);
    }

    void Shader::init(bgfx::ProgramHandle handle)
    {
        m_ProgramHandle = handle;
    }

    void Shader::use()
    {
        //glUseProgram(m_ShaderProgram);
    }

    void Shader::setBool(const std::string& name, bool value)
    {
        //glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value)
    {
        //glUniform1i(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value)
    {
        //glUniform1f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value);
    }

    void Shader::setVec2(const std::string& name, const glm::vec2& value)
    {
        //glUniform2f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value.x, value.y);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value)
    {
        //glUniform3f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value)
    {
        //glUniform4f(glGetUniformLocation(m_ShaderProgram, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& value)
    {
        //glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
}

