#include <engine/font.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	Font::Font(std::string filename) 
	{
		// TODO: move this to File class.
		std::ifstream file;
		file.open(filename, std::ifstream::binary | std::ifstream::in);

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		uint8_t* data = new uint8_t[size];
		file.read(reinterpret_cast<char*>(data), size);
		file.close();

		if (data != nullptr)
		{
			m_Handle = ServiceProvider::getFontManager()->createTtf(data, static_cast<uint32_t>(size));
			delete[] data;
		}
		else
		{
			m_Handle = BGFX_INVALID_HANDLE;
		}
	}

	Font::~Font()
	{
		if (isValid(m_Handle))
		{
			ServiceProvider::getFontManager()->destroyTtf(m_Handle);
		}
	}
}