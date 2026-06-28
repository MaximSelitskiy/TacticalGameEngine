#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>
#include <iostream>

namespace Engine::Adapters::Editor::Actions {

	class ManageInventoryAction : public IEditorAction {
	public:
		ManageInventoryAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}
		~ManageInventoryAction() = default;
		void execute(EditorState& editor_state) override {
			int counter_ = 0;
			int item_type_;
			std::string name_;
			auto& project = editor_state.getEditorProject();
			std::vector<std::string> types = project.Core::Models::Project::getAvailableItemTypes();
			std::cout << "AVAILABLE ITEM TYPES:" << std::endl;
			while (counter_ != types.size()) {
				std::cout << types[counter_] << ", " << std::endl;
				counter_++;
			}
			std::cout << "WRITE NUMBER OF AVAILABLE TYPE FOR ITEM CREATION (1,2,...): " << std::endl;
			std::cin >> item_type_;
			while (!(item_type_ > 0 && item_type_ <= types.size())) {
				logger_->errror("USER CHOSE WRONG TYPE");
				std::cin >> item_type_;
			}
			logger_->info("USER CHOSE TYPE");
			std::cout << "WRITE NAME FOR ITEM CREATION: " << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, name_);
			logger_->info("USER WROTE NAME");
			auto item = std::make_unique<Core::Models::Item>(name_, types[item_type_ - 1]);
			project.spawnItem(std::move(item));
			std::cout << "[Editor] Item " + name_ + " succesfully spawned!" << std::endl;
			std::cout << "ALL ITEMS IN WORLD:" << std::endl;
			for (const auto& u : project.getItemsInWorld()) {
				std::cout << " - " << u->getName() << " (" << u->getType() << ")" << std::endl;
			}
		}
		std::string getName() const override { return "Manage Inventory(Create Item)"; }
	private:
		std::shared_ptr <Core::Interfaces::ILogger> logger_;
	};
}