#pragma once

#include "../utils/id-generator.h"

#include <string>

namespace Engine::Core::Models {

	struct Vec2 {
		short x;
		short y;
	};

	class Map {
	private:
		std::string name_;
		std::string id_;
		short width_;
		short height_;
	public:
		Map(std::string name, short width, short height) : name_(std::move(name)), width_(width), height_(height), id_(std::move(Utils::generateId())) {}//ДОБАВИТЬ MOVE ДЛЯ СТРОК
		~Map() = default;
		std::string getName() const { return name_; }
		Vec2 getSize() const { return { width_,height_ }; }
		std::string getId() const { return id_; }
	};
}