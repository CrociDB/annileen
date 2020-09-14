#pragma once

#include <string>
#include <iostream>

#include <engine/asset.h>
#include <engine/text/fontmanager.h>

namespace annileen
{
	class Font final : public AssetObject
	{
	private:
		TrueTypeHandle m_Handle;
	public:
		const TrueTypeHandle& getHandle() const { return m_Handle; }

		Font(std::string filename);
		~Font();
	};
}