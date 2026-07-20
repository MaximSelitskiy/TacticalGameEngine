#pragma once
#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <functional>

namespace fs = std::filesystem;

namespace Engine::Adapters::Editor::Actions
{

    class LoadProjectAction : public IEditorAction
    {
    private:
        std::shared_ptr<Core::Interfaces::IProjectRepository> repo_;
        std::function<void(std::unique_ptr<Core::Models::Project>)> on_project_loaded_;
        const std::string save_folder_ = "saves";

    public:
        LoadProjectAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
                          std::shared_ptr<Core::Interfaces::IProjectRepository> repo,
                          std::function<void(std::unique_ptr<Core::Models::Project>)> on_project_loaded)
            : IEditorAction(logger), repo_(repo), on_project_loaded_(on_project_loaded) {}

        ~LoadProjectAction() override = default;

        LoadProjectAction(const LoadProjectAction &) = delete;

        LoadProjectAction &operator=(const LoadProjectAction &) = delete;

        LoadProjectAction(LoadProjectAction &&other) noexcept : IEditorAction(std::move(other.logger_)),
                                                                repo_(std::move(other.repo_)),
                                                                on_project_loaded_(std::move(other.on_project_loaded_)),
                                                                save_folder_(std::move(other.save_folder_)) {}

        LoadProjectAction &operator=(LoadProjectAction &&other) noexcept
        {
            if (this != &other)
            {
                IEditorAction::operator=(std::move(other));
                repo_ = std::move(other.repo_);
                on_project_loaded_ = std::move(other.on_project_loaded_);
            }
            return *this;
        }

        std::string getName() const override { return "Load Project from JSON"; }

        void execute(EditorState &editor_state) override
        {
            logger_->info("------ PROJECT LOADING PROCESS -----");
            std::vector<fs::path> save_files;
            if (fs::exists(save_folder_) && fs::is_directory(save_folder_))
            {
                for (const auto &entry : fs::directory_iterator(save_folder_))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".json")
                    {
                        save_files.push_back(entry.path());
                    }
                }
            }

            if (save_files.empty())
            {
                logger_->warn("NO SAVE FILES FOUND IN '" + save_folder_ + "' FOLDER.");
                return;
            }

            logger_->info("0. CANCEL LOADING");
            for (size_t i = 0; i < save_files.size(); ++i)
            {
                logger_->info(std::to_string(i + 1) + ". LOAD: " + save_files[i].filename().string());
            }

            logger_->info("CHOOSE FILE TO LOAD: ");
            size_t choice;
            if (!(std::cin >> choice) || choice > save_files.size() || choice == 0)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                logger_->info("LOADING CANCELLED.");
                return;
            }

            std::string filepath_to_load = save_files[choice - 1].string();
            try
            {
                auto loaded_project = repo_->load(filepath_to_load);
                if (loaded_project)
                {
                    logger_->info("PROJECT LOADED FROM FILE. REBOOTING STATE...");

                    if (on_project_loaded_)
                    {
                        on_project_loaded_(std::move(loaded_project));
                    }
                }
            }
            catch (const std::exception &e)
            {
                logger_->errror("CRITICAL ERROR DURING LOADING: " + std::string(e.what()));
            }
        }
    };
}