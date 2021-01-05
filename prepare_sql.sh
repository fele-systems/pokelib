#!/usr/bin/bash

# Verify if running at the root directory of project
if [[ ! -d 'pokeimporter' ]]; then
    echo "You should execute this script on the root director of the project"
    exit 1
fi

# Remove any previous files
rm *.csv
[ -d sql ] && rm -r sql
mkdir sql
cp pokelib/sql/table_definition.sql sql/00-table_definition.sql
cp pokelib/sql/inserts/PokemonType.sql sql/01-pokemon_type.sql

pokeimporter/importer.py --limit 10

[ -f pokemon_pokedex_entries.csv ] || { echo "Could not locate pokemon_pokedex_entries.csv file"; exit 1; }
[ -f pokemon.csv ] || { echo "Could not locate pokemon.csv file"; exit 1; }

cat pokemon_pokedex_entries.csv | awk 'BEGIN { FS=";";id=1 } { for(i=2;i<NF;i++) { p=index($i,")"); print id++ ";" $1 ";" substr($i,2,p-2) ";" substr($i,p+1) } }' > sql/02-pokedex_entries.csv
# Remove duplicates and double the dexno
cat pokemon.csv | awk 'BEGIN { FS=";";id=0 } { if($1!=id) print "INSERT INTO Pokemon VALUES(" $1 "," $1 ",\"" $2 "\"," $3 "," $4 "," $5 "," $6 "," $7 "," $8 "," $9 "," $10 ");"; id=$1 }' > sql/03-pokemon.sql

echo "All set. Run sql files in sql folter in a sqlite3 context"
