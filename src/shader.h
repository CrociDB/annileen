#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>

class Shader
{
private:
    std::string m_VertexCode;
    std::string m_FragCode;

    std::string loadShaderCode(const char* filename);

public:
    void load(const char* vertexFilename, const char* fragmentFilename);

    Shader();
    ~Shader();
};

#endif
