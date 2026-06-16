#include "TacticalGameEngine.h"
#include "src/core/models/project.h"
#include "src/infrastcructure/logging/logger.h"
using namespace std;

int main()
{
	auto logger = std::make_shared<Engine::Infrastructure::Logging::LoggerConsole>();

	Engine::Core::Models::Project engine("NAME",logger);
	cout << "Hello CMake." << endl;
	engine.getName();
	return 0;
}
