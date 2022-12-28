module;

#include<string>
#include<iostream>
#include<fstream>

export module file;

export namespace annileen
{
	enum class FileMode : char
	{
		Write = 0x01,
		Read = 0x02,
		AtEnd = 0x04,
		Append = 0x08,
		Replace = 0x10,
		Binary = 0x20
	};

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

namespace annileen
{
	File::File(std::string name, FileMode mode)
	{
		m_Name = name;
		m_Mode = mode;
		m_File.open(name, static_cast<std::ios_base::openmode>(mode));
	}

	File::~File()
	{
		close();
	}

	File* File::open(std::string name, FileMode mode)
	{
		return new File(name, mode);
	}

	bool File::isOpen()
	{
		return m_File.is_open();
	}

	void File::close()
	{
		if (isOpen())
		{
			m_File.close();
		}
	}

	void File::writeLine(std::string line)
	{
		if(isOpen() && ((int)m_Mode & (int)FileMode::Write))
		{
			m_File << line << std::endl;
		}
	}
}