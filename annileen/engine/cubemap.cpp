#include "cubemap.h"

namespace annileen
{
	Cubemap::Cubemap(bgfx::TextureHandle handle, bgfx::TextureInfo info, bimg::Orientation::Enum orientation)
		: m_Handle(handle),
		m_Info(info),
		m_Orientation(orientation)
	{
	}

	Cubemap::~Cubemap()
	{
	}
}
