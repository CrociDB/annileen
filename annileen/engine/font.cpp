#include <engine/font.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	bx::FileReaderI* Font::s_fileReader = nullptr;
	bx::AllocatorI* Font::g_allocator = &s_allocator;
	bx::DefaultAllocator Font::s_allocator;
	Font::String Font::s_currentDir;
	

	bx::FileReaderI* Font::getFileReader()
	{
		if (s_fileReader == nullptr)
		{
			s_fileReader = BX_NEW(&s_allocator, FileReader);
		}

		return s_fileReader;
	}

	void* Font::load(const char* _filePath, uint32_t* _size)
	{
		return load(getFileReader(), &s_allocator, _filePath, _size);
	}

	void* Font::load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
	{
		if (bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			void* data = BX_ALLOC(_allocator, size);
			bx::read(_reader, data, size);
			bx::close(_reader);
			if (NULL != _size)
			{
				*_size = size;
			}
			return data;
		}
		else
		{
			std::cerr << "Failed to open: " << _filePath << std::endl;
		}

		if (NULL != _size)
		{
			*_size = 0;
		}

		return NULL;
	}

	TrueTypeHandle Font::loadTtf(FontManager* _fm, const char* _filePath)
	{
		uint32_t size;
		void* data = load(_filePath, &size);

		if (NULL != data)
		{
			TrueTypeHandle handle = _fm->createTtf((uint8_t*)data, size);
			BX_FREE(&s_allocator, data);
			return handle;
		}

		TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
		return invalid;
	}


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