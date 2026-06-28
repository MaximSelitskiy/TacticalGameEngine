#pragma once

#include "IEditorAction.h"
#include "../../core/interfaces/ILogger.h"

#include <vector>
#include <memory>

namespace Engine::Adapters::Shared { class StateManager; }

namespace Engine::Adapters::Editor {

    class MenuComponents {
    private:
        std::shared_ptr<Core::Interfaces::ILogger> logger_;
        std::vector<std::unique_ptr<IEditorAction>> actions_;
        void draw() const;

    public:
        MenuComponents(std::shared_ptr<Core::Interfaces::ILogger> logger);
        ~MenuComponents() = default;
        void registerAction(std::unique_ptr<IEditorAction> action);
        void update(EditorState& editor_state, Shared::StateManager& state_manager);
    };

}