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

	class CreateItemTypeAction : public IEditorAction
	{
	public:
		CreateItemTypeAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}

		~CreateItemTypeAction() override = default;

		CreateItemTypeAction(const CreateItemTypeAction &) = delete;

		CreateItemTypeAction &operator=(const CreateItemTypeAction &) = delete;

		CreateItemTypeAction(CreateItemTypeAction &&other) noexcept = default;

		CreateItemTypeAction &operator=(CreateItemTypeAction &&other) noexcept = default;

		void execute(EditorState &editor_state) override
		{
			using namespace ftxui;

			auto &project = editor_state.getEditorProject();
			std::vector<std::string> types = project.getAvailableItemTypes();

			if (types.empty())
			{
				logger_->warn("NO AVAILABLE ITEM TYPES FOR CREATION.");
				return;
			}

			int selected_type = 0;
			MenuOption menu_opt;
			auto menu_component = Menu(&types, &selected_type, menu_opt);

			std::string item_name_input;
			InputOption input_opt;
			auto input_component = Input(&item_name_input, "Enter item name...", input_opt);

			auto container = Container::Vertical({menu_component,
												  input_component});

			auto screen = ScreenInteractive::Fullscreen();
			bool is_confirmed = false;

			auto ui_renderer = Renderer(container, [&]() -> Element
										{ return window(
													 text(" CREATE NEW ITEM ") | bold | color(Color::Green) | hcenter,
													 vbox({text("Select Item Type:") | dim,
														   separator(),
														   menu_component->Render() | border,
														   separator(),
														   text("Enter Item Name:") | dim,
														   input_component->Render() | border,
														   separator(),
														   text("Use Up/Down to choose type | Click input to write name | Enter: Create |'q': Cancel") | dim | hcenter})) |
												 center; });

			auto event_handler = CatchEvent(ui_renderer, [&](Event event)
											{
        if (event == Event::Return) {
            if (item_name_input.empty()) {
                return true; 
            }
            is_confirmed = true;
            screen.Exit();
            return true;
        }

        if (event == Event::Character("Q") || event == Event::Character("q")) {
            is_confirmed = false;
            screen.Exit();
            return true;
        }

        return false; });

			screen.Loop(event_handler);

			if (!is_confirmed)
			{
				logger_->info("ITEM CREATION CANCELLED. RETURNING TO MAIN MENU.");
				return;
			}

			std::string chosen_type = types[selected_type];
			auto item = std::make_unique<Core::Models::Item>(item_name_input, chosen_type);
			project.addItemToPool(std::move(item));

			logger_->info("ITEM " + item_name_input + " (" + chosen_type + ") SUCCESSFULLY CREATED!");
		}
		std::string getName() const override { return "Manage Inventory(Create Item)"; }
	};
}