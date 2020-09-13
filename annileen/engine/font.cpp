#include <engine/font.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	bx::FileReaderI* Font::s_fileReader = nullptr;
	bx::AllocatorI* Font::g_allocator = &s_allocator;
	bx::DefaultAllocator Font::s_allocator;
	Font::String Font::s_currentDir;
	
	Font::Font(std::string filename) 
	{
		m_Handle = loadTtf(ServiceProvider::getFontManager(), filename.c_str());
	}

	Font::~Font()
	{
		if (isValid(m_Handle))
		{
			ServiceProvider::getFontManager()->destroyTtf(m_Handle);
		}
	}
}