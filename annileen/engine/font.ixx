module;

#include <string>
#include <iostream>
#include <compare>
#include <fstream>
#include "engine/text/fontmanager.h"
#include "engine/text/textbuffermanager.h"

export module font;

import asset;

export namespace annileen
{
	class Font final : public AssetObject
	{
	private:
		static FontManager* s_FontManager;
		static TextBufferManager* s_TextBufferManager;

		TrueTypeHandle m_Handle;
	public:
		const TrueTypeHandle& getHandle() const { return m_Handle; }

		Font(std::string filename);
		~Font();

		static void initializeFontManager();
		static void destroyFontManager();

		static FontManager* getFontManager() { return s_FontManager; }
		static TextBufferManager* getTextBufferManager() { return s_TextBufferManager; }
	};
}

module :private;

namespace annileen
{
	FontManager* Font::s_FontManager = nullptr;
	TextBufferManager* Font::s_TextBufferManager = nullptr;

	void Font::initializeFontManager()
	{
		delete s_FontManager;
		s_FontManager = new FontManager(512);

		delete s_TextBufferManager;
		s_TextBufferManager = new TextBufferManager(s_FontManager);		
	}

	void Font::destroyFontManager()
	{
		delete s_FontManager;
		s_FontManager = nullptr;

		delete s_TextBufferManager;
		s_TextBufferManager;
	}

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
			m_Handle = s_FontManager->createTtf(data, static_cast<uint32_t>(size));
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
			s_FontManager->destroyTtf(m_Handle);
		}
	}
}