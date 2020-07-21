#include "texture.h"

namespace annileen
{
	Texture::Texture(bgfx::TextureHandle handle, bgfx::TextureInfo info, bimg::Orientation::Enum orientation)
		:	m_Handle(handle),
			m_Info(info),
			m_Orientation(orientation)
	{
	}

	Texture::~Texture()
	{
	}
}
