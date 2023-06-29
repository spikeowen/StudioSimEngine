#include "pch.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace QuackEngine
{
	std::shared_ptr<logger> Log::m_engineLogger;
	std::shared_ptr<logger> Log::m_appLogger;

	void Log::Init()
	{
		set_pattern("%^[%T] %n: %v%$");

		m_engineLogger = stdout_color_mt("ENGINE");
		m_engineLogger->set_level(level::trace);

		m_appLogger = stdout_color_mt("APP");
		m_engineLogger->set_level(level::trace);
	}
}