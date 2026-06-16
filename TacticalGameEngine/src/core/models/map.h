#pragma once
#include <string>
#include "../utils/id-generator.h"

namespace Engine::Core::Models {

	struct Vec2 {
		int x;
		int y;
	};

	class Map {
	private:
		std::string name_;
		std::string id_;
		int width_;
		int height_;
	public:
		Map(std::string name, int width, int height) : name_(name), width_(width), height_(height), id_(Utils::generateId()) {}//ДОБАВИТЬ MOVE ДЛЯ СТРОК
		~Map() = default;
		std::string getName() const { return name_; }
		Vec2 getSize() const { return { width_,height_ }; }
		std::string getId() const { return id_; }
	};
}