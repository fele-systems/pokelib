#include <queries.h>
#include <database.h>
#include <iostream>

int main(int argc, char** argv)
{

    if (argc != 3)
    {
        std::cout << "Invalid argument! Usage: pokedex-cli DATABASE_PATH POKEMON_NAME" << std::endl;
    }
    else
    {
            pokelib::Database db { argv[1] };

            if (!db.good())
            {
                std::cout << db.get_error() << std::endl;
            }
            else
            {
                db.request_pokemon(argv[2]);
                auto pkm = db.fetch_request();
                if (pkm != nullptr)
                    std::cout << std::to_string(*pkm) << std::endl;
                else
                    std::cout << "No results for: " << argv[2] << std::endl;
            }
    }

    return EXIT_SUCCESS;
}
