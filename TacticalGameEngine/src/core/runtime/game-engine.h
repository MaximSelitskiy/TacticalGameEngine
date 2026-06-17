#pragma once
#include "../models/project.h"
#include <vector>
#include <memory>
namespace Engine::Core::Runtime {
	class GameEngine {
	private:
		std::unique_ptr<Models::Project> active_project_;
		std::shared_ptr<Interfaces::ILogger> logger_;
		bool is_running_ = false;
	public:
		GameEngine(std::unique_ptr<Models::Project> project,
			std::shared_ptr<Interfaces::ILogger> logger);
		~GameEngine() = default;
		//был virtual, но надо убрать, т к это не интерфейс, а просто реализация методов в cpp
		 void start();
		 void update();//float delta_time
		 void end();
		//
		 bool isActive();
	};
}