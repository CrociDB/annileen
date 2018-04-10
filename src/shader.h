#ifndef _SHADER_H_
#define _SHADER_H_

class Shader
{
public:
    void load(const char* vertexFilename, const char* fragmentFilename);

    Shader();
    ~Shader();
};

#endif
