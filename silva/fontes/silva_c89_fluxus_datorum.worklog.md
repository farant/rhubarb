# silva_c89_fluxus_datorum.c — worklog

## 2026-11-05 — variable index maps (budget fusionis, main walk item 1)

`_variabilis_index` and `_variabilis_membri_index` were find-or-add by linear scan. Corpus-wide that was 1.52 M calls / 15.2 M steps here, and semantics' interval lookups did another 2.9 M calls / 72 M steps over the same list. Now `FluxusDatorum` carries two `TabulaDispersa*` (NIHIL below `FLUXUS_INDEX_LIMEN` = 16 variables; the average function has ~7, where a linear scan is cheaper than hashing).
- **Key:** the identity pointer's bytes (+ member title bytes), composed in a caller stack buffer of 256 (lookups allocate nothing). Keys are copied into the arena only on insert.
- **Order:** insert only if absent, so the FIRST index wins, which is what the linear scan returned. (There are no duplicates anyway; every variable is created by find-or-add.)
- **Threshold crossing:** the map is built from all existing entries in order, then maintained on each append.
- **Long titles** (> 248 bytes) aren't indexed, and their lookups fall back to the linear scan, so the two paths agree.
- **Public:** `silva_c89_fluxus_datorum_variabilis` / `_membrum` (map or linear), used by semantics' interval lookups. Data flow builds its own index and semantics only reads it, so the header rule (semantics reads datorum, never the reverse) holds.
