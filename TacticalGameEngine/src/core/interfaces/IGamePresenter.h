#pragma once

#include "../models/project.h"

namespace Engine::Core::Interfaces {
    class IGamePresenter {
    public:
        virtual ~IGamePresenter() = default;
        virtual void present(const Models::Project& project) const = 0;
    };
}