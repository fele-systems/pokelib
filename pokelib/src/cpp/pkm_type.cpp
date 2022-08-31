#include <pkm_type.h>

std::string std::to_string(pokelib::PokemonType type)
{
	return pokelib::types_str[static_cast<size_t>(type)];
}