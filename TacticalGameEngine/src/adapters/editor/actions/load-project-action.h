#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <filesystem>
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
            using namespace ftxui;
            std::vector<std::string> save_files;
            if (fs::exists(save_folder_) && fs::is_directory(save_folder_))
            {
                for (const auto &entry : fs::directory_iterator(save_folder_))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".json")
                    {
                        save_files.push_back(entry.path().filename().string());
                    }
                }
            }

            if (save_files.empty())
            {
                logger_->warn("NO SAVE FILES FOUND IN '" + save_folder_ + "' FOLDER.");
                return;
            }

            int selected = 0;
            ftxui::MenuOption option;
            option.on_enter = [&]() {};

            auto menu_component = Menu(&save_files, &selected, option);

            auto screen = ScreenInteractive::Fullscreen();

            bool is_confirmed = false;

            auto event_handler = CatchEvent(menu_component, [&](Event event)
                                            {
        if (event == Event::Return) {
            is_confirmed = true;
            screen.ExitLoopClosure()(); 
            return true;
        }

        if (event == Event::Character("Q") || event == Event::Character("q")) {
            is_confirmed = false;   
            screen.ExitLoopClosure()();
            return true;
        }
        return false; });

            auto renderer = Renderer(event_handler, [&]() -> Element
                                     { return window(
                                                  text(" LOADING PROJECT ") | bold | color(Color::Green) | hcenter,
                                                  vbox({text("Use Up/Down arrows to navigate, Enter to select, q to exit.") | dim | hcenter,
                                                        separator(),
                                                        menu_component->Render(),
                                                        separator(),
                                                        text("Status: Ready") | color(Color::GrayDark)})) |
                                              center; });

            screen.Loop(renderer);

            if (!is_confirmed)
            {
                logger_->info("FILE SELECTION CANCELLED. RETURNING TO MAIN MENU.");
                return;
            }

            size_t choice = static_cast<size_t>(selected);

            std::string filename = save_files[choice];

            std::string filepath_to_load = (fs::path(save_folder_) / filename).string();
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