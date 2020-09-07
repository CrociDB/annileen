#pragma once

#include <string>
#include <iostream>

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
#include <unordered_map>

#include <engine/text/fontmanager.h>

namespace annileen
{
	class FontLoader final
	{
	private:
		std::unordered_map<std::string, TrueTypeHandle> m_CachedFonts;

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

		bx::FileReaderI* getFileReader()
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
				std::cerr << "Failed to open: " << _filePath << std::endl;
			}

			if (NULL != _size)
			{
				*_size = 0;
			}

			return NULL;
		}

	public:
		TrueTypeHandle load(const std::string& filename);
		bool unload(const std::string& filename);

		FontLoader();
		~FontLoader();
	};
}