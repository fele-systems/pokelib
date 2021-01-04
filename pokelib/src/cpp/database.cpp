
#include <database.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fmt/core.h>

pokelib::Database::Database(const std::string& filename)
{
    reload(filename);
}

pokelib::Database::~Database()
{
    if (sqlite != nullptr)
        sqlite3_close(sqlite);
}

void pokelib::Database::reload(const std::string& filename)
{
    open_code = sqlite3_open(filename.c_str(), &sqlite);
}

bool pokelib::Database::good() const
{
    return open_code == 0;
}

std::string pokelib::Database::get_error() const
{
    return std::string{ sqlite3_errmsg(sqlite) };
}

void pokelib::Database::request_fuzzy_search(const char* value)
{
    std::stringstream ss;
    ss << R"(SELECT dex_no,
                   name,
                   types,
                   total_stats,
                   total_hp,
                   phys_atk,
                   phys_def,
                   spec_atk,
                   spec_def,
                   speed
            FROM POKEMON )"
        << "WHERE name LIKE '%" << value << "%'"
        << " OR types LIKE '%" << value << "%';";
    std::string buffer = ss.str();
    std::cout << buffer << std::endl;
    
    if (sqlite3_prepare_v2(sqlite, buffer.c_str(), buffer.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }
}

void pokelib::Database::request_pokemon(const char* name)
{
    const char* fmt = R"(
            SELECT dex_no,
                   name,
                   types,
                   total_stats,
                   total_hp,
                   phys_atk,
                   phys_def,
                   spec_atk,
                   spec_def,
                   speed
            FROM POKEMON
            WHERE name = '%s';)";
    size_t buff_size = std::strlen(fmt) + std::strlen(name) + 1;
    std::vector<char> buffer(buff_size, '\0');
    auto written_bytes = size_t(snprintf(buffer.data(), buffer.size(), fmt, name));
    std::cout << "Written: " << written_bytes << "; Buff size: " << buffer.size() << std::endl;
    std::cout << "Query: " << buffer.data() << std::endl;
    assert(written_bytes == buffer.size() - 3); // Minus 3 cuz %s will be replaced
    
    if (sqlite3_prepare_v2(sqlite, buffer.data(), buffer.size(), &current_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error{ sqlite3_errmsg(sqlite) };
    }
}

std::shared_ptr<pokelib::Pokemon> pokelib::Database::fetch_request()
{
    auto rc = sqlite3_step(current_stmt);
    switch (rc)
    {
    case SQLITE_DONE:
        return nullptr;
    case SQLITE_ROW:
    {
        auto pkm = std::make_shared<Pokemon>();
    
        assert(sqlite3_column_type(current_stmt, 0) == SQLITE_INTEGER);
        pkm->dex_no = sqlite3_column_int(current_stmt, 0);
        
        assert(sqlite3_column_type(current_stmt, 1) == SQLITE_TEXT);
        pkm->name = std::string{ (const char*) sqlite3_column_text(current_stmt, 1), (size_t) sqlite3_column_bytes(current_stmt, 1) };

        assert(sqlite3_column_type(current_stmt, 2) == SQLITE_TEXT);
        pkm->types = std::string{ (const char*) sqlite3_column_text(current_stmt, 2), (size_t) sqlite3_column_bytes(current_stmt, 2) };
        
        assert(sqlite3_column_type(current_stmt, 3) == SQLITE_INTEGER);
        pkm->total_stats = sqlite3_column_int(current_stmt, 3);       

        assert(sqlite3_column_type(current_stmt, 4) == SQLITE_INTEGER);
        pkm->total_hp = sqlite3_column_int(current_stmt, 4);       

        assert(sqlite3_column_type(current_stmt, 5) == SQLITE_INTEGER);
        pkm->phys_atk = sqlite3_column_int(current_stmt, 5);       

        assert(sqlite3_column_type(current_stmt, 6) == SQLITE_INTEGER);
        pkm->phys_def = sqlite3_column_int(current_stmt, 6);       
    
        assert(sqlite3_column_type(current_stmt, 7) == SQLITE_INTEGER);
        pkm->spec_atk = sqlite3_column_int(current_stmt, 7);       

        assert(sqlite3_column_type(current_stmt, 8) == SQLITE_INTEGER);
        pkm->spec_def = sqlite3_column_int(current_stmt, 8);       

        assert(sqlite3_column_type(current_stmt, 9) == SQLITE_INTEGER);
        pkm->speed = sqlite3_column_int(current_stmt, 9);       

        return pkm;
    }
    default:
        throw std::runtime_error{ "Unexpected return code from database" };
    }
       
}

pokelib::PokemonType pokelib::Database::get_type_from_name(const char* name)
{
    std::string query = fmt::format(R"(
        SELECT type_id
        FROM PokemonType
        WHERE LOWER(name_en) = LOWER('{}');)", name);
    std::cout << "Query: " << query << std::endl;
    
    if (sqlite3_prepare_v2(sqlite, query.data(), query.size(), &current_stmt, nullptr) != SQLITE_OK)
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

std::string pokelib::Database::get_type_name(pokelib::PokemonType type)
{
    std::string query = fmt::format(R"(
        SELECT name_en
        FROM PokemonType
        WHERE type_id = {};)", static_cast<int>(type));
    std::cout << "Query: " << query << std::endl;
    
    if (sqlite3_prepare_v2(sqlite, query.data(), query.size(), &current_stmt, nullptr) != SQLITE_OK)
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





