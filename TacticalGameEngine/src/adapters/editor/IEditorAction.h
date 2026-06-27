#pragma once

#include "editor-state.h"

namespace Engine::Adapters::Editor {

	class IEditorAction {
	public:
		IEditorAction() = default;
		virtual ~IEditorAction() = default;
		virtual void execute(EditorState& state) = 0;
		//virtual std::string getName() const = 0;
	};

}