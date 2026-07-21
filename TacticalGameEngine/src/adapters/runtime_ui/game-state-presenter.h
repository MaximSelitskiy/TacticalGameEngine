#pragma once

#include "../../core/models/map.h"
#include "../../core/models/project.h"
#include "../../core/interfaces/IGamePresenter.h"

#include <iostream>

namespace Engine::Adapters::RuntimeUI
{

    class GameStatePresenter : public Core::Interfaces::IGamePresenter
    {
    private:
        void clearScreen() const;
        void drawGrid(const Core::Models::Project &project) const;
        void drawHUD(const Core::Models::Project &project) const;

    public:
        GameStatePresenter() = default;

        ~GameStatePresenter() override = default;

        GameStatePresenter(const GameStatePresenter &) = delete;

        GameStatePresenter &operator=(const GameStatePresenter &) = delete;

        GameStatePresenter(GameStatePresenter &&other) noexcept = default;

        GameStatePresenter &operator=(GameStatePresenter &&other) noexcept = default;

        void present(const Core::Models::Project &project) const override;
    };

}