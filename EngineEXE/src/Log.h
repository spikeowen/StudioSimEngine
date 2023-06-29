#pragma once

#include "BasicIncludes.h"
#include "spdlog\spdlog.h"

using namespace spdlog;


// Macro to put a breakpoint and print out the function that failed and which line the function is on and prints out the error number
// Each function wrapped in the GLCall macro will have error checking applied

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLCLearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Clear all the errors in the error queue
static void GLCLearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// Check if there is an error
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		return false;
	}
	return true;
}

namespace QuackEngine {

	/// <summary>
	/// Static class to keep track of the loggers
	/// Engine logger is for the engine and the App logger is for the application
	/// </summary>
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<logger>& GetEngineLogger() { return m_engineLogger; }
		inline static std::shared_ptr<logger>& GetAppLogger()	 { return m_appLogger; }
	private:
		static std::shared_ptr<logger> m_engineLogger;
		static std::shared_ptr<logger> m_appLogger;
	};

}

// definitions to quickly print out engine errors
// Main difference between them is the color of the message
#define QE_ERROR(...) QuackEngine::Log::GetEngineLogger()->error(__VA_ARGS__)
#define QE_WARN(...) QuackEngine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define QE_LOG(...) QuackEngine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define QE_TRACE(...) QuackEngine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define QE_INFO(...) QuackEngine::Log::GetEngineLogger()->info(__VA_ARGS__)

// definitions to quickly print out application errors
// Main difference between them is the color of the message
#define APP_ERROR(...) QuackEngine::Log::GetAppLogger()->error(__VA_ARGS__)
#define APP_WARN(...) QuackEngine::Log::GetAppLogger()->warn(__VA_ARGS__)
#define APP_LOG(...) QuackEngine::Log::GetAppLogger()->info(__VA_ARGS__)
#define APP_TRACE(...) QuackEngine::Log::GetAppLogger()->trace(__VA_ARGS__)
#define APP_FATAL(...) QuackEngine::Log::GetAppLogger()->info(__VA_ARGS__)