#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>

namespace Engine::Adapters::Editor::Actions
{

	class PlaceUnitAction : public IEditorAction
	{
	public:
		PlaceUnitAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}

		~PlaceUnitAction() = default;

		PlaceUnitAction(const PlaceUnitAction &other) = delete;

		PlaceUnitAction &operator=(const PlaceUnitAction &other) = delete;

		PlaceUnitAction(PlaceUnitAction &&other) noexcept = default;

		PlaceUnitAction &operator=(PlaceUnitAction &&other) noexcept = default;

		void execute(EditorState &editor_state) override
		{
			int counter_ = 0;
			int unit_number_;
			auto &project = editor_state.getEditorProject();
			auto &unit_pool = project.getUnitPool();
			const auto &map = project.getMap();
			const auto &size = map.getSize();
			if (unit_pool.empty())
			{
				logger_->warn("UNIT POOL IS EMPTY");
				return;
			}
			logger_->info("AVAILABLE UNITS IN POOL:");
			while (counter_ != unit_pool.size())
			{
				std::string status;
				if (unit_pool[counter_]->isPlaced())
				{
					auto pos = unit_pool[counter_]->getPosition();
					std::string statusX = "PLACED ON X: " + std::to_string(pos->x);
					std::string statusY = " Y: " + std::to_string(pos->y);
					status += statusX + statusY;
				}
				else
				{
					status = "NOT PLACED";
				}
				logger_->info(unit_pool[counter_]->getName() + " | " + unit_pool[counter_]->getType() + " | " + status);
				counter_++;
			}
			logger_->info("WRITE NUMBER OF AVAILABLE TYPE FOR UNIT PLACING (1,2,...): ");
			std::cin >> unit_number_;
			while (!(unit_number_ > 0 && unit_number_ <= unit_pool.size()))
			{
				logger_->errror("USER CHOSE WRONG TYPE");
				std::cin >> unit_number_;
			}
			logger_->info("USER CHOSE UNIT");
			logger_->info("WRITE POSITION X FOR UNIT PLACING: ");
			short x_pos;
			std::cin >> x_pos;
			while (!(x_pos > 0 && x_pos <= size.x))
			{
				logger_->errror("USER CHOSE WRONG X POSITION");
				std::cin >> x_pos;
			}
			logger_->info("WRITE POSITION Y FOR UNIT PLACING: ");
			short y_pos;
			std::cin >> y_pos;
			while (!(y_pos > 0 && y_pos <= size.y))
			{
				logger_->errror("USER CHOSE WRONG Y POSITION");
				std::cin >> y_pos;
			}
			unit_pool[unit_number_ - 1]->setPosition(x_pos, y_pos);
			auto &unit = unit_pool[unit_number_ - 1];
			project.spawnUnit(std::move(unit));
			unit_pool.erase(unit_pool.begin() + unit_number_ - 1);
			logger_->info("UNIT SUCCESFULLY PLACED!");
		}
		std::string getName() const override { return "Place Unit"; }
	};
}