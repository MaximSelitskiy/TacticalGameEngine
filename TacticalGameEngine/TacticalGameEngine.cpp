#include "TacticalGameEngine.h"
#include "src/core/models/project.h"
#include "src/infrastcructure/logging/logger.h"
#include "src/core/runtime/game-engine.h"
#include "src/adapters/editor/actions/create-unit-action.h"
#include <memory>
int main()
{
	auto logger = std::make_shared<Engine::Infrastructure::Logging::LoggerConsole>();
	auto project = std::make_unique<Engine::Core::Models::Project>("project_moon",logger);
	//Engine::Core::Runtime::GameEngine engine(std::move(project), logger);
	Engine::Adapters::Editor::Actions::CreateUnitAction unit_action(logger);
	Engine::Adapters::Editor::EditorState editor(std::move(project));
	unit_action.execute(editor);

	return 0;
}