
#include <database.h>
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

    if (sqlite3_prepare_v2(sqlite, query_str.c_str(), (int) query_str.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }

    auto results = entity::fetch_from_statement<DexPokemon>(current_stmt);
    if (results.empty())
    {
        throw std::runtime_error{ "No pokémon was found with requested name" };
    }

    auto result = next_pokemon_from_statement();
    if (result.second)
    {
        return result.first;
    }
    else
    {
        
    }
}

std::vector<pokelib::DexPokemon> pokelib::PokeDex::search_pokemon(std::string value, Field fields)
{
    std::stringstream ss;
    ss << R"(SELECT pkm.pkm_id,
                    pkm.national_dex_no,
                    pkm.name,
                    type1.name_en,
                    type2.name_en,
                    pkm.total_hp,
                    pkm.phys_atk,
                    pkm.phys_def,
                    pkm.spec_atk,
                    pkm.spec_def,
                    pkm.speed
            FROM Pokemon pkm INNER JOIN PokemonType type1 ON pkm.ptype = type1.type_id
                             LEFT  JOIN PokemonType type2 ON pkm.stype = type2.type_id
            WHERE)";
    std::transform(value.begin(), value.end(), value.begin(), [](char ch)
    {
        if (ch == '*')
            return '%';
        else
            return ch;
    });

    bool first = true;
    if ((fields & Field::name) == Field::name)
    {
        ss << " pkm.name LIKE '" << value << "'";
        first = false;
        fields ^ Field::name;
    }

    if ((fields & Field::dex_number) == Field::dex_number)
    {
        if (!first)
            ss << " OR ";
        ss << " CAST(pkm.national_dex_no AS VARCHAR(4)) LIKE '" << value << "'";
        first = false;
        fields ^ Field::name;
    }
    
    if ((fields & Field::type0) == Field::type0)
    {
        if (!first)
            ss << " OR ";
        ss << " type1.name_en LIKE '" << value << "'";
        first = false;
        fields ^ Field::name;
    }

    if ((fields & Field::type1) == Field::type1)
    {
        if (!first)
            ss << " OR ";
        ss << " type2.name_en LIKE '" << value << "'";
        first = false;
        fields ^ Field::name;
    }
    if (int(fields) == 0)
        throw std::runtime_error { std::string("Invalid flags detected. Resulting fields flag: ") + std::to_string((int) fields) };
    std::string query_str = ss.str();
    std::cout << query_str << std::endl;
    
    if (sqlite3_prepare_v2(sqlite, query_str.c_str(), (int) query_str.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }
    
    std::vector<DexPokemon> results;
    auto result = next_pokemon_from_statement();
    while (result.second)
    {
        results.push_back(result.first);
        result = next_pokemon_from_statement();
    }

    return results;
}

std::pair<pokelib::DexPokemon, bool> pokelib::PokeDex::next_pokemon_from_statement()
{
    auto rc = sqlite3_step(current_stmt);
    switch (rc)
    {
    case SQLITE_DONE:
        return std::make_pair(DexPokemon{}, false);
    case SQLITE_ROW:
    {
        DexPokemon pkm;
        DexPokemon::Entity entity;

        auto count = sqlite3_column_count(current_stmt);
        for (int i = 0; i < count; i++)
        {
            const char* column_name = sqlite3_column_name(current_stmt, i);
            auto& field = entity[column_name];
            auto column_type = sqlite3_column_type(current_stmt, i);
            if (column_type == SQLITE_INTEGER)
            {
                field.set_from(static_cast<uint32_t>(sqlite3_column_int(current_stmt, i)));
            }
            else if (column_type == SQLITE_TEXT)
            {
                auto size = static_cast<size_t>(sqlite3_column_bytes(current_stmt, i));
                auto chars = reinterpret_cast<const char*>(sqlite3_column_text(current_stmt, i));
                field.set_from( std::string{ chars, size } );
            }
            else
            {
                throw std::runtime_error{ field.field_name + ": Unknown type" };
            }
        }
        std::cout << std::to_string(entity);
        DexPokemon::from_entity(entity, pkm);

        //assert(sqlite3_column_type(current_stmt, 0) == SQLITE_INTEGER);
        //pkm.pkm_id = sqlite3_column_int(current_stmt, 0);
        //
        //assert(sqlite3_column_type(current_stmt, 1) == SQLITE_INTEGER);
        //pkm.national_dex_no = sqlite3_column_int(current_stmt, 1);
        //
        //assert(sqlite3_column_type(current_stmt, 2) == SQLITE_TEXT);
        //pkm.name = std::string{ (const char*) sqlite3_column_text(current_stmt, 2), (size_t) sqlite3_column_bytes(current_stmt, 2) };
        //
        //assert(sqlite3_column_type(current_stmt, 3) == SQLITE_TEXT);
        ////pkm.ptype = std::string{ (const char*) sqlite3_column_text(current_stmt, 3), (size_t) sqlite3_column_bytes(current_stmt, 3) };
        //
        //if (sqlite3_column_type(current_stmt, 4) != SQLITE_NULL)
        //{
        //    assert(sqlite3_column_type(current_stmt, 4) == SQLITE_TEXT);
        //    //pkm.stype = std::string{ (const char*) sqlite3_column_text(current_stmt, 4), (size_t) sqlite3_column_bytes(current_stmt, 4) };
        //}
        //
        //assert(sqlite3_column_type(current_stmt, 5) == SQLITE_INTEGER);
        //pkm.total_hp = sqlite3_column_int(current_stmt, 5);       
        //
        //assert(sqlite3_column_type(current_stmt, 6) == SQLITE_INTEGER);
        //pkm.phys_atk = sqlite3_column_int(current_stmt, 6);       
        //
        //assert(sqlite3_column_type(current_stmt, 7) == SQLITE_INTEGER);
        //pkm.phys_def = sqlite3_column_int(current_stmt, 7);
        //
        //assert(sqlite3_column_type(current_stmt, 8) == SQLITE_INTEGER);
        //pkm.spec_atk = sqlite3_column_int(current_stmt, 8);       
        //
        //assert(sqlite3_column_type(current_stmt, 9) == SQLITE_INTEGER);
        //pkm.spec_def = sqlite3_column_int(current_stmt, 9);       
        //
        //assert(sqlite3_column_type(current_stmt, 10) == SQLITE_INTEGER);
        //pkm.speed = sqlite3_column_int(current_stmt, 10);       

        return std::make_pair(pkm, true);
    }
    default:
        throw std::runtime_error{ "Unexpected return code from database" };
    }
       
}

pokelib::PokemonType pokelib::PokeDex::get_type_from_name(const char* name)
{
    
    std::string query = fmt::format(R"(
        SELECT type_id
        FROM PokemonType
        WHERE LOWER(name_en) = LOWER('{}');)", name);
    std::cout << "Query: " << query << std::endl;
    
    if (sqlite3_prepare_v2(sqlite, query.data(), (int) query.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }
    
    auto rc = sqlite3_step(current_stmt);
    if (rc == SQLITE_ROW)
    {
        assert(sqlite3_column_type(current_stmt, 0) == SQLITE_INTEGER);
        return static_cast<PokemonType>( sqlite3_column_int(current_stmt, 0) );
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

std::string pokelib::PokeDex::get_type_name(pokelib::PokemonType type)
{
    std::string query = fmt::format(R"(
        SELECT name_en
        FROM PokemonType
        WHERE type_id = {};)", static_cast<int>(type));
    std::cout << "Query: " << query << std::endl;
    
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





