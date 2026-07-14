#pragma once
#include "../IEditorAction.h"
#include "../../../core/interfaces/IProjectRepository.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace Engine::Adapters::Editor::Actions {
    class SaveProjectAction : public IEditorAction {
    private:
        std::shared_ptr<Core::Interfaces::IProjectRepository> repo_;
        const std::string save_folder_ = "saves";
    public:
        SaveProjectAction(std::shared_ptr<Core::Interfaces::ILogger> logger,
            std::shared_ptr<Core::Interfaces::IProjectRepository> repo)
            : IEditorAction(logger), repo_(repo) {}

        std::string getName() const override { return "Save Project to JSON"; }

        void execute(EditorState& editor_state) override {
            logger_->info("------ PROJECT SAVING PROCESS -----");

            std::vector<fs::path> save_files;
            if (fs::exists(save_folder_) && fs::is_directory(save_folder_)) {
                for (const auto& entry : fs::directory_iterator(save_folder_)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".json") {
                        save_files.push_back(entry.path());
                    }
                }
            }

            logger_->info("0. CREATE NEW SAVE FILE");
            for (size_t i = 0; i < save_files.size(); ++i) {
                logger_->info((i + 1) + ". OVERWRITE: " + save_files[i].filename().string() + "\n");
            }

            logger_->info("CHOOSE OPTION: ");
            size_t choice;
            if (!(std::cin >> choice) || choice > save_files.size()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                logger_->warn("INVALID CHOICE. SAVE CANCELLED.");
                return;
            }
            std::string final_filepath;

            if (choice == 0) {
                std::string new_name;
                logger_->info("ENTER NEW SAVE FILE NAME (without .json): ");
                std::cin >> new_name;
                final_filepath = save_folder_ + "/" + new_name + ".json";
            }
            else {
                final_filepath = save_files[choice - 1].string();
            }
            try {
                repo_->save(editor_state.getEditorProject(), final_filepath);
                logger_->info("PROJECT SUCCESSFULLY SAVED TO: " + final_filepath);
            }
            catch (const std::exception& e) {
                logger_->errror("FAILED TO SAVE PROJECT: " + std::string(e.what()));
            }
        }
    };
}