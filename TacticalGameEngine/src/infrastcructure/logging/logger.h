#pragma once

#include "../../core/interfaces/ILogger.h"

#include <iostream>

namespace Engine::Infrastructure::Logging
{

	class LoggerConsole : public Core::Interfaces::ILogger
	{
	public:
		LoggerConsole() = default;

		~LoggerConsole() override = default;

		void info(const std::string &message) override
		{
			std::cout << "[INFO] " << message << std::endl;
		}
		void warn(const std::string &message) override
		{
			std::cout << "\033[33m[WARN] " << message << "\033[0m" << std::endl;
		}
		void errror(const std::string &message) override
		{
			std::cerr << "\033[31m[ERROR] " << message << "\033[0m" << std::endl;
		}
	};
}