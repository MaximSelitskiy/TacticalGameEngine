#pragma once

#include <string>

namespace Engine::Core::Interfaces
{

	class ILogger
	{
	protected:
	ILogger(const ILogger &) = default;

		ILogger &operator=(const ILogger &) = default;

		ILogger(ILogger &&) = default;

		ILogger &operator=(ILogger &&) = default;
	public:
		ILogger() = default;

		virtual ~ILogger() = default;

		virtual void info(const std::string &message) = 0;
		virtual void warn(const std::string &message) = 0;
		virtual void errror(const std::string &message) = 0;
	};
}
