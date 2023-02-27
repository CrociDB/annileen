module;

#include <iostream>
#include <string>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

export module texture;

import asset;

export namespace annileen
{
	class Texture : public AssetObject
	{
	private:
		bgfx::TextureHandle m_Handle;
		bgfx::TextureInfo m_Info;
		bimg::Orientation::Enum m_Orientation;

		TextureDescriptor m_Descriptor;

	public:
		const bgfx::TextureHandle& getHandle() const { return m_Handle; }
		const bgfx::TextureInfo& getInfo() const { return m_Info; }
		const bimg::Orientation::Enum& getOrientation() const { return m_Orientation; }

		const TextureDescriptor& getDescriptor() const { return m_Descriptor; }
		void setDescriptor(TextureDescriptor descriptor) { m_Descriptor = descriptor; }

		Texture(bgfx::TextureHandle handle, bgfx::TextureInfo info, bimg::Orientation::Enum orientation);
		~Texture();
	};
}

module :private;

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
