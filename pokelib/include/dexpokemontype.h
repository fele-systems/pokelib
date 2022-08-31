#pragma once

#include <entity.h>

namespace pokelib {
	struct DexPokemonType
	{
	public:
		using Entity = entity::EntityModel<DexPokemonType>;
		static void to_entity(Entity& e, const DexPokemonType& p);
		static void from_entity(const Entity& e, DexPokemonType& p);
	public:
		uint32_t type_id;
		std::string name_en;
		std::string name_jp;
		std::string name_pt;
	};
}