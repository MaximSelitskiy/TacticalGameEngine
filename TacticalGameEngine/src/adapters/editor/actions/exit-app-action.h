#pragma once
#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"
#include <functional> 


namespace Engine::Adapters::Editor::Actions {

    class ExitAppAction : public IEditorAction {
    private:
        std::function<void()> on_exit_app_;

    public:
        ExitAppAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
            std::function<void()> on_exit_app)
            : IEditorAction(logger), on_exit_app_(on_exit_app) {}

        std::string getName() const override { return "Run Simulation"; }

        void execute(EditorState& editor_state) override {
            logger_->info("EXITING APP");
            if(on_exit_app_){
                on_exit_app_();
            }
        }
    };
}