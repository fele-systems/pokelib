#include <dexpokemon.h>
#include <sstream>

void pokelib::DexPokemon::to_entity(Entity& e, const DexPokemon& p)
{
    ENTITY_SET(e, p, pkm_id);
    ENTITY_SET(e, p, national_dex_no);
    ENTITY_SET(e, p, name);
    ENTITY_SET(e, p, ptype);
    ENTITY_SET(e, p, stype);
    ENTITY_SET(e, p, total_hp);
    ENTITY_SET(e, p, phys_atk);
    ENTITY_SET(e, p, phys_def);
    ENTITY_SET(e, p, spec_atk);
    ENTITY_SET(e, p, spec_def);
    ENTITY_SET(e, p, speed);
}

void pokelib::DexPokemon::from_entity(const Entity& e, DexPokemon& p)
{
    ENTITY_GET(e, p, pkm_id);
    ENTITY_GET(e, p, national_dex_no);
    ENTITY_GET(e, p, name);
    ENTITY_GET(e, p, ptype);
    ENTITY_GET(e, p, stype);
    ENTITY_GET(e, p, total_hp);
    ENTITY_GET(e, p, phys_atk);
    ENTITY_GET(e, p, phys_def);
    ENTITY_GET(e, p, spec_atk);
    ENTITY_GET(e, p, spec_def);
    ENTITY_GET(e, p, speed);
}

std::string std::to_string(const pokelib::DexPokemon& pokemon)
{
    std::stringstream ss;
    ss << "DexPokemon {\n"
        << '\t' << pokemon.name << ",\n"
        << '\t' << std::to_string(pokemon.ptype) <<  ",\n"
        << '\t' << std::to_string(pokemon.stype) <<  ",\n"
        << '\t' << pokemon.total_hp << ",\n"
        << '\t' << pokemon.phys_atk << ",\n"
        << '\t' << pokemon.phys_def << ",\n"
        << '\t' << pokemon.spec_atk << ",\n"
        << '\t' << pokemon.spec_def << ",\n"
        << '\t' << pokemon.speed << '\n'
        << '}';
    return ss.str();
}
