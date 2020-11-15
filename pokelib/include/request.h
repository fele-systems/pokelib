#pragma once
#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <sstream>

namespace pokelib
{
    template<typename Field, typename Operator, typename Value>
    struct RequestCondition
    {
        Field field;
        Operator op;
        Value value;
    };
    
    enum class PokemonField { dex_number, name, types };
    
    enum class PokemonFieldOperator { equals, contains };
    
    class PokemonRequest
    {
    public:
        using Condition = RequestCondition<PokemonField, PokemonFieldOperator, std::string>;
    public:
        PokemonRequest();
        PokemonRequest& add_condition(const Condition& condition);
        PokemonRequest& add_and();
        PokemonRequest& add_or();
        std::string build_statement();
    public:
        std::stringstream buffer;
    }; 
}

namespace std
{
    string to_string(const pokelib::PokemonField f);
}

#endif
