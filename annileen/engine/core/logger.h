#pragma once

#include <string>
#include <engine/core/file.h>
#include <queue>

#define ANNILEEN_LOG_FILE "annileen-log.txt"

#define ANNILEEN_LOG(_log_level, _log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, _log_level, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_WARNING(_log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, LoggingLevel::Warning, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_MESSAGE(_log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, LoggingLevel::Message, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_ERROR(_log_channel, _log_message) \
	ServiceProvider::getLogger()->log(_log_channel, LoggingLevel::Error, _log_message, __FILE__, __LINE__);

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
		Message = 0x01,
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
	
	private:
		struct Message
		{
			std::string m_Message;
			LoggingLevel m_Level;
			LoggingChannel m_Channel;
		};

		std::string getLoggingLevelString(LoggingLevel level) noexcept
		{
			switch (level)
			{
			case LoggingLevel::Error: return "Error";
			case LoggingLevel::Message: return "Message";
			case LoggingLevel::Warning: return "Warning";
			default: return "";
			}
		}

		std::string getLoggingChannelString(LoggingChannel channel) noexcept
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

		std::queue<Message> m_MessagesBuffer;
		size_t m_MessagesBufferSizeLimit;

		// TODO: create query methods, so gui can get filtered msgs from buffer.

		// will be instantiated by Engine
		Logger();
		// will be destroyed by Engine
		~Logger();
		friend class Engine;

		LoggingMode m_Mode;
		File* m_File;
	};
}