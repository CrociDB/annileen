#pragma once

#include <iostream>
#include <string>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

#include "asset.h"

namespace annileen
{
	class Texture : public AssetObject
	{
	private:
		bgfx::TextureHandle m_Handle;
		bgfx::TextureInfo m_Info;
		bimg::Orientation::Enum m_Orientation;

	public:
		const bgfx::TextureHandle& getHandle() const { return m_Handle; }
		const bgfx::TextureInfo& getInfo() const { return m_Info; }
		const bimg::Orientation::Enum& getOrientation() const { return m_Orientation; }

		Texture(bgfx::TextureHandle handle, bgfx::TextureInfo info, bimg::Orientation::Enum orientation);
		~Texture();
	};
}
