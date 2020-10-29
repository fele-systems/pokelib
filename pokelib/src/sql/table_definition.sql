/* Main Table of Pokemon */

CREATE TABLE IF NOT EXISTS POKEMON (
    dex_no      INTEGER,
    name        VARCHAR(255),
    types       VARCHAR(50),
    total_stats INTEGER,
    total_hp    INTEGER,
    phys_atk    INTEGER,
    phys_def    INTEGER,
    spec_atk    INTEGER,
    spec_def    INTEGER,
    speed       INTEGER)
