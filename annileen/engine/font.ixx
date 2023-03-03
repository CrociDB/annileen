module;

#include <string>
#include <iostream>
#include <compare>
#include <fstream>
#include "engine/text/fontmanager.h"
#include "engine/text/textbuffermanager.h"

export module anni.font;

import anni.asset;

export namespace annileen
{
	class Font final : public AssetObject
	{
	public:
		Font(std::string filename);
		~Font();

		const TrueTypeHandle& getHandle() const noexcept;
		static void initializeFontManager();
		static void destroyFontManager() noexcept;
		static FontManager* getFontManager() noexcept;
		static TextBufferManager* getTextBufferManager() noexcept;
	
	private:
		static std::unique_ptr<FontManager> s_FontManager;
		static std::unique_ptr<TextBufferManager> s_TextBufferManager;
		TrueTypeHandle m_Handle{};
	};
}

module :private;

namespace annileen
{
	std::unique_ptr<FontManager> Font::s_FontManager{};
	std::unique_ptr<TextBufferManager> Font::s_TextBufferManager{};

	const TrueTypeHandle& Font::getHandle() const noexcept
	{ 
		return m_Handle; 
	}

	FontManager* Font::getFontManager() noexcept
	{ 
		return s_FontManager.get(); 
	}
	
	TextBufferManager* Font::getTextBufferManager() noexcept 
	{ 
		return s_TextBufferManager.get(); 
	}

	void Font::initializeFontManager()
	{
		s_FontManager = std::make_unique<FontManager>(512);
		s_TextBufferManager = std::make_unique<TextBufferManager>(s_FontManager.get());		
	}

	void Font::destroyFontManager() noexcept
	{
		s_FontManager.reset();
		s_TextBufferManager.reset();
	}

	Font::Font(std::string filename) 
	{
		// TODO: move this to File class.
		std::ifstream file;
		file.open(filename, std::ifstream::binary | std::ifstream::in);

		file.seekg(0, std::ios::end);
		size_t size{ static_cast<size_t>(file.tellg()) };
		file.seekg(0, std::ios::beg);

		uint8_t* data{ new uint8_t[size] };
		file.read(reinterpret_cast<char*>(data), size);
		file.close();

		if (data != nullptr)
		{
			m_Handle = s_FontManager->createTtf(data, static_cast<uint32_t>(size));
		}
		else
		{
			m_Handle = BGFX_INVALID_HANDLE;
		}
		delete[] data;
	}

	Font::~Font()
	{
		if (isValid(m_Handle))
		{
			s_FontManager->destroyTtf(m_Handle);
		}

		// TODO: remove
		std::cout << "Font destroyed." << std::endl;
	}
}