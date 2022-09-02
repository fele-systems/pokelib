project(pokedex-cli)
# Get all source files in src
add_executable(pokedex-cli src/main.cpp)

target_link_libraries(pokedex-cli pokelib)
message(STATUS "Caminho do include: ")
message(STATUS ${pokelib_SOURCE_DIR})
include_directories(
        include
        ${pokelib_SOURCE_DIR}/include
        third-party/cxxopts/include)
target_compile_options(pokelib PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

add_custom_command(TARGET pokedex-cli POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
		   ${CMAKE_SOURCE_DIR}/pokelib.db $<TARGET_FILE_DIR:pokedex-cli>)
