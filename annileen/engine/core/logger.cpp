#include <engine/core/logger.h>
#include <sstream>
#include <fmt/core.h>
#include <fmt/color.h>
#include <list>

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

		composedMessage << fmt::format("[{0}] [{1}] ", getLoggingLevelString(level), getLoggingChannelString(channel));

		if (!fileName.empty())
		{
			composedMessage << fmt::format("Filename: {0} ({1}) ",fileName, line);
		}
			
		composedMessage << message;

		Message newMessage;
		newMessage.m_Message = composedMessage.str();
		newMessage.m_Channel = channel;
		newMessage.m_Level = level;

		if (m_MessagesBuffer.size() == m_MessagesBufferSizeLimit)
		{
			m_MessagesBuffer.pop_front();
		}
		m_MessagesBuffer.push_back(newMessage);
		
		if ((m_Mode & LoggingMode::File) == LoggingMode::File)
		{
			m_File->writeLine(newMessage.m_Message);
		}

		if ((m_Mode & LoggingMode::Console) == LoggingMode::Console)
		{
			switch (level)
			{
			case LoggingLevel::Error:
				fmt::print(fg(fmt::color::red), "--- ANNILEEN-LOG --- {0}\n", newMessage.m_Message);
				break;
			case LoggingLevel::Warning:
				fmt::print(fg(fmt::color::yellow), "--- ANNILEEN-LOG --- {0}\n", newMessage.m_Message);
				break;
			case LoggingLevel::Message:
				fmt::print("--- ANNILEEN-LOG --- {0}\n", newMessage.m_Message);
				break;
			}
		}
	}




	std::vector<Logger::Message> Logger::getMessagesAtLevel(LoggingLevel level) noexcept
	{
		std::vector<Logger::Message> messages;
		for (auto message : m_MessagesBuffer)
		{
			if (message.m_Level == level)
			{
				messages.push_back(message);
			}
		}
		return messages;
	}

	std::vector<Logger::Message> Logger::getMessagesAtChannel(LoggingChannel channel) noexcept
	{
		std::vector<Logger::Message> messages;
		for (auto message : m_MessagesBuffer)
		{
			if (message.m_Channel == channel)
			{
				messages.push_back(message);
			}
		}
		return messages;
	}

	std::vector<Logger::Message> Logger::getMessages(LoggingLevel level, LoggingChannel channel) noexcept
	{
		std::vector<Logger::Message> messages;
		for (auto message : m_MessagesBuffer)
		{
			if (message.m_Level == level && message.m_Channel == channel)
			{
				messages.push_back(message);
			}
		}
		return messages;
	}

	Logger::~Logger()
	{
		m_File->close();
		delete m_File;
		m_File = nullptr;
	}
}