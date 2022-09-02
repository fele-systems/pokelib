#pragma once
#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <pkm_type.h>
#include <entity.h>
#include <entity_macro.h>

namespace pokelib
{
    struct DexPokemon
    {
    public:
        using Entity = entity::EntitySketch<DexPokemon>;
        //static void to_entity(Entity& e, const DexPokemon& p);
        //static void from_entity(const Entity& e, DexPokemon& p);
        ENTITY_DEFINE_TYPE(DexPokemon,
            pkm_id,
            national_dex_no,
            name,
            ptype,
            stype,
            total_hp,
            phys_atk,
            phys_def,
            spec_atk,
            spec_def,
            speed
        )
    public:
        uint32_t pkm_id;

        uint32_t national_dex_no;
        std::string name;

        PokemonType ptype;
        PokemonType stype;

        uint32_t total_hp;
        uint32_t phys_atk;
        uint32_t phys_def;
        uint32_t spec_atk;
        uint32_t spec_def;
        uint32_t speed;
    };
    

    struct BattlePokemon
    {
    };
}

namespace std
{
    string to_string(const pokelib::DexPokemon& pokemon);
}
#endif
