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
        ("n,name", "Search by a pokémon full name")
        ("f,fuzzy", "Search by name or type using fuzzy searching")
        ("t,type", "Get type effectiveness")
        ("h,help", "Print usage")
        ("INPUT", "Positional arguments: values used depending on the requested operation",
            cxxopts::value<std::vector<std::string>>())
        ;

    options.parse_positional({ "INPUT" });
    auto result = options.parse(argc, argv);
    PokemonRequest::Condition condition;
    if (!result.count("INPUT"))
    {
        std::cout << "Missing input values" << std::endl;
        return EXIT_FAILURE;
    }
    auto input = result["INPUT"].as<std::vector<std::string>>();

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (result.count("name"))
    {
        condition.field = PokemonField::name;
        condition.op = PokemonFieldOperator::equals;
        condition.value = input[0];
    }
    else if (result.count("fuzzy"))
    {
        condition.field = PokemonField::name;
        condition.op = PokemonFieldOperator::contains;
        condition.value = input[0];
    }
    else if (result.count("type"))
    {
        pokelib::Database database{ result["database"].as<std::string>() };
        if (!database.good())
        {
            std::cout << database.get_error() << std::endl;
            return EXIT_FAILURE;
        }
        auto atk = database.get_type_from_name(input[0].c_str());
        auto def = database.get_type_from_name(input[1].c_str());
        
        auto res = get_affinity(atk, def);
        std::cout << res << std::endl;
        return EXIT_SUCCESS;
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
