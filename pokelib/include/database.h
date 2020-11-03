#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <memory>
#include <sqlite3.h>
#include <pokemon.h>

namespace pokelib
{
    // A Gen-8 Pokémon database
    class Database
    {
    public:
        Database(const std::string& filename);
        ~Database();
        Database(const Database&) = delete;
        Database(Database&&) = delete;
        Database& operator=(const Database&) = delete;
        Database& operator=(Database&&) = delete;
    
        void reload(const std::string& filename);
        bool good() const;
        std::string get_error() const;

        void request_pokemon(const char* name);
        std::shared_ptr<Pokemon> fetch_request();
    
        void print_all_shit();
    private:
        sqlite3* sqlite;
        bool open_code = -1;
        sqlite3_stmt* current_stmt = nullptr;
    };
}

#endif
