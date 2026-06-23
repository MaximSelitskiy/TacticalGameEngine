#pragma once
#include <vector>
#include <string>
#include <memory>
#include "item.h"
#include "unit.h"
#include "map.h"
#include "../interfaces/ILogger.h"
namespace Engine::Core::Models {

	class Project {
	private:
		std::string name_;
		std::unique_ptr<Map> active_map_;
		std::vector<std::unique_ptr<Unit>> units_in_world_;
		std::vector<std::unique_ptr<Item>> items_in_world_;

		std::vector<std::string> allowed_unit_types_;
		std::vector<std::string> allowed_item_types_;

		std::shared_ptr<Interfaces::ILogger> logger_;
	public:
		Project(std::string name, std::shared_ptr<Interfaces::ILogger> logger) : name_(name), logger_(logger) {
			active_map_ = std::make_unique<Map>("Deafult Map", 10, 10);

			allowed_unit_types_ = { "ENEMY", "FRIEND" };
			allowed_item_types_ = { "WEAPON", "POTION" };
		}

		std::string getName() const {
			logger_->info("WE GOT NAME.");
			return name_;
		}
		void setName(std::string name) { name_ = name; }

		Map& getMap() const { return *active_map_; }//пришлось бы возвращать указатель
		void setMap(std::unique_ptr<Map> new_map) {
			active_map_ = std::move(new_map);//move потому что указатель уникальный, = не пройдет
		}

		void addUnitType(std::string new_unit_type) {
			allowed_unit_types_.push_back(new_unit_type);
		}
		std::vector<std::string> getAvailableUnitTypes() const { return allowed_unit_types_; }

		void addItemType(std::string new_item_type) {
			allowed_item_types_.push_back(new_item_type);
		}
		std::vector<std::string> getAvailableItemTypes() const { return allowed_item_types_; }

		void spawnUnit(std::unique_ptr<Unit> new_unit) {
			units_in_world_.push_back(std::move(new_unit));
		}
		const std::vector<std::unique_ptr<Unit>>& getUnitsInWorld() const {
			return units_in_world_;
		}//почитать про геттеры с unique_ptr
		void removeUnit(std::string id) {
			std::erase_if(units_in_world_, [&id](const auto& unit) { return unit->getId() == id; });//разобрать лямбду
		}
	};

}