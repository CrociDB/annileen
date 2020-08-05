#pragma once

#include <string>
#include <list>
#include <vector>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <engine/core/file.h>
#include <engine/serviceprovider.h>

#define ANNILEEN_LOG_FILE "annileen-log.txt"

#define ANNILEEN_LOG(_log_level, _log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, _log_level, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_WARNING(_log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, LoggingLevel::Warning, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_INFO(_log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, LoggingLevel::Info, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_ERROR(_log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, LoggingLevel::Error, _log_message, __FILE__, __LINE__);

#define ANNILEEN_LOGF(_log_level, _log_channel, _log_message, ...) \
	ServiceProvider::getLogger()->logFormat(_log_channel, _log_level, __FILE__, __LINE__, _log_message, __VA_ARGS__);
#define ANNILEEN_LOGF_WARNING(_log_channel, _log_message, ...) \
	ServiceProvider::getLogger()->logFormat(_log_channel, LoggingLevel::Warning, __FILE__, __LINE__, _log_message, __VA_ARGS__);
#define ANNILEEN_LOGF_INFO(_log_channel, _log_message, ...) \
	ServiceProvider::getLogger()->logFormat(_log_channel, LoggingLevel::Info, __FILE__, __LINE__, _log_message, __VA_ARGS__);
#define ANNILEEN_LOGF_ERROR(_log_channel, _log_message, ...) \
	ServiceProvider::getLogger()->logFormat(_log_channel, LoggingLevel::Error, __FILE__, __LINE__, _log_message, __VA_ARGS__);

namespace annileen
{
	enum class LoggingMode
	{
		Console = 0x01,
		File = 0x02
	};
	ANNILEEN_ENABLE_BITMASK_OPERATORS(LoggingMode)

	enum class LoggingLevel
	{
		Info = 0x01,
		Warning = 0x02,
		Error = 0x04
	};
	ANNILEEN_ENABLE_BITMASK_OPERATORS(LoggingLevel)

	enum class LoggingChannel
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
	ANNILEEN_ENABLE_BITMASK_OPERATORS(LoggingChannel)

	class Logger final
	{
	public:
		constexpr void setMode(LoggingMode mode) noexcept;
		constexpr LoggingMode getMode() const noexcept;

		void log(LoggingChannel channel, LoggingLevel level, std::string message, std::string fileName = "", int line = 0);
		
		template <typename S, typename... Args>
		inline void logFormat(LoggingChannel channel, LoggingLevel level, std::string fileName, int line, const S& format_str, Args&&... args)
		{
			std::string message = fmt::format(format_str, args...);
			log(channel, level, message, fileName, line);
		}
	
		struct Message
		{
			std::string m_Message;
			LoggingLevel m_Level;
			LoggingChannel m_Channel;
		};
	private:

		const char* getLoggingLevelString(LoggingLevel level) noexcept
		{
			switch (level)
			{
			case LoggingLevel::Error: return "Error";
			case LoggingLevel::Info: return "Info";
			case LoggingLevel::Warning: return "Warning";
			default: return "";
			}
		}

		const char* getLoggingChannelString(LoggingChannel channel) noexcept
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

		std::vector<LoggingLevel> getLoggingLevelsList() noexcept
		{
			return m_LoggingLevelsList;
		}

		std::vector<LoggingChannel> getLoggingChannelsList() noexcept
		{
			return m_LoggingChannelsList;
		}
		 
		// For editor console filtering
		std::vector<Message> getMessagesAtLevel(LoggingLevel level) noexcept;
		std::vector<Message> getMessagesAtChannel(LoggingChannel channel) noexcept;
		std::vector<Message> getMessages(LoggingLevel level, LoggingChannel channel) noexcept;
		std::vector<Message> getAllMessages() noexcept;

		// will be instantiated by Engine
		Logger();
		// will be destroyed by Engine
		~Logger();

		std::vector<LoggingLevel> m_LoggingLevelsList = { LoggingLevel::Error , LoggingLevel::Info, LoggingLevel::Warning };
		std::vector<LoggingChannel> m_LoggingChannelsList = { LoggingChannel::Core, LoggingChannel::Renderer, LoggingChannel::Physics,
			LoggingChannel::Input, LoggingChannel::Editor, LoggingChannel::Asset, LoggingChannel::AI, LoggingChannel::General };

		std::list<Message> m_MessagesBuffer;
		size_t m_MessagesBufferSizeLimit;
		LoggingMode m_Mode;
		File* m_File;
		
		// To allow for use ctor/dtor
		friend class Engine;
		// To allow for getting console messages
		friend class EditorGui;
	};
}