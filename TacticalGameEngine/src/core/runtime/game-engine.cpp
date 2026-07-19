#include "game-engine.h"

#include <vector>
#include <memory>
#include <iostream>
#include <limits>

namespace Engine::Core::Runtime
{
	GameEngine::GameEngine(std::unique_ptr<Models::Project> project,
						   std::shared_ptr<Interfaces::ILogger> logger,
						   std::shared_ptr<Interfaces::IGamePresenter> presenter)
		: active_project_(std::move(project)), logger_(logger), is_running_(false), presenter_(presenter) {}

	void GameEngine::start()
	{
		if (is_running_)
		{
			logger_->warn("ENGINE ALREADY RUNNING");
		}
		is_running_ = true;
		logger_->info("ENGINE STARTED");
		// if (std::cin.peek() == '\n')
		// {
		// 	std::cin.ignore();
		// }
	}

	void GameEngine::update()
	{
		if (!is_running_)
		{
			logger_->warn("ENGINE NOT RUNNING");
			return;
		}
		presenter_->present(*active_project_);

		std::string input;
		std::getline(std::cin, input);
		if (input == "0")
		{
			is_running_ = false;
		}
		return;
	}

	void GameEngine::end()
	{
		if (!is_running_)
			return;
		is_running_ = false;
		logger_->info("ENGINE WAS STOPPED");
	}
	bool GameEngine::isActive() { return is_running_; }
	std::unique_ptr<Core::Models::Project> GameEngine::releaseProject()
	{
		return std::move(active_project_);
	}
}