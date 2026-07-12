#include "json_project_repository.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace Engine::Infrastructure::Persistence {

	void JsonProjectRepository::save(const Core::Models::Project& project, const std::string& filepath) {
		json j;
		j["poject name"] = project.getName();
		auto map_size = project.getMap().getSize();
		j["map"] = {
			{"name", project.getMap().getName() },
			{"id", project.getMap().getId()},
			{"width", map_size.x},
			{"height" , map_size.y}
		};

		j["available_unit_types"] = project.getAvailableUnitTypes();
		j["available_item_types"] = project.getAvailableItemTypes();

		j["unit_pool"] = json::array();

	}

}