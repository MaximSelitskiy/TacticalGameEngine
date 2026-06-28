#pragma once

#include "../../core/models/project.h"
#include "../../core/interfaces/IGamePresenter.h"

namespace Engine::Adapters::RuntimeUI {

    class GameStatePresenter : public Core::Interfaces::IGamePresenter {
    private:
        void clearScreen() const;
        void drawGrid(const Core::Models::Project& project) const;
        void drawHUD(const Core::Models::Project& project) const;

    public:
        GameStatePresenter() = default;
        ~GameStatePresenter() = default;
        void present(const Core::Models::Project& project) const override;
    };

}