#include "shader.h"

#include <fstream>
#include <streambuf>

std::string Shader::loadShaderCode(const char* filename)
{
    std::ifstream file{filename};
    if (!file.is_open())
    {
        std::cout << "ERROR LOADING FILE" << std::endl;
        return std::string();
    }

    file.seekg(0, std::ios::end);

    size_t size = file.tellg();

    std::string shaderCode(size, ' ');
    file.seekg(0, std::ios::beg);
    file.read(&shaderCode[0], size);

    file.close();

    return shaderCode;
}

GLuint Shader::compileShader(std::string shaderCode, GLuint shaderType)
{
    GLuint shaderId = glCreateShader(shaderType);
    const GLchar* code = shaderCode.c_str();
    glShaderSource(shaderId, 1, &code, NULL);
    glCompileShader(shaderId);

    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "[ERROR][SHADER] " << infoLog << std::endl;
    }

    return shaderId;
}

void Shader::load(const char* vertexFilename, const char* fragmentFilename)
{
    m_VertexCode = loadShaderCode(vertexFilename);
    m_FragmentCode = loadShaderCode(fragmentFilename);

    m_VertexShaderIndex = compileShader(m_VertexCode, GL_VERTEX_SHADER);
    m_FragmentShaderIndex = compileShader(m_FragmentCode, GL_FRAGMENT_SHADER);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, m_VertexShaderIndex);
    glAttachShader(m_ShaderProgram, m_FragmentShaderIndex);
}

bool Shader::link()
{
    glLinkProgram(m_ShaderProgram);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        std::cout << "[ERROR][PROGRAM] " << infoLog << std::endl;
    }

    return success;
}

Shader::Shader()
{
}

Shader::~Shader()
{
}
