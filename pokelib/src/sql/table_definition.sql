/* Main Table of Pokemon */
-- Enable foreign key support
PRAGMA foreign_keys = ON;

CREATE TABLE PokemonType(
    type_id         PRIMARY KEY ASC,
    name_en         VARCHAR(25),
    name_jp         VARCHAR(25),
    name_pt         VARCHAR(25)
);

CREATE TABLE PokemonGen8 (
    pkm_id          PRIMARY KEY ASC,

    name            VARCHAR(255),
    dex_desc        VARCHAR(255),
    local_dex_no    INTEGER,
    national_dex_no INTEGER,
    
    ptype           INTEGER,
    stype           INTEGER,
    
    total_hp        INTEGER,
    phys_atk        INTEGER,
    phys_def        INTEGER,
    spec_atk        INTEGER,
    spec_def        INTEGER,
    speed           INTEGER,

    FOREIGN KEY(ptype) REFERENCES PokemonType(type_id),
    FOREIGN KEY(stype) REFERENCES PokemonType(type_id)
);

CREATE TABLE PokemonName (
    pkm_id      INTEGER,
    name_en     VARCHAR(255),
    name_jp     VARCHAR(255),
    name_pt     VARCHAR(255),

    FOREIGN KEY(pkm_id) REFERENCES PokemonGen8(pkm_id)
);

CREATE TABLE PokemonAbilityGen8 (
    abl_id      PRIMARY KEY ASC,
    name        VARCHAR(255),
    descr       VARCHAR(255)
);

CREATE TABLE PokemonToAbilityGen8 (
    pkm_id      INTEGER,
    abl_id      INTEGER,
    hidden      BOOLEAN,

    FOREIGN KEY(pkm_id) REFERENCES PokemonGen8(pkm_id),
    FOREIGN KEY(abl_id) REFERENCES PokemonAbilityGen8(abl_id)
);

CREATE TABLE PokemonMoveGen8 (
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

CREATE TABLE PokemonLeanSetGen8 (
    pkm_id      INTEGER,
    move_id     INTEGER,
    by_tm       INTEGER, -- If non-zero, indicates which TM this pokemon can learn this move
    by_level    INTEGER  -- If non-zero, indicates at which level the pokemon learns this move
);
