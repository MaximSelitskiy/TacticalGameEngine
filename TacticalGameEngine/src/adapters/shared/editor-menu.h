#pragma once

#include "../editor/editor-state.h"
#include "../editor/actions/create-unit-action.h"
#include "../editor/actions/manage_inventory_action.h"

#include <iostream>
#include <limits>
#include <memory>

namespace Engine::Adapters::Shared {
    class StateManager;
    class EditorMenu {
    private:
        std::shared_ptr<Core::Interfaces::ILogger> logger_;
        void draw() const;
    public:
        EditorMenu(std::shared_ptr<Core::Interfaces::ILogger> logger);

        void update(Editor::EditorState& editor_state, StateManager& state_manager);
    };
}