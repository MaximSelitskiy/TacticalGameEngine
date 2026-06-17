#include "id-generator.h"
#include <uuid.h>
#include <random>

namespace Engine::Core::Utils {
    std::string generateId() {
        std::random_device rd;
        auto seeds = { rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
        std::seed_seq seed(seeds);
        std::mt19937 engine(seed);
        uuids::uuid_random_generator gen(engine);
        uuids::uuid id = gen();
        return uuids::to_string(id);
    }
}