#include <dexpokemontype.h>

void pokelib::DexPokemonType::to_entity(Entity& e, const DexPokemonType& p)
{
	ENTITY_SET(e, p, type_id);
	ENTITY_SET(e, p, name_en);
	ENTITY_SET(e, p, name_jp);
	ENTITY_SET(e, p, name_pt);
}

void pokelib::DexPokemonType::from_entity(const Entity& e, DexPokemonType& p)
{
	ENTITY_GET(e, p, type_id);
	ENTITY_GET(e, p, name_en);
	ENTITY_GET(e, p, name_jp);
	ENTITY_GET(e, p, name_pt);
}