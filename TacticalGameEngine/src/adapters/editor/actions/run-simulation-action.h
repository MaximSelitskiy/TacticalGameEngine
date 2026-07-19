#pragma once
#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"
#include <functional> 


namespace Engine::Adapters::Editor::Actions {

    class RunSimulationAction : public IEditorAction {
    private:
        std::function<void()> on_run_simulation_;

    public:
        RunSimulationAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
            std::function<void()> on_run_simulation)
            : IEditorAction(logger), on_run_simulation_(on_run_simulation) {}

        std::string getName() const override { return "Run Simulation"; }

        void execute(EditorState& editor_state) override {
            logger_->info("PREPARING FOR SIMULATION..");
            if(on_run_simulation_){
                on_run_simulation_();
            }
        }
    };
}