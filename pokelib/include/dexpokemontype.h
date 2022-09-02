#pragma once

#include <entity.h>
#include <entity_macro.h>

namespace pokelib {
    struct DexPokemonType
    {
    public:
        using Entity = entity::EntitySketch<DexPokemonType>;
        ENTITY_DEFINE_TYPE(DexPokemonType,
            type_id,
            name_en,
            name_jp,
            name_pt);
    public:
        uint32_t type_id;
        std::string name_en;
        std::string name_jp;
        std::string name_pt;
    };
}
