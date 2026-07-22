#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <limits>

namespace Engine::Adapters::Editor::Actions
{

	class PlaceItemAction : public IEditorAction
	{
	public:
		PlaceItemAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}

		~PlaceItemAction() override = default;

		PlaceItemAction(const PlaceItemAction &) = delete;

		PlaceItemAction &operator=(const PlaceItemAction &) = delete;

		PlaceItemAction(PlaceItemAction &&other) noexcept = default;

		PlaceItemAction &operator=(PlaceItemAction &&other) noexcept = default;

		void execute(EditorState &editor_state) override
		{
			using namespace ftxui;

			auto &project = editor_state.getEditorProject();
			auto &item_pool = project.getItemPool();
			const auto &unit_pool = project.getUnitPool();

			if (item_pool.empty())
			{
				logger_->warn("ITEM POOL IS EMPTY");
				return;
			}
			if (unit_pool.empty())
			{
				logger_->warn("UNIT POOL IS EMPTY");
				return;
			}

			std::vector<std::string> item_names;
			for (const auto &item : item_pool)
			{
				item_names.push_back(item->getName() + " [" + item->getType() + "]");
			}

			std::vector<std::string> unit_names;
			for (const auto &unit : unit_pool)
			{
				unit_names.push_back(unit->getName() + " [" + unit->getType() + "]");
			}

			int selected_item = 0;
			int selected_unit = 0;

			MenuOption menu_opt;
			auto item_menu = Menu(&item_names, &selected_item, menu_opt);
			auto unit_menu = Menu(&unit_names, &selected_unit, menu_opt);

			auto container = Container::Vertical({item_menu,
												  unit_menu});

			auto screen = ScreenInteractive::Fullscreen();
			bool is_confirmed = false;

			auto ui_renderer = Renderer(container, [&]() -> Element
										{ return window(
													 text(" GIVE ITEM TO UNIT ") | bold | color(Color::Green) | hcenter,
													 vbox({text("Select Item:") | dim,
														   item_menu->Render() | border | size(HEIGHT, LESS_THAN, 6),
														   separator(),
														   text("Select Unit:") | dim,
														   unit_menu->Render() | border | size(HEIGHT, LESS_THAN, 6),
														   separator(),
														   text("Up/Down/Tab: Navigate | Enter: Confirm | 'q': Cancel") | dim | hcenter})) |
												 center; });

			auto event_handler = CatchEvent(ui_renderer, [&](Event event)
											{
                if (event == Event::Return) {
                    is_confirmed = true;
                    screen.Exit();
                    return true;
                }
                if (event == Event::Character("q") || event == Event::Character("Q")) {
                    is_confirmed = false;
                    screen.Exit();
                    return true;
                }
                return false; });

			screen.Loop(event_handler);

			if (!is_confirmed)
			{
				logger_->info("ACTION CANCELLED.");
				return;
			}

			auto item_it = item_pool.begin() + selected_item;
			std::unique_ptr<Core::Models::Item> moving_item = std::move(*item_it);
			item_pool.erase(item_it);

			std::string itemName = moving_item->getName();
			std::string unitName = unit_pool[selected_unit]->getName();

			unit_pool[selected_unit]->addItem(std::move(moving_item));

			logger_->info("SUCCESSFULLY MOVED ITEM '" + itemName + "' TO " + unitName + "'S INVENTORY.");
		}
		std::string getName() const override { return "Give Item to Unit"; }
	};
}