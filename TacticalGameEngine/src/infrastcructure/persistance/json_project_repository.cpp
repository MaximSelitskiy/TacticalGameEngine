#include "json_project_repository.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace Engine::Infrastructure::Persistence {

    void JsonProjectRepository::save(const Core::Models::Project& project, const std::string& filepath) {
        json j;

        //SERIALIZING
        j["project_name"] = project.getName();

        //MAP
        auto map_size = project.getMap().getSize();
        j["map"] = {
            {"name", project.getMap().getName() },
            {"id", project.getMap().getId()},
            {"width", map_size.x},
            {"height" , map_size.y}
        };

        //AVAILABLE UNIT AND ITEM TYPES
        j["available_unit_types"] = project.getAvailableUnitTypes();
        j["available_item_types"] = project.getAvailableItemTypes();
        const auto& unitts = project.getUnitPool();
        logger_->info("REAL POOL SIZE: " + std::to_string(unitts.size()));
        //AVAILABLE UNIT POOL
        if (!project.getUnitPool().empty()) {
            j["unit_pool"] = json::array();
            for (const auto& unit : project.getUnitPool()) {
                //if (!unit) {
                //    continue;
                //}
                json unit_json = { {"name", unit->getName()}, {"id", unit->getId()}, {"type", unit->getType()} };
                if (unit->isPlaced()) {
                    auto pos = unit->getPosition();
                    unit_json["pos"] = { {"x", pos->x}, {"y", pos->y} };
                }
                nlohmann::json inventory_array = nlohmann::json::array();
                for (const auto& item : unit->getInventory()) {
                    nlohmann::json i_json;
                    i_json["id"] = item->getId();
                    i_json["name"] = item->getName();
                    i_json["type"] = item->getType();
                    inventory_array.push_back(i_json);
                }
                unit_json["inventory"] = inventory_array;
                j["unit_pool"].push_back(unit_json);
            }
        }

        //AVAILABLE ITEM POOL
        if (!project.getItemPool().empty()) {
            j["item_pool"] = json::array();
            for (const auto& item : project.getItemPool()) {
                if (!item) {
                    continue;
                }
                json item_json = { {"name", item->getName()}, {"id", item->getId()}, {"type", item->getType()} };
                j["item_pool"].push_back(item_json); 
            }
        }

        //UNITS IN WORLD
        if (!project.getUnitsInWorld().empty()) {
            j["units_in_world"] = json::array();
            for (const auto& unit : project.getUnitsInWorld()) {
                if (!unit) {
                    continue;
                }
                json unit_json = { {"name", unit->getName()}, {"id", unit->getId()}, {"type", unit->getType()} };
                if (unit->isPlaced()) {
                    auto pos = unit->getPosition();
                    unit_json["pos"] = { {"x", pos->x}, {"y", pos->y} };
                }

                nlohmann::json inventory_array = nlohmann::json::array();
                for (const auto& item : unit->getInventory()) {
                    nlohmann::json i_json = { {"id", item->getId()}, {"name", item->getName()}, {"type", item->getType()} };
                    inventory_array.push_back(i_json);
                }
                unit_json["inventory"] = inventory_array;
                j["units_in_world"].push_back(unit_json);
            }
        }

        //ITEMS IN WORLD
        if (!project.getItemsInWorld().empty()) {
            j["items_in_world"] = json::array();
            for (const auto& item : project.getItemsInWorld()) {
                if (!item) {
                    continue;
                }
                json item_json = { {"name", item->getName()}, {"id", item->getId()}, {"type", item->getType()} };
                j["items_in_world"].push_back(item_json);
            }
        }

        //WRITING TO FILE
        std::ofstream file(filepath);
        if (file.is_open()) {
            file << j.dump(3);
            logger_->info("PROJECT SAVED TO " + filepath);
        }
        else {
            logger_->errror("FAILED TO OPEN FILE FOR SAVING: " + filepath);
        }
    }

	std::unique_ptr<Core::Models::Project> JsonProjectRepository::load(const std::string& filepath) {
		std::ifstream file(filepath);
		if (!file.is_open()) {
			logger_->errror("FAILED TO OPEN FILE FOR LOADING: " + filepath);
			return nullptr;
		}
		//DESERIALIZING
		json j;
		file >> j;

		auto project = std::make_unique<Core::Models::Project>(j["project_name"].get<std::string>(), logger_);

		//MAP
		auto json_map = j["map"];
		auto map = std::make_unique<Core::Models::Map>(json_map["name"], json_map["width"], json_map["height"]);
		project->setMap(std::move(map));

		//UNIT POOL
		for (const auto& u_json : j["unit_pool"]) {
			auto unit = std::make_unique<Core::Models::Unit>(
				u_json["name"].get<std::string>(),
				u_json["type"].get<std::string>()
			);
			unit->setId(u_json["id"].get<std::string>());
			if (u_json.contains("pos")) {
				unit->setPosition(u_json["pos"]["x"].get<int>(), u_json["pos"]["y"].get<int>());
			}
			if (u_json.contains("inventory")) {
				for (const auto& i_json : u_json["inventory"]) {
					auto item = std::make_unique<Core::Models::Item>(
						i_json["name"].get<std::string>(),
						i_json["type"].get<std::string>()
					);
					item->setId(i_json["id"].get<std::string>());
					unit->addItem(std::move(item));
				}
			}
			project->addUnitToPool(std::move(unit));
		}

        //UNITS IN WORLD
        for (const auto& u_json : j["units_in_world"]) {
            auto unit = std::make_unique<Core::Models::Unit>(
                u_json["name"].get<std::string>(),
                u_json["type"].get<std::string>()
            );
            unit->setId(u_json["id"].get<std::string>());
            if (u_json.contains("pos")) {
                unit->setPosition(u_json["pos"]["x"].get<int>(), u_json["pos"]["y"].get<int>());
            }
            if (u_json.contains("inventory")) {
                for (const auto& i_json : u_json["inventory"]) {
                    auto item = std::make_unique<Core::Models::Item>(
                        i_json["name"].get<std::string>(),
                        i_json["type"].get<std::string>()
                    );
                    item->setId(i_json["id"].get<std::string>());
                    unit->addItem(std::move(item));
                }
            }
            project->spawnUnit(std::move(unit));
        }

		//ITEM POOL
		for (const auto& item_json : j["item_pool"]) {
			auto item = std::make_unique<Core::Models::Item>(item_json["name"], item_json["type"]);
			project->addItemToPool(std::move(item));
		}

		logger_->info("PROJECT LOADED FROM " + filepath);
		return project;
	}

}