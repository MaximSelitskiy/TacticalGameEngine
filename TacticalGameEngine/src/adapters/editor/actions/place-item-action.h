#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <iostream>
#include <vector>
#include <limits>

namespace Engine::Adapters::Editor::Actions {

	class PlaceItemAction : public IEditorAction {
	public:
		PlaceItemAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}
		~PlaceItemAction() = default;

		void execute(EditorState& editor_state) override {
			auto& project = editor_state.getEditorProject();
			//auto& item_pool = const_cast<std::vector<std::unique_ptr<Core::Models::Item>>&>(project.getItemPool()); 
			auto& item_pool = project.getItemPool();
			const auto& unit_pool = project.getUnitPool();

			if (item_pool.empty()) {
				logger_->warn("ITEM POOL IS EMPTY");
				return;
			}
			if (unit_pool.empty()) {
				logger_->warn("UNIT POOL IS EMPTY");
				return;
			}

			std::cout << "\n--- AVAILABLE ITEMS IN POOL ---" << std::endl;
			for (size_t i = 0; i < item_pool.size(); ++i) {
				std::cout << (i + 1) << ". " << item_pool[i]->getName()
					<< " [" << item_pool[i]->getType() << "]" << std::endl;
			}
			std::cout << "SELECT ITEM TO ASSIGN (1-" << item_pool.size() << "): ";
			size_t item_choice;
			while (!(std::cin >> item_choice) || item_choice < 1 || item_choice > item_pool.size()) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				logger_->warn("INVALID CHOICE!");
			}
			std::cout << "\n--- AVAILABLE UNITS ---" << std::endl;
			for (size_t i = 0; i < unit_pool.size(); ++i) {
				std::cout << (i + 1) << ". " << unit_pool[i]->getName() << std::endl;
			}
			std::cout << "SELECT UNIT (1-" << unit_pool.size() << "): ";
			size_t unit_choice;
			while (!(std::cin >> unit_choice) || unit_choice < 1 || unit_choice > unit_pool.size()) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				logger_->warn("INVALID CHOICE!");
			}
			auto item_it = item_pool.begin() + (item_choice - 1);
			std::unique_ptr<Core::Models::Item> moving_item = std::move(*item_it);
			item_pool.erase(item_it);
			std::string itemName = moving_item->getName(); 
				std::string unitName = unit_pool[unit_choice - 1]->getName();
			unit_pool[unit_choice - 1]->addItem(std::move(moving_item));
			logger_->info("SUCCESFULLY MOVED ITEM '" + itemName + "' TO " + unitName + "'S INVENTORY.");
		}
		std::string getName() const override { return "Give Item to Unit"; }
	};
}