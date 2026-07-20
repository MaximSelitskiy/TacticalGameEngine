#pragma once

#include "../../core/models/map.h"
#include "../../core/models/project.h"
#include "../../core/interfaces/IGamePresenter.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <string>
#include <vector>

namespace Engine::Adapters::RuntimeUI
{

    class GameTerminalPresenter : public Core::Interfaces::IGamePresenter
    {
    public:

        GameTerminalPresenter() = default;

        ~GameTerminalPresenter() override = default;

        GameTerminalPresenter(const GameTerminalPresenter &) = default;

        GameTerminalPresenter &operator=(const GameTerminalPresenter &) = default;

        GameTerminalPresenter(GameTerminalPresenter &&) noexcept = default;

        GameTerminalPresenter &operator=(GameTerminalPresenter &&) noexcept = default;

        void present(const Core::Models::Project &project) const override;
    };
}