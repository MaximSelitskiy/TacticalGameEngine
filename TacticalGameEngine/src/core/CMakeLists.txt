# Создаем статическую библиотеку ядра
# Здесь перечисляем файлы, которые относятся ТОЛЬКО к ядру (модели и рантайм)
add_library(engine_core STATIC
    models/unit.cpp
    models/unit.h
    models/item.h
    models/map.h
    models/project.h
    runtime/game-engine.h
 "utils/id-generator.h")

# Указываем, что папка src/core является корневой для инклудов внутри ядра
target_include_directories(engine_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Связываем ядро со скачанным stduuid (строго так, как вывел vcpkg!)
target_link_libraries(engine_core PRIVATE stduuid)