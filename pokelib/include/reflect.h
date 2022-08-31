#include <array>
#include <cstddef>

namespace impl
{
    template <typename T>
    constexpr const auto& RawTypeName()
    {
#ifdef _MSC_VER
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    struct RawTypeNameFormat
    {
        std::size_t leading_junk = 0, trailing_junk = 0;
    };

    // Returns `false` on failure.
    inline constexpr bool GetRawTypeNameFormat(RawTypeNameFormat* format)
    {
        const auto& str = RawTypeName<int>();
        for (std::size_t i = 0;; i++)
        {
            if (str[i] == 'i' && str[i + 1] == 'n' && str[i + 2] == 't')
            {
                if (format)
                {
                    format->leading_junk = i;
                    format->trailing_junk = sizeof(str) - i - 3 - 1; // `3` is the length of "int", `1` is the space for the null terminator.
                }
                return true;
            }
        }
        return false;
    }

    inline static constexpr RawTypeNameFormat format =
        [] {
        static_assert(GetRawTypeNameFormat(nullptr), "Unable to figure out how to generate type names on this compiler.");
        RawTypeNameFormat _format;
        GetRawTypeNameFormat(&_format);
        return _format;
    }();
}

// Returns the type name in a `std::array<char, N>` (null-terminated).
template <typename T>
[[nodiscard]] constexpr auto CexprTypeName()
{
    constexpr std::size_t len = sizeof(impl::RawTypeName<T>()) - impl::format.leading_junk - impl::format.trailing_junk;
    std::array<char, len> name{};
    for (std::size_t i = 0; i < len - 1; i++)
        name[i] = impl::RawTypeName<T>()[i + impl::format.leading_junk];
    return name;
}

template <typename T>
[[nodiscard]] const char* TypeName()
{
    static constexpr auto name = CexprTypeName<T>();
    return name.data();
}
template <typename T>
[[nodiscard]] const char* TypeName(const T&)
{
    return TypeName<T>();
}