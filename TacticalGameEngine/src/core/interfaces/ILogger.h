#pragma once

#include <string>

namespace Engine::Core::Interfaces
{

	class ILogger
	{
	public:
		ILogger() = default;

		virtual ~ILogger() = default;

		ILogger(const ILogger &) = delete;

		ILogger &operator=(const ILogger &) = delete;

		ILogger(ILogger &&) = delete;

		ILogger &operator=(ILogger &&) = delete;

		virtual void info(const std::string &message) = 0;
		virtual void warn(const std::string &message) = 0;
		virtual void errror(const std::string &message) = 0;
	};
}
