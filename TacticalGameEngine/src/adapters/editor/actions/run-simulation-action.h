#pragma once
#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"
#include <functional>

namespace Engine::Adapters::Editor::Actions
{

    class RunSimulationAction : public IEditorAction
    {
    private:
        std::function<void()> on_run_simulation_;

    public:
        RunSimulationAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
                            std::function<void()> on_run_simulation)
            : IEditorAction(logger), on_run_simulation_(on_run_simulation) {}

        RunSimulationAction(const RunSimulationAction &) = delete;

        RunSimulationAction &operator=(const RunSimulationAction &) = delete;

        RunSimulationAction(RunSimulationAction &&other) noexcept : IEditorAction(std::move(other.logger_)),
                                                                    on_run_simulation_(std::move(other.on_run_simulation_)) {}

        RunSimulationAction &operator=(RunSimulationAction &&other) noexcept
        {
            if (this != &other)
            {
                IEditorAction::operator=(std::move(other));
                on_run_simulation_ = std::move(other.on_run_simulation_);
            }
            return *this;
        }

        std::string getName() const override { return "Run Simulation"; }

        void execute(EditorState &editor_state) override
        {
            logger_->info("PREPARING FOR SIMULATION..");
            if (on_run_simulation_)
            {
                on_run_simulation_();
            }
        }
    };
}