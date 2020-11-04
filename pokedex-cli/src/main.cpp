#include <queries.h>
#include <database.h>
#include <iostream>
#include <cxxopts.hpp>

enum class DBOperation { FUZZY_SEARCH, NAME_SEARCH };

int main(int argc, char** argv)
{
    cxxopts::Options options("Pokedex-cli", "Command-line pokémon tools");

    options.add_options()
        ("d,database", "Specify the database location", cxxopts::value<std::string>()->default_value("pokemon.db"))
        ("v,verbose", "Enable verbose logging")
        ("n,name", "Search by a pokémon full name", cxxopts::value<std::string>())
        ("f,fuzzy", "Search by name or type using fuzzy searching", cxxopts::value<std::string>())
        ("h,help", "Print usage");

    options.parse_positional({ "database" });
    auto result = options.parse(argc, argv);
    DBOperation operation;

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (result.count("name"))
    {
        operation = DBOperation::NAME_SEARCH;
    }
    else if (result.count("fuzzy"))
    {
        operation = DBOperation::FUZZY_SEARCH;
    }
    else
    {
        std::cout << "Must use at least one of --name or --fuzzy" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }
    
    pokelib::Database database{ result["database"].as<std::string>() };
    if (!database.good())
    {
        std::cout << database.get_error() << std::endl;
        return EXIT_FAILURE;
    }

    switch (operation)
    {
    case DBOperation::NAME_SEARCH:
        database.request_pokemon(result["name"].as<std::string>().c_str());
        break;
    case DBOperation::FUZZY_SEARCH:
        database.request_fuzzy_search(result["fuzzy"].as<std::string>().c_str());
        break;
    default:
        std::cout << "unkown error: Invalid operation (" << (int) operation << ")" << std::endl;
        return EXIT_FAILURE;
    }

    auto pkm = database.fetch_request();
    while (pkm != nullptr)
    {
        std::cout << std::to_string(*pkm) << std::endl;
        pkm = database.fetch_request();
    }

    return EXIT_SUCCESS;
}
