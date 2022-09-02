#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <dexpokemontype.h>
#include <dexpokemon.h>

#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <stdexcept>
#include <pkm_type.h>
#include <vector>

namespace pokelib
{
    enum class Field
    {
        name        = 1 << 0,
        type0       = 1 << 1,
        type1       = 1 << 2,
        dex_number  = 1 << 3,
        
        types       = type0 | type1,
        all         = name | dex_number | types
    };

    inline Field operator| (Field a, Field b) { return (Field)((int)a | (int)b); }
    inline Field operator& (Field a, Field b) { return (Field)((int)a & (int)b); }
    inline Field operator^ (Field a, Field b) { return (Field)((int)a ^ (int)b); }

    // A Gen-8 Pokémon database
    class PokeDex
    {
    public:
        PokeDex(const std::string& filename);
        ~PokeDex();
        PokeDex(const PokeDex&) = delete;
        PokeDex(PokeDex&&) = delete;
        PokeDex& operator=(const PokeDex&) = delete;
        PokeDex& operator=(PokeDex&&) = delete;
    
        void reload(const std::string& filename);
        bool good() const;
        std::string get_error() const;

        DexPokemon pokemon(const std::string& name);
        std::vector<DexPokemon> search_pokemon(std::string value, Field fields);

        DexPokemonType get_type_from_name(const char* name);
        std::string get_type_name(PokemonType type);
    
        void print_all_shit();
    private:
        std::pair<DexPokemon, bool> next_pokemon_from_statement();
    private:
        sqlite3* sqlite;
        int open_code = -1;
        sqlite3_stmt* current_stmt = nullptr;
    };

//   template<typename RequestType>
//   void PokeDex::request(RequestType& r)
//   {
//       auto query = r.build_statement();
//       std::cout << query << std::endl;
//       if (sqlite3_prepare_v2(sqlite, query.c_str(), query.size(), &current_stmt, nullptr) != SQLITE_OK)
//       {
//           throw std::runtime_error{ sqlite3_errmsg(sqlite) };
//       }
//   }
}

#endif
