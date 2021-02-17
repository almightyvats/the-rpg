#include "Equipment.hpp"


enum class LootMaterial {
    wood,
    metal,
    gold,
    fire,
    ice,
    special,
};

Equipment GenerateLoot(int min_level, int max_level);

Equipment GenerateBaseLoot(int min_level, int max_level);