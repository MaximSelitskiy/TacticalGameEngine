#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>

namespace Engine::Adapters::Editor::Actions
{

	class CreateItemTypeAction : public IEditorAction
	{
	public:
		CreateItemTypeAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}

		~CreateItemTypeAction() override = default;

		CreateItemTypeAction(const CreateItemTypeAction &other) = delete;

		CreateItemTypeAction &operator=(const CreateItemTypeAction &other) = delete;

		CreateItemTypeAction(CreateItemTypeAction &&other) noexcept = default;

		CreateItemTypeAction &operator=(CreateItemTypeAction &&other) noexcept = default;

		void execute(EditorState &editor_state) override
		{
			int counter_ = 0;
			int item_type_;
			std::string name_;
			auto &project = editor_state.getEditorProject();
			std::vector<std::string> types = project.Core::Models::Project::getAvailableItemTypes();
			logger_->info("AVAILABLE ITEM TYPES:");
			while (counter_ != types.size())
			{
				logger_->info(types[counter_] + ", ");
				counter_++;
			}
			logger_->info("WRITE NUMBER OF AVAILABLE TYPE FOR ITEM CREATION (1,2,...): ");
			std::cin >> item_type_;
			while (!(item_type_ > 0 && item_type_ <= types.size()))
			{
				logger_->errror("USER CHOSE WRONG TYPE");
				std::cin >> item_type_;
			}
			logger_->info("USER CHOSE TYPE");
			logger_->info("WRITE NAME FOR ITEM CREATION: ");
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, name_);
			logger_->info("USER WROTE NAME");
			auto item = std::make_unique<Core::Models::Item>(name_, types[item_type_ - 1]);
			project.addItemToPool(std::move(item));
			logger_->info("ITEM " + name_ + " SUCCESFULLY CREATED!");
			logger_->info("ALL ITEMS IN WORLD:");
			for (const auto &u : project.getItemsInWorld())
			{
				logger_->info(" - " + u->getName() + " (" + u->getType() + ")");
			}
		}
		std::string getName() const override { return "Manage Inventory(Create Item)"; }
	};
}