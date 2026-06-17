#include "TacticalGameEngine.h"
#include "src/core/models/project.h"
#include "src/infrastcructure/logging/logger.h"
#include "src/core/runtime/game-engine.h"

#include <memory>
int main()
{
	auto logger = std::make_shared<Engine::Infrastructure::Logging::LoggerConsole>();
	auto project = std::make_unique<Engine::Core::Models::Project>("project_moon",logger);
	Engine::Core::Runtime::GameEngine engine(std::move(project), logger);
	engine.update();
	engine.start();
	while (engine.isActive()) {
		engine.update();
		engine.end();
	}
	return 0;
}
