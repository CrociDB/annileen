#include <engine/core/logger.h>
#include <sstream>

namespace annileen
{
	Logger::Logger()
	{
		m_Mode = LoggingMode::Console | LoggingMode::File;
		m_File = File::open(ANNILEEN_LOG_FILE, FileMode::Append | FileMode::Write);
		m_MessagesBufferSizeLimit = 1000;
	}

	constexpr void Logger::setMode(LoggingMode mode) noexcept
	{
		m_Mode = mode;
	}

	constexpr LoggingMode Logger::getMode() const noexcept
	{
		return m_Mode;
	}

	void Logger::log(LoggingChannel channel, LoggingLevel level, std::string message, std::string fileName, int line)
	{
		std::stringstream composedMessage;

		composedMessage << "[" << getLoggingLevelString(level) << "] "
			<< "[" << getLoggingChannelString(channel) << "] ";

		if (!fileName.empty())
		{
			composedMessage << "Filename: " << fileName << " (" << line << ") ";
		}
			
		composedMessage << message;

		Message newMessage;
		newMessage.m_Message = composedMessage.str();
		newMessage.m_Channel = channel;
		newMessage.m_Level = level;

		if (m_MessagesBuffer.size() == m_MessagesBufferSizeLimit)
		{
			m_MessagesBuffer.pop();
		}
		m_MessagesBuffer.push(newMessage);
		
		if ((m_Mode & LoggingMode::File) == LoggingMode::File)
		{
			m_File->writeLine(newMessage.m_Message);
		}

		if ((m_Mode & LoggingMode::Console) == LoggingMode::Console)
		{
			std::cout << "---{ANNILEEN-LOG}--- " << newMessage.m_Message << std::endl;
		}
	}

	Logger::~Logger()
	{
		m_File->close();
		delete m_File;
		m_File = nullptr;
	}
}