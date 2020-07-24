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
