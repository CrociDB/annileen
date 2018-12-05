#include "cubemap.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

void Cubemap::unloadTextures()
{
}

GLuint Cubemap::getId() const
{
    return m_TextureId;
}

void Cubemap::load()
{
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < m_Faces.size(); i++)
    {
        SDL_Surface* surface = IMG_Load(m_Faces[i].c_str());
        auto mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
        SDL_FreeSurface(surface);
    }
}

void Cubemap::bindTextures()
{
}

Cubemap::Cubemap(const std::vector<std::string>& faces)
{
    m_Faces = faces;
}

Cubemap::~Cubemap()
{
    unloadTextures();
}
