#pragma once

import anni.logger;

#define ANNILEEN_LOG(_log_level, _log_channel, _log_message) \
	Logger::log(_log_channel, _log_level, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_WARNING(_log_channel, _log_message) \
	Logger::log(_log_channel, LoggingLevel::Warning, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_INFO(_log_channel, _log_message) \
	Logger::log(_log_channel, LoggingLevel::Info, _log_message, __FILE__, __LINE__);
#define ANNILEEN_LOG_ERROR(_log_channel, _log_message) \
	Logger::log(_log_channel, LoggingLevel::Error, _log_message, __FILE__, __LINE__);

#define ANNILEEN_LOGF(_log_level, _log_channel, _log_message, ...) \
	Logger::logFormat(_log_channel, _log_level, __FILE__, __LINE__, _log_message, __VA_ARGS__);
#define ANNILEEN_LOGF_WARNING(_log_channel, _log_message, ...) \
	Logger::logFormat(_log_channel, LoggingLevel::Warning, __FILE__, __LINE__, _log_message, __VA_ARGS__);
#define ANNILEEN_LOGF_INFO(_log_channel, _log_message, ...) \
	Logger::logFormat(_log_channel, LoggingLevel::Info, __FILE__, __LINE__, _log_message, __VA_ARGS__);
#define ANNILEEN_LOGF_ERROR(_log_channel, _log_message, ...) \
	Logger::logFormat(_log_channel, LoggingLevel::Error, __FILE__, __LINE__, _log_message, __VA_ARGS__);

#define ANNI_LOG(_log_message) \
	ANNILEEN_LOG(annileen::LoggingLevel::Info, annileen::LoggingChannel::General, _log_message)

#define ANNI_ERROR(_log_message) \
	ANNILEEN_LOG(annileen::LoggingLevel::Error, annileen::LoggingChannel::General, _log_message)

#define ANNI_ASSERT(_condition,_log_message) \
	if (!_condition) { ANNI_ERROR(_log_message); exit(-1); }
