#include <engine/core/file.h>

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