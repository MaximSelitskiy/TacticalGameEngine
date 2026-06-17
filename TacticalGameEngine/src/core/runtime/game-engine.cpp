#include "game-engine.h"

#include <vector>
#include <memory>

namespace Engine::Core::Runtime {
	GameEngine::GameEngine(std::unique_ptr<Models::Project> project,
		std::shared_ptr<Interfaces::ILogger> logger)
		: active_project_(std::move(project)), logger_(logger), is_running_(false) {
		//
	}

	void GameEngine::start() {
		if (is_running_) {
			logger_->warn("ENGINE ALREADY RUNNING");
			return;
		}
		is_running_ = true;
		logger_->info("ENGINE STARTED");
	}

	void GameEngine::update() {
		if (!is_running_) {
			logger_->warn("ENGINE NOT RUNNING");
			return;
		}

		//будет апдейт тиков
	}

	void GameEngine::end() {
		if (!is_running_) return;
		is_running_ = false;
		logger_->info("ENGINE WAS STOPPED");
	}
	bool GameEngine::isActive() { return is_running_; }
}