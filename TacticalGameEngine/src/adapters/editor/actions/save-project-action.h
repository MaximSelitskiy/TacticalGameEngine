#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace Engine::Adapters::Editor::Actions
{
    class SaveProjectAction : public IEditorAction
    {
    private:
        std::shared_ptr<Core::Interfaces::IProjectRepository> repo_;
        const std::string save_folder_ = "saves";

    public:
        SaveProjectAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
                          std::shared_ptr<Core::Interfaces::IProjectRepository> repo)
            : IEditorAction(logger), repo_(repo) {}

        ~SaveProjectAction() override = default;

        SaveProjectAction(const SaveProjectAction &) = delete;

        SaveProjectAction &operator=(const SaveProjectAction &) = delete;

        SaveProjectAction(SaveProjectAction &&other) noexcept : IEditorAction(std::move(other.logger_)),
                                                                repo_(std::move(other.repo_)),
                                                                save_folder_(std::move(other.save_folder_)) {}

        SaveProjectAction &operator=(SaveProjectAction &&other) noexcept
        {
            if (this != &other)
            {
                IEditorAction::operator=(std::move(other));
                repo_ = std::move(other.repo_);
            }
            return *this;
        }

        std::string getName() const override { return "Save Project to JSON"; }

        void execute(EditorState &editor_state) override
        {
            using namespace ftxui;

            if (!fs::exists(save_folder_))
            {
                fs::create_directories(save_folder_);
            }

            std::vector<std::string> save_files;
            if (fs::is_directory(save_folder_))
            {
                for (const auto &entry : fs::directory_iterator(save_folder_))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".json")
                    {
                        save_files.push_back(entry.path().filename().string());
                    }
                }
            }

            int selected_file = 0;
            ftxui::MenuOption menu_opt;
            auto menu_component = Menu(&save_files, &selected_file, menu_opt);

            std::string new_filename_input;
            InputOption input_opt;
            auto input_component = Input(&new_filename_input, "my_project_name", input_opt);

            int tab_selected = save_files.empty() ? 1 : 0;
            std::vector<std::string> tab_entries = {"Overwrite Existing File", "Create New File"};
            auto tab_toggle = Toggle(&tab_entries, &tab_selected);

            auto container = Container::Vertical({
                tab_toggle,
                Container::Tab({
                    menu_component,
                    input_component,
                }, &tab_selected)
            });

            auto screen = ScreenInteractive::Fullscreen();
            bool is_confirmed = false;

            auto ui_renderer = Renderer(container, [&]() -> Element {
                Element content_view;
                if (tab_selected == 0 && !save_files.empty())
                {
                    content_view = vbox({
                        text("Select file to overwrite:") | dim,
                        separator(),
                        menu_component->Render()
                    });
                }
                else if (tab_selected == 0 && save_files.empty())
                {
                    content_view = text("No existing save files found.") | color(Color::Yellow);
                }
                else
                {
                    content_view = vbox({
                        text("Enter new filename (without extension):") | dim,
                        separator(),
                        input_component->Render() | border
                    });
                }

                return window(
                    text(" SAVE PROJECT ") | bold | color(Color::Green) | hcenter,
                    vbox({
                        tab_toggle->Render() | hcenter,
                        separator(),
                        content_view,
                        separator(),
                        text("Tab: Switch mode | Enter: Save |'q': Cancel") | dim | hcenter
                    })
                ) | center;
            });

            auto event_handler = CatchEvent(ui_renderer, [&](Event event) {
                if (event == Event::Return) {
                    if (tab_selected == 0 && save_files.empty()) {
                        return true;
                    }
                    if (tab_selected == 1 && new_filename_input.empty()) {
                        return true;
                    }
                    is_confirmed = true;
                    screen.Exit();
                    return true;
                }

                if (event == Event::Character("Q") || event == Event::Character("q")) {
                    is_confirmed = false;
                    screen.Exit();
                    return true;
                }

                return false;
            });

            screen.Loop(event_handler);

            if (!is_confirmed)
            {
                logger_->info("SAVE CANCELLED. RETURNING TO MAIN MENU.");
                return;
            }

            std::string final_filepath;
            if (tab_selected == 0)
            {
                final_filepath = (fs::path(save_folder_) / save_files[selected_file]).string();
            }
            else
            {
                std::string name = new_filename_input;
                if (name.rfind(".json") == std::string::npos || name.rfind(".json") != name.length() - 5)
                {
                    name += ".json";
                }
                final_filepath = (fs::path(save_folder_) / name).string();
            }

            try
            {
                repo_->save(editor_state.getEditorProject(), final_filepath);
                logger_->info("PROJECT SUCCESSFULLY SAVED TO: " + final_filepath);
            }
            catch (const std::exception &e)
            {
                logger_->errror("FAILED TO SAVE PROJECT: " + std::string(e.what()));
            }
        }
    };
}