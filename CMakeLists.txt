project( pokedex )
cmake_minimum_required( VERSION 2.8 )

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif (MSVC)

# Include pokelib library
add_subdirectory( pokelib )

# Include our executable for testing
add_subdirectory( pokedex-cli )

#add_executable( ${PROJECT_NAME} main.cpp )
#target_link_libraries(${PROJECT_NAME} fmt)
