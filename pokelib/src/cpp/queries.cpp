#include <queries.h>
#include <sqlite3.h>

std::string get_first_pokemon()
{
    return std::string {"bulbasaur: Provided for you by "} + sqlite3_libversion();
}

