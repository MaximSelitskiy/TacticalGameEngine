add_library(engine_core STATIC
    models/unit.cpp
    models/unit.h
    models/item.h
    models/map.h
    models/project.h
    runtime/game-engine.h
    utils/id-generator.h 
    interfaces/ILogger.h "runtime/game-engine.cpp" "utils/id-generator.cpp" "interfaces/IProjectRepository.h")

target_include_directories(engine_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine_core PUBLIC stduuid)