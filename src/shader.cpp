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

void Shader::load(const char* vertexFilename, const char* fragmentFilename)
{
    m_VertexCode = loadShaderCode(vertexFilename);
    m_FragCode = loadShaderCode(fragmentFilename);
}

Shader::Shader()
{
}

Shader::~Shader()
{
}
