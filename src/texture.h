#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

class Texture
{
private:
	SDL_Surface* m_Surface;
	std::string m_Filename;
	GLuint m_TextureId;

public:
	bool load();

    GLuint getId() const;

	Texture(const std::string& filename);
	~Texture();
};

#endif
