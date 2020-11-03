
#include <database.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <cstdio>
#include <cstring>

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

void pokelib::Database::print_all_shit()
{
    assert(good());

    char* error = nullptr;
    auto rc = sqlite3_exec(sqlite, "SELECT * FROM POKEMON;", [](void*, int argc, char** argv, char** argn)
    {
        for (int i = 0; i < argc; i++)
        {
            std::cout << argn[i] << " = ";
            if (argv[i])
                std::cout << argv[i] << "] ";
            else
                std::cout << "NULL] ";

        }
        std::cout << std::endl;
        return 0;
        
    }, nullptr, &error);
    
    if (rc != SQLITE_OK)
    {
        std::string error_str = error;
        sqlite3_free(error);
        throw std::runtime_error { error_str };
    } 
}




