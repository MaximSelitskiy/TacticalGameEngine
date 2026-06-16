#pragma once
#include <string>

namespace Engine::Core::Interfaces {

	class ILogger {
	public:
		ILogger() = default;
		~ILogger() = default;
		virtual void info(const std::string& message)  = 0;
		virtual void warn(const std::string& message)  = 0;
		virtual void errror(const std::string& message)  = 0;
	};
}
