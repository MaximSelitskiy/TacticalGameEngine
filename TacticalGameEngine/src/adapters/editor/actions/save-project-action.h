#pragma once
#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"
#include <iostream>

namespace Engine::Adapters::Editor::Actions {
    class SaveProjectAction : public IEditorAction {
    private:
        std::shared_ptr<Core::Interfaces::IProjectRepository> repo_;
    public:
        SaveProjectAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
            std::shared_ptr<Core::Interfaces::IProjectRepository> repo)
            : IEditorAction(logger), repo_(repo) {}

        std::string getName() const override { return "Save Project to JSON"; }

        void execute(EditorState& editor_state) override {
            std::string filename;
            std::cout << "ENTER FILE NAME TO SAVE (e.g., save.json): ";
            std::cin >> filename;

            repo_->save(editor_state.getEditorProject(), filename);
        }
    };
}