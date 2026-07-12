add_library(nlohmann_json::nlohmann_json STATIC
    json_project_repository.cpp,
json_project_repository.h)

target_include_directories(nlohmann_json::nlohmann_json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(nlohmann_json::nlohmann_json PUBLIC nlohmann_json::nlohmann_json)