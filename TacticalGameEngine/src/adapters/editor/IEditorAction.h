#pragma once

#include "editor-state.h"

namespace Engine::Adapters::Editor {

	class IEditorAction {
	protected:
		std::shared_ptr<Core::Interfaces::ILogger> logger_;
	public:
		IEditorAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}
		virtual ~IEditorAction() = default;
		virtual void execute(EditorState& state) = 0;
		virtual std::string getName() const = 0;
	};

}