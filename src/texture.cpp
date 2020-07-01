#include "texture.h"

bool Texture::load()
{
    m_Surface = IMG_Load(m_Filename.c_str());

    if (m_Surface == NULL)
        return false;

    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto mode = GL_RGB;
    if (m_Surface->format->BytesPerPixel == 4)
        mode == GL_RGBA;
    else if (m_Surface->format->BytesPerPixel == 1)
        mode == GL_R;
    
    glTexImage2D(GL_TEXTURE_2D, 0, mode, m_Surface->w, m_Surface->h, 0,
        mode, GL_UNSIGNED_BYTE, m_Surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    return true;
}

GLuint Texture::getId() const
{
    return m_TextureId;
}

Texture::Texture(const std::string& filename)
{
    m_Filename = filename;
    m_Surface = NULL;
}

Texture::~Texture()
{
    if (m_Surface != NULL)
    {
        SDL_FreeSurface(m_Surface);
        m_Surface = NULL;
    }
}
