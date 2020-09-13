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

#include <engine/asset.h>
#include <engine/text/fontmanager.h>

namespace annileen
{
	class Font final : public AssetObject
	{
	private:

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

		bx::FileReaderI* getFileReader();
		void* load(const char* _filePath, uint32_t* _size);
		void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);
		TrueTypeHandle loadTtf(FontManager* _fm, const char* _filePath);

		TrueTypeHandle m_Handle;
	public:
		const TrueTypeHandle& getHandle() const { return m_Handle; }

		Font(std::string filename);
		~Font();
	};
}