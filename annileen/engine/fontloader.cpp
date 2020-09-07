#include <engine/fontloader.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	bx::FileReaderI* FontLoader::s_fileReader = nullptr;
	bx::AllocatorI* FontLoader::g_allocator = &s_allocator;
	bx::DefaultAllocator FontLoader::s_allocator;
	FontLoader::String FontLoader::s_currentDir;
	
	TrueTypeHandle FontLoader::load(const std::string& filename)
	{
		auto fontIt = m_CachedFonts.find(filename);

		if (fontIt != m_CachedFonts.end())
		{
			return fontIt->second;
		}

		TrueTypeHandle newFont = loadTtf(ServiceProvider::getFontManager(), filename.c_str());

		m_CachedFonts[filename] = newFont;

		return newFont;
	}

	bool FontLoader::unload(const std::string& filename)
	{
		auto fontIt = m_CachedFonts.find(filename);

		if (fontIt != m_CachedFonts.end())
		{
			if (isValid(fontIt->second))
			{
				ServiceProvider::getFontManager()->destroyTtf(fontIt->second);
				return true;
			}

			return false;
		}

		return false;
	}

	FontLoader::FontLoader() {}

	FontLoader::~FontLoader()
	{
		if (m_CachedFonts.size())
		{
			for (auto fontIt : m_CachedFonts)
			{
				if (isValid(fontIt.second))
				{
					ServiceProvider::getFontManager()->destroyTtf(fontIt.second);
				}
			}
		}
	}
}