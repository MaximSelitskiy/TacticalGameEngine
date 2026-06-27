#pragma once
#include "../../core/models/project.h"
#include <memory>
namespace Engine::Adapters::Editor {

	class EditorState {
	private:
		std::unique_ptr <Core::Models::Project> active_project_;

	public:
		EditorState(std::unique_ptr <Core::Models::Project> project) : active_project_(std::move(project)) {}
		const Core::Models::Project& getConstProject() const { return *active_project_; }
		Core::Models::Project& getEditorProject() { return *active_project_; }
		std::unique_ptr<Core::Models::Project> releaseProject() {
			return std::move(active_project_);
		}
	};

}