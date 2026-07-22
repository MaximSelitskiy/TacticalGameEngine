#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
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
			using namespace ftxui;

			auto &project = editor_state.getEditorProject();
			std::vector<std::string> types = project.Core::Models::Project::getAvailableUnitTypes();

			if (types.empty())
			{
				logger_->warn("NO AVAILABLE UNIT TYPES FOR CREATION.");
				return;
			}

			int selected_type = 0;
			MenuOption menu_opt;
			auto menu_component = Menu(&types, &selected_type, menu_opt);

			std::string unit_name_input;
			InputOption input_opt;
			auto input_component = Input(&unit_name_input, "Enter unit name...", input_opt);

			auto container = Container::Vertical({menu_component,
												  input_component});

			auto screen = ScreenInteractive::Fullscreen();
			bool is_confirmed = false;

			auto ui_renderer = Renderer(container, [&]() -> Element
										{ return window(
													 text(" CREATE UNIT ") | bold | color(Color::Green) | hcenter,
													 vbox({text("Select Unit Type:") | dim,
														   separator(),
														   menu_component->Render() | border,
														   separator(),
														   text("Enter Unit Name:") | dim,
														   input_component->Render() | border,
														   separator(),
														   text("Up/Down: Choose type | Click input: Write name | Enter: Create | 'q': Cancel") | dim | hcenter})) |
												 center; });

			auto event_handler = CatchEvent(ui_renderer, [&](Event event)
											{
                if (event == Event::Return) {
                    if (unit_name_input.empty()) {
                        return true;
                    }
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
				logger_->info("UNIT CREATION CANCELLED.");
				return;
			}

			std::string chosen_type = types[selected_type];
			auto unit = std::make_unique<Core::Models::Unit>(unit_name_input, chosen_type);
			project.addUnitToPool(std::move(unit));

			logger_->info("UNIT " + unit_name_input + " SUCCESSFULLY CREATED!");
		}
		std::string getName() const override { return "Create Unit"; }
	};

}