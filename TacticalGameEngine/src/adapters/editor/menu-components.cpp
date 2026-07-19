#include "menu-components.h"

namespace Engine::Adapters::Editor
{

    MenuComponents::MenuComponents(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}

    void MenuComponents::registerAction(std::unique_ptr<IEditorAction> action)
    {
        if (!action)
        {
            logger_->errror("INVALID ACTION");
            return;
        }
        actions_.push_back(std::move(action));
    }

    void MenuComponents::update(EditorState &editor_state)
{
    using namespace ftxui;

    std::vector<std::string> menu_entries;
    for (const auto &action : actions_)
    {
        menu_entries.push_back(action->getName());
    }

    if (menu_entries.empty()) {
        logger_->warn("NO ACTIONS REGISTERED IN MENU");
        return;
    }

    int selected = 0;
    ftxui::MenuOption option;
    option.on_enter = [&]() {};

    auto menu_component = Menu(&menu_entries, &selected, option);

    auto renderer = Renderer(menu_component, [&]() -> Element { 
        return window(
            text(" EDITOR MENU ") | bold | color(Color::Blue) | hcenter,
            vbox({
                text("Use Up/Down arrows to navigate, Enter to select.") | dim | hcenter,
                separator(),
                menu_component->Render(),
                separator(),
                text("Status: Ready") | color(Color::GrayDark)
            })
        ) | center; 
    });

    auto screen = ScreenInteractive::TerminalOutput();

    auto event_handler = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()(); 
            return true;
        }
        if (event == Event::Escape) {
            selected = static_cast<int>(actions_.size() - 1);
            screen.ExitLoopClosure()();
            return true;
        }
        return false; 
    });

    screen.Loop(event_handler);

    size_t choice = static_cast<size_t>(selected);
    if (choice < actions_.size())
    {
        actions_[choice]->execute(editor_state);
    }
    else
    {
        logger_->warn("INVALID INDEX SELECTION");
    }
}
}