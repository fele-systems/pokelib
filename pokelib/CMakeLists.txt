project(pokelib)
# Get all source files in src
file(GLOB SOURCES "src/cpp/*.cpp")

message(STATUS ${SOURCES})

add_library(pokelib ${SOURCES})

target_link_libraries(pokelib -lsqlite3 -lfmt)

target_include_directories(pokelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(pokelib PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)


