#include "game-state-presenter.h"
#include "../../core/models/map.h"

#include "iostream"

namespace Engine::Adapters::RuntimeUI {
	void GameStatePresenter::clearScreen() const {
		system("cls");//MAY BE SOME SHY
	}
    void GameStatePresenter::drawGrid(const Core::Models::Project& project) const {
        auto& map = project.getMap();
        Core::Models::Vec2 size = map.getSize();
        auto& unit_pool = project.getUnitPool();

        std::cout << "--- MAP: " << map.getName() << " (" << size.x << "x" << size.y << ") ---" << std::endl;

        for (int y = 0; y < size.y; ++y) {
            for (int x = 0; x < size.x; ++x) {
                char cell_symbol = '.'; 
                for (const auto& unit : unit_pool) {
                    if (unit->isPlaced()) {
                        auto pos = unit->getPosition();
                        if (pos->x == x && pos->y == y) {
                            cell_symbol = unit->getType().empty() ? 'U' : unit->getType()[0];
                            break;
                        }
                    }
                }

                std::cout << cell_symbol << " ";
            }
            std::cout << std::endl;
        }
    }

    void GameStatePresenter::drawHUD(const Core::Models::Project& project) const {
        std::cout << "\n=================== HUD ===================" << std::endl;
        std::cout << "UUNITS IN WORLD:" << std::endl;

        bool any_placed = false;
        for (const auto& unit : project.getUnitPool()) {
            if (unit->isPlaced()) {
                auto pos = unit->getPosition();
                std::cout << " - [" << unit->getType() << "] " << unit->getName()
                    << " at (" << pos->x << ", " << pos->y << ")" << std::endl;
                any_placed = true;
            }
        }
        if (!any_placed) {
            std::cout << " NO UNITS PLACED ON MAP" << std::endl;
        }

        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Press Enter to continue simulation (0 to exit to Editor)..." << std::endl;
    }

    void GameStatePresenter::present(const Core::Models::Project& project) const {
        clearScreen();
        drawGrid(project);
        drawHUD(project);
    }
}