module;

#include <iostream>
#include <string>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

export module cubemap;

import asset;

export namespace annileen
{
	class Cubemap : public AssetObject
	{
	private:
		bgfx::TextureHandle m_Handle;
		bgfx::TextureInfo m_Info;
		bimg::Orientation::Enum m_Orientation;

	public:
		const bgfx::TextureHandle& getHandle() const { return m_Handle; }
		const bgfx::TextureInfo& getInfo() const { return m_Info; }
		const bimg::Orientation::Enum& getOrientation() const { return m_Orientation; }

		Cubemap(bgfx::TextureHandle handle, bgfx::TextureInfo info, bimg::Orientation::Enum orientation);
		~Cubemap();
	};
}

module :private;

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
		// TODO: remove
		std::cout << "Cubemap destroyed." << std::endl;
	}
}
