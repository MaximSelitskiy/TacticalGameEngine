#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>

namespace Engine::Adapters::Editor::Actions {

	class PlaceUnitAction : public IEditorAction {
	public:
		PlaceUnitAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}
		~PlaceUnitAction() = default;
		void execute(EditorState& editor_state) override {
			int counter_ = 0;
			int unit_number_;
			auto& project = editor_state.getEditorProject();
			const auto& unit_pool = project.getUnitPool();
			if (unit_pool.empty()) {
				logger_->warn("UNIT POOL IS EMPTY");
				return;
			}
			logger_->info("AVAILABLE UNITS IN POOL:");
			while (counter_ != unit_pool.size()) {
				std::string status;
				if (unit_pool[counter_]->isPlaced()) {
					auto pos = unit_pool[counter_]->getPosition();
					std::string statusX = "PLACED ON X: " + pos->x;
					std::string statusY = " Y: " + pos->y;
					status += statusX + statusY;
				}
				else {
					status = "NOT PLACED";
				}
				logger_->info(unit_pool[counter_]->getName() + " | " + unit_pool[counter_]->getType() + " | "
					+ status);
				counter_++;
			}
			logger_->info("WRITE NUMBER OF AVAILABLE TYPE FOR UNIT PLACING (1,2,...): ");
			std::cin >> unit_number_;
			while (!(unit_number_ > 0 && unit_number_ <= unit_pool.size())) {
				logger_->errror("USER CHOSE WRONG TYPE");
				std::cin >> unit_number_;
			}
			logger_->info("USER CHOSE UNIT");
			logger_->info("WRITE NAME FOR ITEM CREATION: ");
			
		}
		std::string getName() const override { return "Place Unit"; }
	};
}