#pragma once

#include <engine/scenenodemodule.h>
#include <bx/bx.h>
#include <bx/allocator.h>
#include <bgfx/bgfx.h>

#include <bgfx/bgfx.h>
#include <bx/commandline.h>
#include <bx/endian.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <bx/string.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <engine/text/fontmanager.h>
#include <engine/text/textbuffermanager.h>


#define DBG_STRINGIZE(_x) DBG_STRINGIZE_(_x)
#define DBG_STRINGIZE_(_x) #_x
#define DBG_FILE_LINE_LITERAL "" __FILE__ "(" DBG_STRINGIZE(__LINE__) "): "
#define DBG(_format, ...) bx::debugPrintf(DBG_FILE_LINE_LITERAL "" _format "\n", ##__VA_ARGS__)

namespace annileen
{
	static const char* s_fontFilePath[] =
	{
		"assets/font/droidsans.ttf",
		"assets/font/chp-fire.ttf",
		"assets/font/bleeding_cowboys.ttf",
		"assets/font/mias_scribblings.ttf",
		"assets/font/ruritania.ttf",
		"assets/font/signika-regular.ttf",
		"assets/font/five_minutes.otf",
	};

	class Text final : public SceneNodeModule
	{

	private:

		static const uint32_t numFonts = BX_COUNTOF(s_fontFilePath);

		FontManager* m_fontManager;
		TextBufferManager* m_textBufferManager;
		TrueTypeHandle m_fontFiles[numFonts];
		FontHandle m_fonts[numFonts];

		FontHandle m_visitor10;
		TrueTypeHandle m_fontAwesomeTtf;
		TrueTypeHandle m_fontKenneyTtf;
		FontHandle m_fontAwesome72;
		FontHandle m_fontKenney64;
		TrueTypeHandle m_visitorTtf;

		TextBufferHandle m_transientText;
		TextBufferHandle m_staticText;


		static bx::DefaultAllocator s_allocator;
		static bx::FileReaderI* s_fileReader;

		static bx::AllocatorI* g_allocator;
		typedef bx::StringT<&g_allocator> String;

		static String s_currentDir;

		class FileReader : public bx::FileReader
		{
			typedef bx::FileReader super;

		public:
			virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
			{
				String filePath(s_currentDir);
				filePath.append(_filePath);
				return super::open(filePath.getPtr(), _err);
			}
		};

		static bx::FileReaderI* getFileReader()
		{
			if (s_fileReader == nullptr)
			{
				s_fileReader = BX_NEW(&s_allocator, FileReader);
			}

			return s_fileReader;
		}

		void* load(const char* _filePath, uint32_t* _size)
		{
			return load(getFileReader(), &s_allocator, _filePath, _size);
		}

		TrueTypeHandle loadTtf(FontManager* _fm, const char* _filePath)
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

		void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
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
				DBG("Failed to open: %s.", _filePath);
			}

			if (NULL != _size)
			{
				*_size = 0;
			}

			return NULL;
		}

	public:
		
		void init();
		void shutdown();

		void update(bgfx::ViewId viewId, uint16_t width, uint16_t height);

		Text();
		~Text();
	};

	typedef Text* TextPtr;
}