#include "game-terminal-presenter.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

namespace Engine::Adapters::RuntimeUI
{
    using namespace ftxui;

    bool GameTerminalPresenter::present(const Core::Models::Project &project) const
    {
        bool should_continue = true;

        auto &map = project.getMap();
        Core::Models::Vec2 map_size = map.getSize();
        auto &units_in_world = project.getUnitsInWorld();

        std::vector<std::vector<Element>> grid_rows;
        for (int y = 0; y < map_size.y; ++y)
        {
            std::vector<Element> current_row;
            for (int x = 0; x < map_size.x; ++x)
            {
                std::string cell_text = ". ";
                bool unit_found = false;
                std::string unit_type = "";

                for (const auto &unit : units_in_world)
                {
                    if (unit && unit->isPlaced())
                    {
                        auto pos = unit->getPosition();
                        if (x == pos->x && y == pos->y)
                        {
                            char symbol = unit->getType().empty() ? 'U' : unit->getType()[0];
                            cell_text = std::string(1, symbol);
                            unit_type = unit->getType();
                            unit_found = true;
                            break;
                        }
                    }
                }

                Element cell = text(cell_text);
                if (unit_found)
                {
                    if (unit_type == "ENEMY")
                        cell = cell | color(Color::Red) | bold;
                    else if (unit_type == "FRIEND")
                        cell = cell | color(Color::Green) | bold;
                    else
                        cell = cell | color(Color::Yellow);
                }
                else
                {
                    cell = cell | color(Color::GrayDark);
                }
                current_row.push_back(cell);
            }
            grid_rows.push_back(current_row);
        }

        std::string map_title = " MAP: " + map.getName() +
                                " (" + std::to_string(map_size.x) + "x" + std::to_string(map_size.y) + ") ";

        Element map_window = window(text(map_title) | bold | color(Color::Blue), 
                                    gridbox(grid_rows) | center) | center;

        std::vector<Element> hud_elements;
        hud_elements.push_back(text("UNITS IN WORLD:") | bold | color(Color::BlueLight));

        bool any_placed = false;
        for (const auto &unit : project.getUnitsInWorld())
        {
            if (unit && unit->isPlaced())
            {
                auto pos = unit->getPosition();
                std::string info = " - [" + unit->getType() + "] " + unit->getName() +
                                   " at (" + std::to_string(pos->x) + ", " + std::to_string(pos->y) + ")";
                Element unit_text = text(info);

                if (unit->getType() == "ENEMY")
                    unit_text = unit_text | color(Color::RedLight);
                else if (unit->getType() == "FRIEND")
                    unit_text = unit_text | color(Color::GreenLight);

                hud_elements.push_back(unit_text);
                any_placed = true;
            }
        }

        if (!any_placed)
        {
            hud_elements.push_back(text(" NO UNITS PLACED ON MAP") | dim);
        }

        hud_elements.push_back(separator());
        hud_elements.push_back(text("Press Enter to continue simulation (q to exit to Editor)...") | italic);

        Element hud_content = vbox(std::move(hud_elements));
        Element limited_hud = hud_content | size(HEIGHT, LESS_THAN, 8);

        Element hud_window = window(text(" HUD ") | bold | color(Color::Blue), limited_hud);

        auto screen = ScreenInteractive::Fullscreen();

        auto component = Renderer([&]() -> Element {
            return vbox({
                map_window,
                hud_window
            }) | center;
        });

        auto event_handler = CatchEvent(component, [&](Event event) {
            if (event == Event::Return) {
                should_continue = false;
                screen.Exit();
                return true;
            }

            if (event == Event::Character("q") || event == Event::Character("Q")) {
                should_continue = false;
                screen.Exit();
                return true;
            }

            return false;
        });

        screen.Loop(event_handler);

        return should_continue;
    }
}