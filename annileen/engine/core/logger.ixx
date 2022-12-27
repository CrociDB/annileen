module;

#include <iostream>
#include <format>
#include <list>
#include <string>
#include <vector>

export module logger;

import file;

export namespace annileen
{
	enum class LoggingMode : int
	{
		Console = 0x01,
		File = 0x02
	};
	
	enum class LoggingLevel : int
	{
		Info = 0x01,
		Warning = 0x02,
		Error = 0x04
	};

	enum class LoggingChannel : int
	{
		Core = 0x01,
		Renderer = 0x02,
		Physics = 0x04,
		Input = 0x08,
		Editor = 0x10,
		Asset = 0x20,
		AI = 0x40,
		General = 0x80
	};

	class Logger final
	{
	public:
		inline static std::string logFileName{ "annileen-log.txt" };

		static void setMode(LoggingMode mode) noexcept;
		static LoggingMode getMode() noexcept;

		static void log(LoggingChannel channel, LoggingLevel level, std::string message, std::string fileName = "", int line = 0);

		template <typename S, typename... Args>
		inline static void logFormat(LoggingChannel channel, LoggingLevel level, std::string fileName, int line, const S& format_str, Args&&... args)
		{
			std::string message = std::vformat(format_str, std::make_format_args(args...));
			log(channel, level, message, fileName, line);
		}

		struct Message
		{
			std::string m_Message;
			LoggingLevel m_Level;
			LoggingChannel m_Channel;
		};
	private:

		static const char* getLoggingLevelString(LoggingLevel level) noexcept
		{
			switch (level)
			{
			case LoggingLevel::Error: return "Error";
			case LoggingLevel::Info: return "Info";
			case LoggingLevel::Warning: return "Warning";
			default: return "";
			}
		}

		static const char* getLoggingChannelString(LoggingChannel channel) noexcept
		{
			switch (channel)
			{
			case LoggingChannel::AI: return "AI";
			case LoggingChannel::Asset: return "Asset";
			case LoggingChannel::Core: return "Core";
			case LoggingChannel::Editor: return "Editor";
			case LoggingChannel::General: return "General";
			case LoggingChannel::Input: return "Input";
			case LoggingChannel::Physics: return "Physics";
			case LoggingChannel::Renderer: return "Renderer";
			default: return "";
			}
		}

		static std::vector<LoggingLevel> getLoggingLevelsList() noexcept
		{
			return m_LoggingLevelsList;
		}

		static std::vector<LoggingChannel> getLoggingChannelsList() noexcept
		{
			return m_LoggingChannelsList;
		}

		// For editor console filtering
		static std::vector<Message> getMessagesAtLevel(LoggingLevel level) noexcept;
		static std::vector<Message> getMessagesAtChannel(LoggingChannel channel) noexcept;
		static std::vector<Message> getMessages(LoggingLevel level, LoggingChannel channel) noexcept;
		static std::vector<Message> getAllMessages() noexcept;
		static void clearMessages() noexcept;

		Logger() = delete;
		~Logger() = delete;

		inline static std::vector<LoggingLevel> m_LoggingLevelsList = { LoggingLevel::Error , LoggingLevel::Info, LoggingLevel::Warning };
		inline static std::vector<LoggingChannel> m_LoggingChannelsList = { LoggingChannel::Core, LoggingChannel::Renderer, LoggingChannel::Physics,
			LoggingChannel::Input, LoggingChannel::Editor, LoggingChannel::Asset, LoggingChannel::AI, LoggingChannel::General };

		static std::list<Message> m_MessagesBuffer;
		static size_t m_MessagesBufferSizeLimit;
		static LoggingMode m_Mode;
		static File* m_File;

		// will be instantiated by Engine
		static void initialize();
		// will be destroyed by Engine
		static void destroy();

		// To allow for use ctor/dtor
		friend class Engine;
		// To allow for getting console messages
		friend class EditorGui;
	};
}

namespace annileen
{
	std::list<Logger::Message> Logger::m_MessagesBuffer{};
	size_t Logger::m_MessagesBufferSizeLimit{};
	LoggingMode Logger::m_Mode{};
	File* Logger::m_File{};

	void Logger::initialize()
	{
		using loggingModeType = typename std::underlying_type<LoggingMode>::type;
		m_Mode = static_cast<LoggingMode>(static_cast<loggingModeType>(LoggingMode::Console) | static_cast<loggingModeType>(LoggingMode::File));

		using fileModeType = typename std::underlying_type<FileMode>::type;
		m_File = File::open(logFileName, static_cast<FileMode>(static_cast<fileModeType>(FileMode::Append) | static_cast<fileModeType>(FileMode::Write)));
		m_MessagesBufferSizeLimit = 1000;
	}

	void Logger::setMode(LoggingMode mode) noexcept
	{
		m_Mode = mode;
	}

	LoggingMode Logger::getMode() noexcept
	{
		return m_Mode;
	}

	void Logger::log(LoggingChannel channel, LoggingLevel level, std::string message, std::string fileName, int line)
	{
		std::string composedMessage;

		composedMessage = std::format("[{0}] [{1}] ", getLoggingLevelString(level), getLoggingChannelString(channel));

		if (!fileName.empty())
		{
			composedMessage += std::format("Filename: {0} ({1}) ",fileName, line);
		}
			
		composedMessage += message;

		Message newMessage;
		newMessage.m_Message = std::move(composedMessage);
		newMessage.m_Channel = channel;
		newMessage.m_Level = level;

		if (m_MessagesBuffer.size() == m_MessagesBufferSizeLimit)
		{
			m_MessagesBuffer.pop_front();
		}
		m_MessagesBuffer.push_back(newMessage);
		
		using loggingModeType = typename std::underlying_type<LoggingMode>::type;

		if (static_cast<LoggingMode>(static_cast<loggingModeType>(m_Mode) & static_cast<loggingModeType>(LoggingMode::File)) == LoggingMode::File)
		{
			m_File->writeLine(newMessage.m_Message);
		}

		if (static_cast<LoggingMode>(static_cast<loggingModeType>(m_Mode) & static_cast<loggingModeType>(LoggingMode::Console)) == LoggingMode::Console)
		{
			std::cout << std::format("--- ANNILEEN-LOG --- {0}\n", newMessage.m_Message);			
		}
	}

	std::vector<Logger::Message> Logger::getMessagesAtLevel(LoggingLevel level) noexcept
	{
		std::vector<Logger::Message> messages;
		for (auto& message : m_MessagesBuffer)
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
		for (auto& message : m_MessagesBuffer)
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
		for (auto& message : m_MessagesBuffer)
		{
			if (message.m_Level == level && message.m_Channel == channel)
			{
				messages.push_back(message);
			}
		}
		return messages;
	}

	std::vector<Logger::Message> Logger::getAllMessages() noexcept
	{
		std::vector<Logger::Message> messages;
		for (auto& message : m_MessagesBuffer)
		{
			messages.push_back(message);
		}
		return messages;
	}

	void Logger::clearMessages() noexcept
	{
		m_MessagesBuffer.clear();
	}

	void Logger::destroy()
	{
		m_File->close();
		delete m_File;
		m_File = nullptr;
	}
}
