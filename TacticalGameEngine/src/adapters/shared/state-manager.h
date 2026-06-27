#pragma once

#include "../editor/editor-state.h"
#include "../../core/runtime/game-engine.h"
namespace Engine::Adapters::Shared {
	enum class AppState {
		Editor,
		Runtime
	};

	class StateManager {
	private:
		AppState current_state_;
		std::shared_ptr<Core::Interfaces::ILogger> logger_;
		std::unique_ptr<Editor::EditorState> editor_state_;
		std::unique_ptr<Core::Runtime::GameEngine> game_engine_;
		std::unique_ptr<Core::Models::Project> temporary_project_;
	public:
		StateManager(std::shared_ptr<Core::Interfaces::ILogger> logger, std::unique_ptr<Core::Models::Project> project) :
			logger_(logger), temporary_project_(std::move(project));
		~StateManager() = default;
		void changeState(AppState new_state);
		void update();
		bool isExiting() const;
	};
	
}