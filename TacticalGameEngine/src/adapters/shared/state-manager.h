#pragma once

#include "../editor/actions/create-unit-action.h"
#include "../editor/actions/create-item-action.h"
#include "../editor/actions/resize-map-action.h"
#include "../editor/actions/place-unit-action.h"
#include "../editor/actions/place-item-action.h"
#include "../editor/actions/save-project-action.h"
#include "../editor/actions/load-project-action.h"
#include "../editor/actions/run-simulation-action.h"
#include "../editor/actions/exit-app-action.h"
#include "../editor/editor-state.h"
#include "../editor/menu-components.h"
#include "../../core/runtime/game-engine.h"
#include "../runtime_ui/game-terminal-presenter.h"

#include <memory>

namespace Engine::Adapters::Shared
{
	enum class AppState
	{
		Editor,
		Runtime,
		Exit
	};

	class StateManager
	{
	private:
		AppState current_state_;
		std::shared_ptr<Core::Interfaces::ILogger> logger_;
		std::unique_ptr<Editor::EditorState> editor_state_;
		std::unique_ptr<Core::Runtime::GameEngine> game_engine_;
		std::unique_ptr<Core::Models::Project> temporary_project_;
		std::unique_ptr<Editor::MenuComponents> editor_menu_;

	public:
		StateManager(std::shared_ptr<Core::Interfaces::ILogger> logger,
					 std::unique_ptr<Core::Models::Project> project,
					 std::shared_ptr<Core::Interfaces::IProjectRepository> repo);

		~StateManager() = default;

		StateManager(const StateManager &) = delete;

		StateManager &operator=(const StateManager &) = delete;

		StateManager(StateManager &&other) noexcept
			: current_state_(other.current_state_),
			  logger_(std::move(other.logger_)),
			  editor_state_(std::move(other.editor_state_)),
			  game_engine_(std::move(other.game_engine_)),
			  temporary_project_(std::move(other.temporary_project_)),
			  editor_menu_(std::move(other.editor_menu_)) {}

		StateManager &operator=(StateManager &&other) noexcept
		{
			if (this != &other)
			{
				current_state_ = other.current_state_;
				logger_ = std::move(other.logger_);
				editor_state_ = std::move(other.editor_state_);
				game_engine_ = std::move(other.game_engine_);
				temporary_project_ = std::move(other.temporary_project_);
				editor_menu_ = std::move(other.editor_menu_);
			}
			return *this;
		}

		void changeState(AppState new_state);
		void loadNewProject(std::unique_ptr<Core::Models::Project> new_project);
		void update();
		bool isExiting() const;
	};

}