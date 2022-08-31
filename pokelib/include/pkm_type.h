#pragma once
#ifndef PKM_PokemonTypeYPE_H
#define PKM_PokemonTypeYPE_H

#include <array>
#include <string>
#include <entity.h>

namespace pokelib
{
    enum class PokemonType
    {
        fairy       = 0,
        normal      = 1,
        fire        = 2,
        fighting    = 3,
        water       = 4,
        flying      = 5,
        grass       = 6,
        poison      = 7,
        electric    = 8,
        ground      = 9,
        psychic     = 10,
        rock        = 11,
        ice         = 12,
        bug         = 13,
        dragon      = 14,
        ghost       = 15,
        dark        = 16,
        steel       = 17,
        count       = 18
    };

    // The lines represents the offensive type and the columns the defensive type.
    // The higher the number, the higher the effectiveness, starting from 0 (no effect).
    // Types at columns are in the same order as types in the lines, so the first column is fairy
    // and the last one is steel
    static constexpr size_t affinity_chart_width = 18;
    static constexpr size_t affinity_chart_size = affinity_chart_width * affinity_chart_width;
    static constexpr std::array<uint8_t, affinity_chart_size> affinity_chart =
    {
                 /*fai|nor|fir|fig|wat|fly|gra|poi|ele|gro|psy|roc|ice|bug|dra|gho|dar|ste*/
        /* fai */  2,  2,  1,  3,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  3,  2,  3,  1,
        /* nor */  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  0,  2,  1,
        /* fir */  2,  2,  1,  2,  1,  2,  3,  2,  2,  2,  2,  1,  3,  3,  1,  2,  2,  3,
        /* fig */  1,  3,  2,  2,  2,  1,  2,  1,  2,  2,  1,  3,  3,  1,  2,  0,  2,  3,
        /* wat */  2,  2,  3,  2,  1,  2,  1,  2,  2,  3,  2,  3,  2,  2,  1,  2,  2,  2,
        /* fly */  2,  2,  2,  3,  2,  2,  3,  2,  1,  2,  2,  1,  2,  3,  2,  2,  2,  1,
        /* gra */  2,  2,  1,  2,  3,  1,  1,  1,  2,  3,  2,  3,  2,  1,  1,  2,  2,  1,
        /* poi */  3,  2,  2,  2,  2,  2,  3,  1,  2,  1,  2,  1,  2,  2,  2,  1,  2,  0,
        /* ele */  2,  2,  2,  2,  3,  3,  1,  2,  1,  0,  2,  2,  2,  2,  1,  2,  2,  2,
        /* gro */  2,  2,  3,  2,  2,  0,  1,  3,  3,  2,  2,  3,  2,  1,  2,  2,  2,  3,
        /* psy */  2,  2,  2,  3,  2,  2,  2,  3,  2,  2,  1,  2,  2,  2,  2,  2,  0,  1,
        /* roc */  2,  2,  3,  1,  2,  3,  2,  2,  2,  1,  2,  2,  3,  3,  2,  2,  2,  1,
        /* ice */  2,  2,  1,  2,  1,  3,  3,  2,  2,  3,  2,  2,  1,  2,  3,  2,  2,  1,
        /* bug */  1,  2,  1,  1,  2,  1,  3,  2,  2,  2,  3,  2,  2,  2,  2,  1,  3,  1,
        /* dra */  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  2,  2,  1,
        /* gho */  2,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  3,  1,  2,
        /* dar */  1,  2,  2,  1,  2,  2,  2,  2,  2,  2,  3,  2,  2,  2,  2,  3,  1,  2, 
        /* ste */  3,  2,  1,  2,  1,  2,  2,  2,  1,  2,  2,  3,  3,  2,  2,  2,  2,  1
    };

    inline int get_affinity(PokemonType attacker, PokemonType defender)
    {
        const auto y = static_cast<int>(attacker);
        const auto x = static_cast<int>(defender);
        return affinity_chart[x + y * affinity_chart_width];
    }

    static constexpr std::array<const char*, 4> affinity_str = {
        "no effect",
        "not very effective",
        "effective",
        "super effective"
    };

    static constexpr std::array<const char*, static_cast<size_t>(PokemonType::count)> types_str = {
        "fairy",
        "normal",
        "fire",
        "fighting",
        "water",
        "flying",
        "grass",
        "poison",
        "electric",
        "ground",
        "psychic",
        "rock",
        "ice",
        "bug",
        "dragon",
        "ghost",
        "dark",
        "steel"
    };

    // inline PokemonType operator~ (PokemonType a) { return (PokemonType)~(int)a; }
    inline PokemonType operator| (PokemonType a, PokemonType b) { return (PokemonType)((int)a | (int)b); }
    inline PokemonType operator& (PokemonType a, PokemonType b) { return (PokemonType)((int)a & (int)b); }
    inline PokemonType operator^ (PokemonType a, PokemonType b) { return (PokemonType)((int)a ^ (int)b); }
    inline PokemonType& operator|= (PokemonType& a, PokemonType b) { return (PokemonType&)((int&)a |= (int)b); }
    inline PokemonType& operator&= (PokemonType& a, PokemonType b) { return (PokemonType&)((int&)a &= (int)b); }
    inline PokemonType& operator^= (PokemonType& a, PokemonType b) { return (PokemonType&)((int&)a ^= (int)b); }
    
}

namespace entity {
    template <>
    struct Converter<uint32_t, pokelib::PokemonType>
    {
        static constexpr bool enabled = true;
        pokelib::PokemonType operator()(uint32_t f) const
        {
            return static_cast<pokelib::PokemonType>(f);
        }
    };

    template <>
    struct Converter<pokelib::PokemonType, uint32_t>
    {
        static constexpr bool enabled = true;
        uint32_t operator()(pokelib::PokemonType f) const
        {
            return static_cast<uint32_t>(f);
        }
    };
}

namespace std {
    string to_string(pokelib::PokemonType type);
}

#endif
