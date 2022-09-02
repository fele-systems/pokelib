project(pokelib)

message(STATUS ${SOURCES})

add_library(pokelib "src/cpp/dexpokemon.cpp"
                    "src/cpp/database.cpp"
                    "src/cpp/queries.cpp"
                    "src/cpp/request.cpp"
                    "src/cpp/pkm_type.cpp"
                    "src/cpp/dexpokemontype.cpp")

if (MSVC)
    find_package(Catch2 CONFIG REQUIRED)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    target_link_libraries(pokelib PRIVATE unofficial::sqlite3::sqlite3 Catch2::Catch2)

    find_package(fmt 7.1.3 CONFIG REQUIRED)
    target_link_libraries(pokelib PRIVATE fmt::fmt)
else ()
    target_link_libraries(pokelib -lsqlite3 -lfmt)
endif (MSVC)

target_include_directories(pokelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(pokelib PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)


