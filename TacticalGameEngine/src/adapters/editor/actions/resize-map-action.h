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

    class ResizeMapAction : public IEditorAction
    {
    public:
        ResizeMapAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}

        ~ResizeMapAction() override = default;

        ResizeMapAction(const ResizeMapAction &) = delete;

        ResizeMapAction &operator=(const ResizeMapAction &) = delete;

        ResizeMapAction(ResizeMapAction &&other) noexcept = default;

        ResizeMapAction &operator=(ResizeMapAction &&other) noexcept = default;

        void execute(EditorState &editor_state) override
        {
            using namespace ftxui;

            auto &project = editor_state.getEditorProject();
            auto &current_map = project.getMap();
            Core::Models::Vec2 current_size = current_map.getSize();

            int new_width = current_size.x;
            int new_height = current_size.y;

            auto width_slider = Slider("Width: ", &new_width, 1, 100, 1);
            auto height_slider = Slider("Height: ", &new_height, 1, 100, 1);

            auto container = Container::Vertical({width_slider,
                                                  height_slider});

            auto screen = ScreenInteractive::Fullscreen();
            bool is_confirmed = false;

            auto ui_renderer = Renderer(container, [&]() -> Element
                                        { return window(
                                                     text(" RESIZE MAP ") | bold | color(Color::Green) | hcenter,
                                                     vbox({text("Current Map: " + current_map.getName() + " [" + std::to_string(current_size.x) + "x" + std::to_string(current_size.y) + "]") | dim | hcenter,
                                                           separator(),
                                                           text("Set New Dimensions:") | dim,
                                                           width_slider->Render(),
                                                           height_slider->Render(),
                                                           separator(),
                                                           text("Up/Down: Switch | Left/Right: Adjust | Enter: Confirm | 'q': Cancel") | dim | hcenter})) |
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
                logger_->info("MAP RESIZE CANCELLED.");
                return;
            }

            logger_->info("REGENERATING MAP WITH NEW SIZE...");
            auto new_map = std::make_unique<Core::Models::Map>(current_map.getName(), new_width, new_height);
            project.setMap(std::move(new_map));

            logger_->info("MAP RESIZED TO " + std::to_string(new_width) + "x" + std::to_string(new_height) + "!");
        }
        std::string getName() const override { return "Resize Map"; }
    };

}