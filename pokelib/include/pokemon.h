#pragma once
#ifndef POKEMON_H
#define POKEMON_H

#include <string>

namespace pokelib
{
    struct DexPokemon
    {
        uint32_t pkm_id;

        uint32_t national_dex_no;
        std::string name;

        std::string ptype;
        std::string stype;

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
