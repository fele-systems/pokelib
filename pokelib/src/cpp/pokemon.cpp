#include <pokemon.h>
#include <sstream>


std::string std::to_string(const pokelib::Pokemon& pokemon)
{
    std::stringstream ss;
    ss << "Pokemon {\n"
        << '\t' << pokemon.name << ",\n"
        << '\t' << pokemon.types <<  ",\n"
        << '\t' << pokemon.total_stats << ",\n"
        << '\t' << pokemon.total_hp << ",\n"
        << '\t' << pokemon.phys_atk << ",\n"
        << '\t' << pokemon.phys_def << ",\n"
        << '\t' << pokemon.spec_atk << ",\n"
        << '\t' << pokemon.spec_def << ",\n"
        << '\t' << pokemon.speed << '\n'
        << '}';
    return ss.str();
}
