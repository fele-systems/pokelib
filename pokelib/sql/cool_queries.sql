SELECT  pkm.national_dex_no,
        pkm.name, 
        type1.name_en, 
        type2.name_en
FROM Pokemon pkm
    INNER JOIN PokemonType type1 ON pkm.ptype = type1.type_id
    LEFT JOIN PokemonType type2 ON pkm.stype = type2.type_id    -- INNER JOIN shows only matches on both sides, but type2 can be null so we should use LEFT JOIN
    ORDER BY pkm.national_dex_no ASC;
