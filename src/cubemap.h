#ifndef _CUBEMAP_H_
#define _CUBEMAP_H_

#include <iostream>
#include <string>
#include <vector>

class Cubemap
{
private:
    std::vector<std::string> m_Faces;
    unsigned int m_TextureId;

    void unloadTextures();

public:
    void loadTextures();
    void bindTextures();

    Cubemap(const std::vector<std::string>& faces);
    ~Cubemap();
};

#endif
