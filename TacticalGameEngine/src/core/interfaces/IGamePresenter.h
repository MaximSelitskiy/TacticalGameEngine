#pragma once

#include "../models/project.h"

namespace Engine::Core::Interfaces
{
    class IGamePresenter
    {
    protected:
        IGamePresenter(const IGamePresenter &) = default;

        IGamePresenter &operator=(const IGamePresenter &) = default;

        IGamePresenter(IGamePresenter &&) = default;

        IGamePresenter &operator=(IGamePresenter &&) = default;

    public:
        IGamePresenter() = default;

        virtual ~IGamePresenter() = default;

        virtual bool present(const Models::Project &project) const = 0;
    };
}