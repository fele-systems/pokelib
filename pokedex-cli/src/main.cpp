#include <queries.h>
#include <database.h>
#include <request.h>
#include <iostream>
#include <cxxopts.hpp>
#include <entity.h>

#ifdef __WIN32__
#define OS_WINDOWS
#elif _MSC_VER
#define OS_WINDOWS
#include <Windows.h>
#else
#define OS_UNIX
#endif

std::unique_ptr<pokelib::PokeDex> load(const std::string& dbfile)
{
    auto database = std::make_unique<pokelib::PokeDex> ( dbfile );
    if (!database->good())
    {
        std::cout << database->get_error() << std::endl;
        return nullptr;
    }
    return database;
}

namespace pokelib {

}

int main(int argc, char** argv)
{


#ifdef OS_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
#endif
    
    using namespace pokelib;
    cxxopts::Options options("Pokedex-cli", "Command-line pokémon tools");

    options.add_options()
        ("d,database", "Specify the database location", cxxopts::value<std::string>()->default_value("pokelib.db"))
        ("v,verbose", "Enable verbose logging")
        ("n,name", "Search by a pokémon full name")
        ("f,fuzzy", "Search by name or type using fuzzy searching")
        ("t,type", "Get type effectiveness")
        ("h,help", "Print usage")
        ("INPUT", "Positional arguments: values used depending on the requested operation",
            cxxopts::value<std::vector<std::string>>())
        ;
    try
    {
        options.parse_positional({ "INPUT" });
        auto result = options.parse(argc, argv);
        auto dbfile = result["database"].as<std::string>();

        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return EXIT_SUCCESS;
        }

        if (!result.count("INPUT"))
        {
            std::cout << "Missing input values" << std::endl;
            std::cout << options.help() << std::endl;
            return EXIT_FAILURE;
        }
        auto input = result["INPUT"].as<std::vector<std::string>>();

        if (result.count("name"))
        {
            if (input.size() != 1)
                throw std::runtime_error { "--name should take only 1 parameter" };
            auto db = load(dbfile);
            auto pokemon = db->pokemon(input[0]);
            std::cout << std::to_string(pokemon) << std::endl;
        }
        else if (result.count("fuzzy"))
        {
            if (input.size() != 1)
                throw std::runtime_error { "--fuzzy should take only 1 parameter" };
            auto db = load(dbfile);
            auto pokemons = db->search_pokemon(input[0], Field::all);
            for (auto& pokemon : pokemons)
            {
                std::cout << std::to_string(pokemon) << std::endl;
            }
        }
        else if (result.count("type"))
        {
            if (input.size() != 2)
                throw std::runtime_error { "--type should take only 1 parameter" };
            auto db = load(dbfile);
            auto atk = db->get_type_from_name(input[0].c_str()).type_id;
            auto def = db->get_type_from_name(input[1].c_str()).type_id;
            
            auto res = get_affinity((PokemonType) atk, (PokemonType) def);
            std::cout << affinity_str[res] << std::endl;
            return EXIT_SUCCESS;
        }
        else
        {
            std::cout << "Must use at least one of --name or --fuzzy" << std::endl;
            std::cout << options.help() << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
