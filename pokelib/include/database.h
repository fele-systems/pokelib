#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include <memory>
#include <sqlite3.h>

namespace pokelib
{
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
    
        void print_all_shit();
    private:
        sqlite3* sqlite;
        bool open_code = -1;
    };

}

#endif
