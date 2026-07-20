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
        ~GameStatePresenter() override = default;

        GameStatePresenter(const GameStatePresenter &) = default;

        GameStatePresenter &operator=(const GameStatePresenter &) = default;

        GameStatePresenter(GameStatePresenter &&) noexcept = default;

        GameStatePresenter &operator=(GameStatePresenter &&) noexcept = default;
        
        void present(const Core::Models::Project &project) const override;
    };

}