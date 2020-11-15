#include <queries.h>
#include <database.h>
#include <request.h>
#include <iostream>
#include <cxxopts.hpp>

int main(int argc, char** argv)
{
    using namespace pokelib;
    cxxopts::Options options("Pokedex-cli", "Command-line pokémon tools");

    options.add_options()
        ("d,database", "Specify the database location", cxxopts::value<std::string>()->default_value("pokemon.db"))
        ("v,verbose", "Enable verbose logging")
        ("n,name", "Search by a pokémon full name", cxxopts::value<std::string>())
        ("f,fuzzy", "Search by name or type using fuzzy searching", cxxopts::value<std::string>())
        ("h,help", "Print usage");

    options.parse_positional({ "database" });
    auto result = options.parse(argc, argv);
    PokemonRequest::Condition condition;

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (result.count("name"))
    {
        condition.field = PokemonField::name;
        condition.op = PokemonFieldOperator::equals;
        condition.value = result["name"].as<std::string>();
    }
    else if (result.count("fuzzy"))
    {
        condition.field = PokemonField::name;
        condition.op = PokemonFieldOperator::contains;
        condition.value = result["fuzzy"].as<std::string>();
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

    database.request(PokemonRequest()
        .add_condition(condition));

    auto pkm = database.fetch_request();
    while (pkm != nullptr)
    {
        std::cout << std::to_string(*pkm) << std::endl;
        pkm = database.fetch_request();
    }

    return EXIT_SUCCESS;
}
