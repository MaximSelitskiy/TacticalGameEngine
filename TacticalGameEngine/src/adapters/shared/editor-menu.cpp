#include "editor-menu.h"
#include "state-manager.h"

namespace Engine::Adapters::Shared {

    void EditorMenu::draw() const {
        std::cout << "\n================ EDITOR MENU ================" << std::endl;
        std::cout << "1. Create Unit" << std::endl;
        std::cout << "2. Manage Inventory (Create Item)" << std::endl;
        std::cout << "3. Start Game Simulation (Runtime)" << std::endl;
        std::cout << "0. Exit Application" << std::endl;
        std::cout << "==============================================" << std::endl;
        std::cout << "Enter your choice: ";
    }

    EditorMenu::EditorMenu(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}

	void EditorMenu::update(Editor::EditorState& editor_state, StateManager& state_manager) {
        draw();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            logger_->warn("INVALID INPUT");
            return;
        }

        switch (choice) {
        case 1: {
            Editor::Actions::CreateUnitAction action(logger_);
            action.execute(editor_state);
            break;
        }
        case 2: {
            Editor::Actions::ManageInventoryAction action(logger_);
            action.execute(editor_state);
            break;
        }
        case 3:
            state_manager.changeState(AppState::Runtime);
            break;
        case 0:
            state_manager.changeState(AppState::Exit);
            break;
        default:
            logger_->warn("UNKNOWN INPUT");
            break;
        }
    }
}