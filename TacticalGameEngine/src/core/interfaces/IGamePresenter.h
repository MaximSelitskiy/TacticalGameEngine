#pragma once

#include "../models/project.h"

namespace Engine::Core::Interfaces {
    class IGamePresenter {
    public:
        IGamePresenter() = default;

        virtual ~IGamePresenter() = default;

        IGamePresenter(const IGamePresenter&) = delete;

        IGamePresenter& operator=(const IGamePresenter&) = delete;

        IGamePresenter(IGamePresenter&&) = delete;

        IGamePresenter& operator=(IGamePresenter&&) = delete;

        virtual void present(const Models::Project& project) const = 0;
    };
}