#include <queries.h>
#include <database.h>
#include <iostream>

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        std::cout << "Invalid argument! Usage: pokedex-cli DATABASE_PATH" << std::endl;
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
                db.print_all_shit();
            }
    }

    return EXIT_SUCCESS;
}
