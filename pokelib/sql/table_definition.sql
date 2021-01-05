/* Main Table of Pokemon */
-- Enable foreign key support
PRAGMA foreign_keys = ON;
.separator ";"

/***************************************************
    Base Tables
****************************************************/

CREATE TABLE Games(
    gen     PRIMARY KEY ASC,
    name_en VARCHAR(25)
);

CREATE TABLE PokemonType(
    type_id         PRIMARY KEY ASC,
    name_en         VARCHAR(25),
    name_jp         VARCHAR(25),
    name_pt         VARCHAR(25)
);

.read 01-pokemon_type.sql
-- #include ./PokemonType.sql

CREATE TABLE Pokemon (
    pkm_id          PRIMARY KEY ASC,

    national_dex_no INTEGER,
    name            VARCHAR(255),
    
    ptype           INTEGER,
    stype           INTEGER NULL,
    
    total_hp        INTEGER,
    phys_atk        INTEGER,
    phys_def        INTEGER,
    spec_atk        INTEGER,
    spec_def        INTEGER,
    speed           INTEGER,

    FOREIGN KEY(ptype) REFERENCES PokemonType(type_id),
    FOREIGN KEY(stype) REFERENCES PokemonType(type_id)
);
CREATE TABLE TmpPokemon (
    pkm_id          PRIMARY KEY ASC,

    national_dex_no INTEGER,
    name            VARCHAR(255),
    
    ptype           INTEGER,
    stype           INTEGER NULL,
    
    total_hp        INTEGER,
    phys_atk        INTEGER,
    phys_def        INTEGER,
    spec_atk        INTEGER,
    spec_def        INTEGER,
    speed           INTEGER
); 
.import 03-pokemon.sql Pokemon

CREATE TABLE PokeDexEntries (
    entry_id    PRIMARY KEY ASC,
    
    pkm_id      INTEGER,

    games       VARCHAR(128),
    descr       VARCHAR(255),

    FOREIGN KEY(pkm_id) REFERENCES Pokemon(pkm_id)
);

-- .import 02-pokedex_entries.csv PokeDexEntries

CREATE TABLE PokemonName (
    pkm_id      INTEGER,
    name_en     VARCHAR(255),
    name_jp     VARCHAR(255),
    name_pt     VARCHAR(255),

    FOREIGN KEY(pkm_id) REFERENCES Pokemon(pkm_id)
);

CREATE TABLE PokemonAbility (
    abl_id      PRIMARY KEY ASC,
    name        VARCHAR(255),
    descr       VARCHAR(255)
);

CREATE TABLE PokemonToAbility (
    pkm_id      INTEGER,
    abl_id      INTEGER,
    hidden      BOOLEAN,

    FOREIGN KEY(pkm_id) REFERENCES Pokemon(pkm_id),
    FOREIGN KEY(abl_id) REFERENCES PokemonAbility(abl_id)
);

CREATE TABLE PokemonMove (
    move_id     PRIMARY KEY ASC,
    name        VARCHAR(255),
    descr       VARCHAR(255),
    pow         INTEGER,
    type_id     INTEGER,
    cat         INTEGER,
    acc         INTEGER,
    pp          INTEGER,

    FOREIGN KEY(type_id) REFERENCES PokemonType(type_id)
);

CREATE TABLE PokemonLeanSet (
    pkm_id      INTEGER,
    move_id     INTEGER,
    by_tm       INTEGER, -- If non-zero, indicates which TM this pokemon can learn this move
    by_level    INTEGER  -- If non-zero, indicates at which level the pokemon learns this move
);
