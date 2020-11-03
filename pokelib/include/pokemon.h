#pragma once
#ifndef POKEMON_H
#define POKEMON_H

#include <string>

namespace pokelib
{
    struct Pokemon
    {
        uint32_t dex_no;
        std::string name;
        std::string types;
        uint32_t total_stats;
        uint32_t total_hp;
        uint32_t phys_atk;
        uint32_t phys_def;
        uint32_t spec_atk;
        uint32_t spec_def;
        uint32_t speed;
    };
}

namespace std
{
    string to_string(const pokelib::Pokemon& pokemon);
}
#endif
