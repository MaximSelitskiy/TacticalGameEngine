#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
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

		PlaceUnitAction(const PlaceUnitAction &) = delete;

		PlaceUnitAction &operator=(const PlaceUnitAction &) = delete;

		PlaceUnitAction(PlaceUnitAction &&other) noexcept = default;

		PlaceUnitAction &operator=(PlaceUnitAction &&other) noexcept = default;

		void execute(EditorState &editor_state) override
		{
			using namespace ftxui;

			auto &project = editor_state.getEditorProject();
			auto &unit_pool = project.getUnitPool();
			const auto &map = project.getMap();
			const auto &size = map.getSize();

			if (unit_pool.empty())
			{
				logger_->warn("UNIT POOL IS EMPTY");
				return;
			}

			std::vector<std::string> unit_names;
			for (const auto &unit : unit_pool)
			{
				std::string status = unit->isPlaced() ? " [PLACED]" : " [NOT PLACED]";
				unit_names.push_back(unit->getName() + " | " + unit->getType() + status);
			}

			int selected_unit = 0;
			MenuOption menu_opt;
			auto unit_menu = Menu(&unit_names, &selected_unit, menu_opt);

			int x_pos = 1;
			int y_pos = 1;

			auto x_slider = Slider("X Position: ", &x_pos, 1, size.x, 1);
			auto y_slider = Slider("Y Position: ", &y_pos, 1, size.y, 1);

			auto container = Container::Vertical({unit_menu,
												  x_slider,
												  y_slider});

			auto screen = ScreenInteractive::TerminalOutput();
			bool is_confirmed = false;

			auto ui_renderer = Renderer(container, [&]() -> Element
										{ return window(
													 text(" PLACE UNIT ON MAP ") | bold | color(Color::Green) | hcenter,
													 vbox({text("Select Unit:") | dim,
														   unit_menu->Render() | border | ftxui::size(HEIGHT, LESS_THAN, 6),
														   separator(),
														   text("Set Coordinates:") | dim,
														   x_slider->Render(),
														   y_slider->Render(),
														   separator(),
														   text("Up/Down/Tab: Navigate | Left/Right: Adjust | Enter: Place | 'q': Cancel") | dim | hcenter})) |
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

			unit_pool[selected_unit]->setPosition(x_pos, y_pos);
			auto &unit = unit_pool[selected_unit];
			project.spawnUnit(std::move(unit));
			unit_pool.erase(unit_pool.begin() + selected_unit);

			logger_->info("UNIT SUCCESSFULLY PLACED AT X:" + std::to_string(x_pos) + " Y:" + std::to_string(y_pos) + "!");
		}
		std::string getName() const override { return "Place Unit"; }
	};
}