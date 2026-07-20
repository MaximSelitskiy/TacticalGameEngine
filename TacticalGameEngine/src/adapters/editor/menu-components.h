#pragma once

#include "IEditorAction.h"
#include "../../core/interfaces/ILogger.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <memory>
#include <iostream>

namespace Engine::Adapters::Shared
{
    class StateManager;
}

namespace Engine::Adapters::Editor
{

    class MenuComponents
    {
    private:
        std::shared_ptr<Core::Interfaces::ILogger> logger_;
        std::vector<std::unique_ptr<IEditorAction>> actions_;

    public:
        MenuComponents(std::shared_ptr<Core::Interfaces::ILogger> logger);

        ~MenuComponents() = default;

        MenuComponents(const MenuComponents &) = delete;

        MenuComponents &operator=(const MenuComponents &) = delete;

        MenuComponents(MenuComponents &&other) noexcept : logger_(std::move(other.logger_)),
                                                          actions_(std::move(other.actions_)) {}

        MenuComponents &operator=(MenuComponents &&other)
        {
            if (this != &other)
            {
                logger_ = std::move(other.logger_);
                actions_ = std::move(other.actions_);
            }
            return *this;
        }
        void registerAction(std::unique_ptr<IEditorAction> action);
        void update(EditorState &editor_state);
    };

}