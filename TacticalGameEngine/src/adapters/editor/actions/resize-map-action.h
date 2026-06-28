#pragma once

#include "../IEditorAction.h"
#include "../../../core/interfaces/ILogger.h"

#include <string>
#include <vector>
#include <numeric>
#include <iostream>

namespace Engine::Adapters::Editor::Actions {

	class ResizeMapAction : public IEditorAction {
	public:
		ResizeMapAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : IEditorAction(logger) {}
		~ResizeMapAction() = default;
		void execute(EditorState& editor_state)override {
			auto& project = editor_state.getEditorProject();
			auto& current_map = project.getMap();
			Core::Models::Vec2 current_size = current_map.getSize();
            std::cout << "\n--- RESIZE MAP ACTION ---" << std::endl;
            std::cout << "CURRENT MAP: " << current_map.getName()
                << " [" << current_size.x << "x" << current_size.y << "]" << std::endl;

            int new_width = 0, new_height = 0;

            std::cout << "ENTER NEW WIDTH (min 1, max 100): ";
            while (!(std::cin >> new_width) || new_width <= 0 || new_width > 100) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                logger_->warn("INVALID WIDTH! RANGE [1..100]");
                std::cout << "ENTER NEW WIDTH: ";
            }

            std::cout << "ENTER NEW HEIGHT (min 1, max 100): ";
            while (!(std::cin >> new_height) || new_height <= 0 || new_height > 100) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                logger_->warn("INVALID HEIGHT! RANGE [1..100]");
                std::cout << "ENTER NEW WIDTH: ";
            }

            logger_->info("REGENERATING MAP WITH NEW SIZE...");

            auto new_map = std::make_unique<Core::Models::Map>(current_map.getName(), new_width, new_height);
            project.setMap(std::move(new_map));

            std::cout << "MAP RESIZED TO " << new_width << "x" << new_height << "!" << std::endl;
		}
		std::string getName() const override { return "Resize Map"; }
	};

}