#ifndef _CUBEMAP_H_
#define _CUBEMAP_H_

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

class Cubemap
{
private:
    std::vector<std::string> m_Faces;
    GLuint m_TextureId;

    void unloadTextures();

public:
    GLuint getId() const;

    void load();
    void bindTextures();

    Cubemap(const std::vector<std::string>& faces);
    ~Cubemap();
};

#endif
