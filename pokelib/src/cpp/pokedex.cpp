
#include "dexpokemon.h"
#include "entity.h"
#include <pokedex.h>
#include <dexpokemontype.h>

#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fmt/core.h>
#include <algorithm>


pokelib::PokeDex::PokeDex(const std::string& filename)
{
    reload(filename);
}

pokelib::PokeDex::~PokeDex()
{
    if (sqlite != nullptr)
        sqlite3_close(sqlite);
}

void pokelib::PokeDex::reload(const std::string& filename)
{
    open_code = sqlite3_open(filename.c_str(), &sqlite);
}

bool pokelib::PokeDex::good() const
{
    return open_code == 0;
}

std::string pokelib::PokeDex::get_error() const
{
    return std::string{ sqlite3_errmsg(sqlite) };
}


pokelib::DexPokemon pokelib::PokeDex::pokemon(const std::string& name)
{
    const std::string query_str = entity::find_by<pokelib::DexPokemon>("Pokemon", "name", name);;
    
    //std::cout << query_str << std::endl;
    sqlite3_stmt* current_stmt = nullptr;
    if (sqlite3_prepare_v2(sqlite, query_str.c_str(), (int) query_str.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }

    auto results = entity::fetch_from_statement<DexPokemon>(current_stmt);
    if (results.empty())
    {
        throw std::runtime_error{ "No pokémon was found with requested name" };
    }
    return results[0];
}

std::vector<pokelib::DexPokemon> pokelib::PokeDex::search_pokemon(const std::string& value)
{
    const auto query = fmt::format(R"(SELECT
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
    FROM Pokemon
    WHERE INSTR(LOWER(name), LOWER('{}')) > 0;
    )", value);

    sqlite3_stmt* current_stmt = nullptr;
    if (sqlite3_prepare_v2(sqlite, query.c_str(), (int) query.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }
    return entity::fetch_from_statement<DexPokemon>(current_stmt);    
}

pokelib::DexPokemonType pokelib::PokeDex::get_type_from_name(const char* name)
{
    const std::string query = entity::find_by<DexPokemonType>("PokemonType", "name_en", name);

    sqlite3_stmt* current_stmt = nullptr;
    if (sqlite3_prepare_v2(sqlite, query.data(), (int) query.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }

    auto results = entity::fetch_from_statement<DexPokemonType>(current_stmt);

    if (results.empty())
    {
        throw std::runtime_error { "The requested type does not exists" };
    }

    return results[0];
}

std::string pokelib::PokeDex::get_type_name(pokelib::PokemonType type)
{
    std::string query = fmt::format(R"(
        SELECT name_en
        FROM PokemonType
        WHERE type_id = {};)", static_cast<int>(type));
    std::cout << "Query: " << query << std::endl;
    
    sqlite3_stmt* current_stmt = nullptr;
    if (sqlite3_prepare_v2(sqlite, query.data(), (int) query.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }
    
    auto rc = sqlite3_step(current_stmt);
    if (rc == SQLITE_ROW)
    {
        assert(sqlite3_column_type(current_stmt, 0) == SQLITE_TEXT);
        return std::string{ (const char*) sqlite3_column_text(current_stmt, 0), (size_t) sqlite3_column_bytes(current_stmt, 0) };
    }
    else if (rc == SQLITE_DONE)
    {
        throw std::runtime_error { "The requested type does not exists" };
    }
    else
    {
        throw std::runtime_error{ "Unexpected return code from database" };
    }
}





