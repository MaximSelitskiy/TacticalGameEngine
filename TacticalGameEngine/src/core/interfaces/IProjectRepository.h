#pragma once

#include "../models/project.h"

namespace Engine::Core::Interfaces
{
	class IProjectRepository
	{
	public:
		IProjectRepository() = default;

		virtual ~IProjectRepository() = default;

		IProjectRepository(const IProjectRepository &) = delete;

		IProjectRepository &operator=(const IProjectRepository &) = delete;

		IProjectRepository(IProjectRepository &&) = delete;

		IProjectRepository &operator=(IProjectRepository &&) = delete;

		virtual void save(const Models::Project &project, const std::string &filepath) = 0;
		// можно и unique использовать, но тогда у сохранения будет возможность изменять данные
		virtual std::unique_ptr<Models::Project> load(const std::string &filepath) = 0;
	};
}