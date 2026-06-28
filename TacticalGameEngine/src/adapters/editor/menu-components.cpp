#include "menu-components.h"
#include "../shared/state-manager.h"
#include <iostream>

namespace Engine::Adapters::Editor {

	MenuComponents::MenuComponents(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}

	void MenuComponents::registerAction(std::unique_ptr<IEditorAction> action) {
		if(!action){
			logger_->errror("INVALID ACTION");
			return;
		}
		actions_.push_back(std::move(action));
	}

    void MenuComponents::draw() const {
        std::cout << "\n===== EDITOR MENU =====" << std::endl;
        for (size_t i = 0; i < actions_.size(); ++i) {
            std::cout << (i + 1) << ". " << actions_[i]->getName() << std::endl;
        }
        std::cout << "-------------------------------" << std::endl;
        std::cout << "8. Start Game Simulation (Runtime)" << std::endl;
        std::cout << "0. Exit Application" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "Enter your choice: ";
    }

    void MenuComponents::update(EditorState& editor_state, Shared::StateManager& state_manager) {
        draw();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            logger_->warn("INVALID INPUT");
            return;
        }

        switch (choice) {
        case 0:
            state_manager.changeState(Shared::AppState::Exit);
            break;
        case 8:
            state_manager.changeState(Shared::AppState::Runtime);
            break;
        default:
            break;
        }

        size_t action_index = static_cast<size_t>(choice - 1);
        if (action_index < actions_.size()) {
            actions_[action_index]->execute(editor_state);
        }
        else {
            logger_->warn("INVALID INPUT/INDEX");
        }
    }
}