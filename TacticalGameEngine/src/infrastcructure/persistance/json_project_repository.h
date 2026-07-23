#pragma once

#include "../../core/interfaces/IProjectRepository.h"

#include <memory>
#include <nlohmann/json.hpp>

namespace Engine::Infrastructure::Persistence
{
    class JsonProjectRepository : public Core::Interfaces::IProjectRepository
    {
    private:
        std::shared_ptr<Core::Interfaces::ILogger> logger_;

    public:
        JsonProjectRepository(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}

        ~JsonProjectRepository() override = default;

        JsonProjectRepository(const JsonProjectRepository &) = delete;

        JsonProjectRepository &operator=(const JsonProjectRepository &) = delete;

        JsonProjectRepository(JsonProjectRepository &&other) noexcept = default;

        JsonProjectRepository &operator=(JsonProjectRepository &&other) noexcept = default;

        void save(const Core::Models::Project &project, const std::string &filepath) override;
        std::unique_ptr<Core::Models::Project> load(const std::string &filepath) override;
    };
}