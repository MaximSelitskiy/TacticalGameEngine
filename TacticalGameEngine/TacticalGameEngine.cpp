#include "TacticalGameEngine.h"
#include "src/core/models/project.h"
#include "src/infrastcructure/logging/logger.h"
#include "src/infrastcructure/logging/logger_ftxui.h"
#include "src/infrastcructure/persistance/json_project_repository.h"
#include "src/adapters/shared/state-manager.h"
#include <memory>
#include <filesystem>
int main()
{
	std::filesystem::create_directory("saves");
	auto logger = std::make_shared<Engine::Infrastructure::Logging::LoggerFTXUI>();
	auto project = std::make_unique<Engine::Core::Models::Project>("project_moon",logger);
	auto repository = std::make_shared<Engine::Infrastructure::Persistence::JsonProjectRepository>(logger);
	Engine::Adapters::Shared::StateManager state_manager(logger, std::move(project),repository);
	while (!state_manager.isExiting()) {
		state_manager.update();
		//system("cls");
	}
	return 0;
}