#include "TacticalGameEngine.h"
#include "src/core/models/project.h"
#include "src/infrastcructure/logging/logger.h"
#include "src/adapters/shared/state-manager.h"
#include <memory>
int main()
{
	auto logger = std::make_shared<Engine::Infrastructure::Logging::LoggerConsole>();
	auto project = std::make_unique<Engine::Core::Models::Project>("project_moon",logger);
	Engine::Adapters::Shared::StateManager state_manager(logger, std::move(project));
	while (!state_manager.isExiting()) {
		state_manager.update();
		//system("cls");
	}
	return 0;

}
