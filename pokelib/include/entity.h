#pragma once

#include <vector>
#include <variant>
#include <string>
#include <algorithm>
#include <type_traits>
#include <stdexcept>
#include <cassert>
#include <concepts>

#include <fmt/core.h>
#include <sqlite3.h>

#include <reflect.h>


#define ENTITY_SET(model, entity, field) model[ # field ].set_from(entity . field)
#define ENTITY_GET(model, entity, field) model[ # field ].get_to(entity . field)

namespace entity {

	template <typename F, typename T>
	struct Converter
	{
		static constexpr bool enabled = false;
		T operator()(F f) const
		{
			static_assert(false, "No conversion from F to T");
		}
	};

	template <typename T>
	concept EntityFundamentalType = std::is_same_v<T, std::string>
		|| std::is_same_v<T, uint32_t>;

	template <typename T>
	concept EntityConvertibleType = Converter<T, std::string>::enabled
		|| Converter<T, uint32_t>::enabled;

	struct EntityField
	{
	public:
		using var_t = std::variant<std::string, uint32_t>;
	public:
		template <typename T>
		requires EntityFundamentalType<T> || EntityConvertibleType<T>
		void get_to(T& out) const;

		template <typename T>
			requires EntityFundamentalType<T> || EntityConvertibleType<T>
		void set_from(T value);
	public:
		std::string field_name;
		var_t value;
	};

	template <typename T>
	class EntityModel
	{
	public:
		EntityField& operator[](std::string field_name);
		const EntityField& operator[](std::string field_name) const;
		const std::vector<EntityField>& get_fields() const;
	private:
		std::vector<EntityField> fields;
	};


	template <typename T>
	class SelectQueryBuilder
	{
		std::string table_name;

	};

	/*
	using q = entity::dsl;
	entity::select<DexPokemon>()
		.where( q::field("Name") == "Lucario" && q::field("Type )
	
	*/

	template <typename E, typename T>
	std::string find_by(const std::string& table_name, const std::string& field_name, const T& field_value)
	{
		std::string query = "SELECT ";
		E object;
		EntityModel<E> entity;
		E::to_entity(entity, object);
		assert(!entity.get_fields().empty());

		auto itr = entity.get_fields().cbegin();
		auto end = entity.get_fields().cend();

		query += itr->field_name;
		while (++itr != end)
		{
			query += ',' + itr->field_name;
		}

		return query + " FROM " + table_name + " WHERE " + field_name + "='" + field_value + "'";
	}

	template <typename T>
	inline EntityField& EntityModel<T>::operator[](std::string field_name)
	{
		auto itr = std::find_if(fields.begin(), fields.end(), [&](auto& field)
		{
			return field.field_name == field_name;
		});

		if (itr != fields.end())
		{
			return *itr;
		}
		else
		{
			fields.emplace_back();
			fields.back().field_name = std::move(field_name);
			return fields.back();
		}
	}

	template <typename T>
	inline const EntityField& EntityModel<T>::operator[](std::string field_name) const
	{
		auto itr = std::find_if(fields.cbegin(), fields.cend(), [&](const auto& field)
			{
				return field.field_name == field_name;
			});

		if (itr != fields.end())
		{
			return *itr;
		}
		else
		{
			throw std::runtime_error{ fmt::format("No such field: {} for entity {}", field_name, TypeName<T>()) };
		}
	}

	template <typename T>
	inline const std::vector<EntityField>& EntityModel<T>::get_fields() const
	{
		return fields;
	}

	template <typename T>
	requires EntityFundamentalType<T> || EntityConvertibleType<T>
	inline void EntityField::get_to(T& out) const
	{
		if constexpr (std::is_convertible_v<T, var_t>)
		{
			out = std::get<T>(value);
		}
		else // needs casting
		{
			out = std::visit([](const auto& arg) -> T
			{
				using Type = std::decay_t<decltype(arg)>;
				if constexpr (Converter<Type, T>::enabled)
					return Converter<Type, T>()(arg);
				else
					throw std::runtime_error{ fmt::format("No conversion found from {} to {}", TypeName<Type>(), TypeName<T>()) };
			}, value);
		}
	}

	template <typename T>
	requires EntityFundamentalType<T> || EntityConvertibleType<T>
	inline void EntityField::set_from(T value)
	{
		using Type = std::decay_t<T>;
		if constexpr (std::is_convertible_v<T, var_t>)
			this->value = std::move(value);
		else if constexpr (Converter<Type, uint32_t>::enabled)
			this->value = Converter<Type, uint32_t>()(value);
		else if constexpr (Converter<Type, std::string>::enabled)
			this->value = Converter<Type, std::string>()(value);
		else
			static_assert(false, "Non convertible type!");
	}

	template <typename T>
	std::vector<T> fetch_from_statement(sqlite3_stmt* stmt)
	{
		std::vector<T> results;

		auto rc = sqlite3_step(stmt);
		while (rc == SQLITE_ROW)
		{
			T::Entity entity;
			const auto count = sqlite3_column_count(stmt);
			for (int i = 0; i < count; i++)
			{
				const char* column_name = sqlite3_column_name(stmt, i);
				auto& field = entity[column_name];
				const auto column_type = sqlite3_column_type(stmt, i);
				if (column_type == SQLITE_INTEGER)
				{
					field.set_from(static_cast<uint32_t>(sqlite3_column_int(stmt, i)));
				}
				else if (column_type == SQLITE_TEXT)
				{
					auto size = static_cast<size_t>(sqlite3_column_bytes(stmt, i));
					auto chars = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
					field.set_from(std::string{ chars, size });
				}
				else
				{
					throw std::runtime_error{ field.field_name + ": Unknown type" };
				}
			}
			results.emplace_back();
			T::from_entity(entity, results.back());

			rc = sqlite3_step(stmt);
		}

		if (rc == SQLITE_DONE)
		{
			return results;
		}
		else
		{
			throw std::runtime_error{ "Unexpected return code from database" };
		}
	}
}

namespace std {
	template<typename T>
	std::string to_string(const entity::EntityModel<T>& entity)
	{
		std::string str = fmt::format("{} [", TypeName<T>());
		
		for (const auto& field : entity.get_fields())
		{
			str += "\t" + field.field_name + ": " + std::visit([](auto&& arg) -> std::string
			{
				if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) return arg;
				else return std::to_string(arg);
			}, field.value) + '\n';
		}

		return str + ']';
	}
}