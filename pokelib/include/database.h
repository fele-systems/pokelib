#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <pokemon.h>
#include <stdexcept>
#include <pkm_type.h>

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

        template<typename RequestType>
        void request(RequestType& r);

        void request_pokemon(const char* name);
        void request_fuzzy_search(const char* value);

        std::shared_ptr<Pokemon> fetch_request();

        PokemonType get_type_from_name(const char* name);
        std::string get_type_name(PokemonType type);
    
        void print_all_shit();
    private:
        sqlite3* sqlite;
        bool open_code = -1;
        sqlite3_stmt* current_stmt = nullptr;
    };


    template<typename RequestType>
    void Database::request(RequestType& r)
    {
        auto query = r.build_statement();
        std::cout << query << std::endl;
        if (sqlite3_prepare_v2(sqlite, query.c_str(), query.size(), &current_stmt, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error{ sqlite3_errmsg(sqlite) };
        }
    }

}

#endif
