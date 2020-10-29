
#include <database.h>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cstdio>

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




