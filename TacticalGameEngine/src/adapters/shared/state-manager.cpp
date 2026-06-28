#include "state-manager.h"

#include <memory>

namespace Engine::Adapters::Shared {
	StateManager::StateManager(std::shared_ptr<Core::Interfaces::ILogger> logger, std::unique_ptr<Core::Models::Project> project) :
		logger_(logger),
		temporary_project_(std::move(project)),
		editor_state_(nullptr),
		game_engine_(nullptr),
		current_state_(AppState::Editor) {
		editor_menu_ = std::make_unique<Editor::MenuComponents>(logger);
		editor_menu_->registerAction(std::make_unique<Editor::Actions::CreateUnitAction>(logger_));
		editor_menu_->registerAction(std::make_unique<Editor::Actions::ManageInventoryAction>(logger_));
		editor_state_ = std::make_unique<Editor::EditorState>(std::move(temporary_project_));
	}
	void StateManager::changeState(AppState new_state) {
		if (current_state_ == new_state) return;
		//Logic of exit of state
		if (current_state_ == AppState::Editor && editor_state_) {
			temporary_project_ = editor_state_->releaseProject();
			editor_state_ = nullptr;
		}
		else if (current_state_ == AppState::Runtime && game_engine_) {
			temporary_project_ = game_engine_->releaseProject();
			game_engine_ = nullptr;
		}
		//Logic of setting new state
		switch (new_state) {
		case AppState::Editor:
			editor_state_ = std::make_unique<Editor::EditorState>(std::move(temporary_project_));
			logger_->info("EDITOR IS RUNNING");
			break;
		case AppState::Runtime:
			game_engine_ = std::make_unique<Core::Runtime::GameEngine>(std::move(temporary_project_),logger_);
			game_engine_->start();
			logger_->info("ENGINE IS RUNNNING");
			break;
		case AppState::Exit:
			if (temporary_project_) {
				logger_->info("PROJECT '" + temporary_project_->getName() + "' SAFELY DEALLOCATED");
			}
		}
	}

	void StateManager::update() {
		switch (current_state_) {
		case AppState::Editor:
			if (editor_state_) {
				editor_menu_->update(*editor_state_, *this);
			}
			break;
		case AppState::Runtime:
			if (game_engine_) {
				if (game_engine_->isActive()) {
					game_engine_->update();
				}
				else {
					logger_->info("GAME SESSION FINISHED. RETURNING TO EDITOR WINDOW");
					changeState(AppState::Editor);
				}
			}
			break;
		case AppState::Exit:
			break;
		}
	}

	bool StateManager::isExiting() const {
		return current_state_ == AppState::Exit;
	}
}