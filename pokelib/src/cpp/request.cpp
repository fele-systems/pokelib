#include <request.h>
#include <array>

pokelib::PokemonRequest::PokemonRequest()
{
    buffer << R"(
        SELECT dex_no,
               name,
               types,
               total_stats,
               total_hp,
               phys_atk,
               phys_def,
               spec_atk,
               spec_def,
               speed
        FROM POKEMON WHERE )";
}

pokelib::PokemonRequest& pokelib::PokemonRequest::add_condition(const pokelib::PokemonRequest::Condition& condition)
{
    buffer << std::to_string(condition.field);

    if (condition.op == PokemonFieldOperator::equals)
        buffer << " = '" << condition.value << '\'';
    else // (condition.op == PokemonFieldOperator::contains)
        buffer << " LIKE '%" << condition.value << "%'";

    return *this;
}

pokelib::PokemonRequest& pokelib::PokemonRequest::add_and()
{
    buffer << " AND ";
    return *this;

}

pokelib::PokemonRequest& pokelib::PokemonRequest::add_or()
{
    buffer << " OR ";
    return *this;
}


std::string pokelib::PokemonRequest::build_statement()
{
    buffer << ';';
    auto s = buffer.str();
    buffer = std::stringstream {};
    return s;
}


std::string std::to_string(const pokelib::PokemonField f)
{
    static std::array<const char*, 3> as_str = { "dex_no", "name", "type" };
    return std::string { as_str[ (int) f ] };
}

