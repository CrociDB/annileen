#pragma once

#include<string>
#include<iostream>
#include<fstream>
#include<engine/definitions.h>

namespace annileen
{
	enum class FileMode
	{
		Write = 0x01,
		Read = 0x02,
		AtEnd = 0x04,
		Append = 0x08,
		Replace = 0x10,
		Binary = 0x20
	};
	ANNILEEN_ENABLE_BITMASK_OPERATORS(FileMode);

	class File final
	{
	public:
		
	public:
		static File* open(std::string name, FileMode mode);
		
		bool isOpen();
		void close();

		void writeLine(std::string line); // overload << operator?

		~File();

	private:
		File(std::string name, FileMode mode);

		std::string m_Name;
		FileMode m_Mode;
		std::fstream m_File;
	};
}