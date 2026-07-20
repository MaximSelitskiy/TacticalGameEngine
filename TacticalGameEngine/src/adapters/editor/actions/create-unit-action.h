#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>
#include <iostream>

namespace Engine::Adapters::Editor::Actions
{

	class CreateUnitTypeAction : public IEditorAction
	{
	public:
		CreateUnitTypeAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}

		~CreateUnitTypeAction() override = default;

		CreateUnitTypeAction(const CreateUnitTypeAction &) = delete;

		CreateUnitTypeAction &operator=(const CreateUnitTypeAction &) = delete;

		CreateUnitTypeAction(CreateUnitTypeAction &&other) noexcept = default;

		CreateUnitTypeAction &operator=(CreateUnitTypeAction &&other) noexcept = default;

		void execute(EditorState &editor_state) override
		{
			int counter_ = 0;
			int unit_type_;
			std::string name_;
			auto &project = editor_state.getEditorProject();
			std::vector<std::string> types = project.Core::Models::Project::getAvailableUnitTypes();
			logger_->info("AVAILABLE UNIT TYPES:");
			while (counter_ != types.size())
			{
				logger_->info(types[counter_] + ", ");
				counter_++;
			}
			logger_->info("WRITE NUMBER OF AVAILABLE TYPE FOR UNIT CREATION (1,2,...): ");
			std::cin >> unit_type_;
			while (!(unit_type_ > 0 && unit_type_ <= types.size()))
			{
				logger_->errror("USER CHOSE WRONG TYPE");
				std::cin >> unit_type_;
			}
			logger_->info("USER CHOSE TYPE");
			logger_->info("WRITE NAME FOR UNIT CREATION: ");
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, name_);
			logger_->info("USER WROTE NAME");
			auto unit = std::make_unique<Core::Models::Unit>(name_, types[unit_type_ - 1]);
			project.addUnitToPool(std::move(unit));
			logger_->info("UNIT " + name_ + " SUCCEFULLY CREATED!");
			logger_->info("ALL UNITS IN WORLD:");
			for (const auto &u : project.getUnitsInWorld())
			{
				logger_->info(" - " + u->getName() + " (" + u->getType() + ")");
			}
		}
		std::string getName() const override { return "Create Unit"; }
	};

}