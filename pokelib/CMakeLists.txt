project(pokelib)
# Get all source files in src
file(GLOB SOURCES "src/cpp/*.cpp")

message(STATUS ${SOURCES})

add_library(pokelib ${SOURCES})

if (MSVC)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    target_link_libraries(pokelib PRIVATE unofficial::sqlite3::sqlite3)

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


