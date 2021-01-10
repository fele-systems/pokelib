#include <pokemon.h>
#include <sstream>


std::string std::to_string(const pokelib::DexPokemon& pokemon)
{
    std::stringstream ss;
    ss << "DexPokemon {\n"
        << '\t' << pokemon.name << ",\n"
        << '\t' << pokemon.ptype <<  ",\n"
        << '\t' << pokemon.stype <<  ",\n"
        << '\t' << pokemon.total_hp << ",\n"
        << '\t' << pokemon.phys_atk << ",\n"
        << '\t' << pokemon.phys_def << ",\n"
        << '\t' << pokemon.spec_atk << ",\n"
        << '\t' << pokemon.spec_def << ",\n"
        << '\t' << pokemon.speed << '\n'
        << '}';
    return ss.str();
}
