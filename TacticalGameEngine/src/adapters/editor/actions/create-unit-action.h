#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>
#include <iostream>

namespace Engine::Adapters::Editor::Actions {

	class CreateUnitAction : public IEditorAction {
	public:
		CreateUnitAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}
		~CreateUnitAction() = default;
		void execute(EditorState& editor_state) override {
			int counter_ = 0;
			int unit_type_;
			std::string name_;
			auto& project = editor_state.getEditorProject();
			std::vector<std::string> types = project.Core::Models::Project::getAvailableUnitTypes();
			std::cout << "AVAILABLE UNIT TYPES:" << std::endl;
			while (counter_ != types.size()) {
				std::cout << types[counter_] << ", " << std::endl;
				counter_++;
			}
			std::cout << "WRITE NUMBER OF AVAILABLE TYPE FOR UNIT CREATION (1,2,...): " << std::endl;
			std::cin >> unit_type_;
			while (!(unit_type_ > 0 && unit_type_ <= types.size())) {
				logger_->errror("USER CHOSE WRONG TYPE");
				std::cin >> unit_type_;
			}
			logger_->info("USER CHOSE TYPE");
			std::cout << "WRITE NAME FOR UNIT CREATION: " << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, name_);
			logger_->info("USER WROTE NAME");
			auto unit = std::make_unique<Core::Models::Unit>(name_, types[unit_type_ - 1]);
			project.spawnUnit(std::move(unit));
			std::cout << "[Editor] Unit " + name_ + " succesfully spawned!" << std::endl;
			std::cout << "ALL UNITS IN WORLD:" << std::endl;
			for (const auto& u : project.getUnitsInWorld()) {
				std::cout << " - " << u->getName() << " (" << u->getType() << ")" << std::endl;
			}
		}
		std::string getName() const override { return "Create Unit"; }
	};

}