cmake_minimum_required(VERSION 3.20)
project(TacticalGameEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    stduuid
    GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
    GIT_TAG v1.2.3
)

FetchContent_MakeAvailable(stduuid)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v7.0.1 
)

FetchContent_MakeAvailable(ftxui)

add_subdirectory(src/core)

add_executable(TacticalGameEngine
    "TacticalGameEngine.cpp"
    "TacticalGameEngine.h"
    "src/adapters/editor/editor-state.h"
    "src/adapters/editor/menu-components.h"
    "src/adapters/editor/actions/resize-map-action.h"
    "src/adapters/editor/actions/create-unit-action.h"
    "src/adapters/editor/actions/create-item-action.h"
    "src/adapters/editor/IEditorAction.h"
    "src/adapters/shared/state-manager.h"
    "src/adapters/shared/state-manager.cpp"
    "src/adapters/editor/menu-components.cpp"
    "src/adapters/runtime_ui/game-state-presenter.h"
    "src/adapters/runtime_ui/game-state-presenter.cpp"
    "src/adapters/runtime_ui/game-terminal-presenter.h"
    "src/adapters/runtime_ui/game-terminal-presenter.cpp"
    "src/adapters/editor/actions/place-unit-action.h"
    "src/adapters/editor/actions/place-item-action.h"
    "src/infrastcructure/persistance/json_project_repository.cpp"
    "src/infrastcructure/persistance/json_project_repository.h"
    "src/adapters/editor/actions/save-project-action.h"
    "src/adapters/editor/actions/load-project-action.h"
)

target_link_libraries(TacticalGameEngine PRIVATE
    engine_core
    nlohmann_json::nlohmann_json
    ftxui
)
