# K3 EXPLORATIO 2 — suturae nostrae (gesta/tabularium)
# Relatio exploratoris verbatim (compressa), 2026-07-16. Fons pro spec-v2.

## 1. FASCIS ATOMICUS - refactor _scribere_crudum (gesta.c:1176-1440)
Factor out 3 transactionless helpers + driver:
- _eventum_praeparare (resolve res_id/canon/flags; ULID minting for
  creatio MOVES OUT to caller - executor binds $novus)
- _eventum_validare (1251-1352 verbatim; pure reads -> violatio|NIHIL)
- _eventum_inserere (INSERT+seq readback+creatum+annales+sordidae,
  NO envelope; runs inside open txn)
Driver gesta_fascis_scribere(m, GestaFascisEventum* eventa {event_id
NIHIL=mint, eventum}, numerus, res_ids_out):
per-event praeparare+validare; work-list interleaves custodia notae
AFTER producing event (notae INTRA fascem - kills second
_scribere_crudum); one BEGIN; loop inserere; one COMMIT. N annales
lines in seq order ante COMMIT (lex K1 superset servata).
$nunc: creatum of FIRST row reused for all (strftime advances between
statements - one stable bundle timestamp).
gesta_scribere -> 1-element wrapper + plicare + advancement drain.

## 2. RE-ENTRANTIA - drain post-commit, NON recursio
scrinium: NO nested txns (incipere = raw BEGIN, scrinium.c:213-217).
Advancement reads FOLDS (updated only by plicare, own txns). Shape:
_provehere_ad_fixum: loop { computare(reads fold) -> empty? done ->
fascis_scribere(nova) -> plicare }. Trigger gate: per new STATUS
event, ONE probe idx_membra_membrum -> member of running
species:processus instance? Chunk B inert without instances.
Replay-safe: replicare/restituere call only plicare - engine never
re-fires. PARK: piscina mundi grows monotonically per bundle (256MB
arena, long-lived resident + bulk agere).

## 3. MEMBRA - one indexed query, no new index
SELECT m.res_id, m.pars, r.genus, r.status FROM membra m JOIN res r
ON r.res_id=m.res_id WHERE m.membrum=?1;  (idx_membra_membrum)
then species:processus + status currens filter. = decision 10's "one
lookup". Same shape as gesta_nexus_rei.

## 4. SPECIES - generalize _species_nexus_est (286-311) ->
_species_est(m, titulus, species, piscina); thin nexus wrapper keeps
4 call sites (437, 967, 1748, 2644) unchurned.

## 5. AFFORDANTIAE - full genera scan FINE (6 genera today, 9 post
seed-v3; species inside JSON datum, no column, none needed). res tool
appends actiones: line (all pins are strstr-presence - safe).

## 6. AGERE + SEMEN V3 - dispatch in _toolscall_tractare (2140-2165);
6th instrumentum in _toolslist (1953-1974); doctrine grows SAFELY
(pin = strstr CONTRACTUS SCRIBAE only). Seed v3 = +3 SEMINA_GENERUM
entries, absent-by-title idempotent. BUT see B2.

## 7. FRUSTA - A (bundle+actio, new symbols; gesta_scribere behavior
byte-identical), B (processus+advancement, inert-without-instances),
C (tenant). All pins additive-safe; no count assertions.

## SPEC V1 BREAKS
B1 (MUST-FIX, silent): _membrum_validare requires container species
    nexus (436-440) -> every instance->opus link draws "membrum in
    genere non-nexu" custodia. Gate on "genus declares partes OR
    species processus" (also solutum-purge :967). No current test
    can catch (no process genus exists).
B2 (MUST-FIX, silent): seed v2 attributa-fusion (tabularium.c:
    2004-2037) is UNSCOPED - would stamp board attributa onto
    opus/actio/processus genera. Split SEMINA_BOARD_NUMERUS from
    total, or guard on species/opes/gradus presence.
B3 (DECIDE): intra-bundle validation reads stale fold - effect
    referencing $novus res created earlier in SAME bundle draws
    spurious "membrum inexistens" custodia. Options: accept noise /
    in-memory overlay of bundle-created res / skip existence checks
    for $novus referents. Recommend overlay or skip-for-$novus.
    (Flagship's optional 4th linking effect = exactly this case.)
B4 (MECHANISM): actio-facta same-bundle REQUIRES pre-minted event
    ULIDs (GestaFascisEventum.event_id) so eventus:[ids] listable
    before write. Commit to carrier-with-id.
B5 (VERIFY): all 7 verbs non-generis -> sordidae correct; test that
    actio res FTS corpus doesn't bloat from facta stream.

## ADDENDUM (E1 reconciliation)
Oracle re-read-after-append == our drain loop (each iteration:
fascis -> plicare -> computare reads fold). Advancement spawns stay
SMALL per micro-bundle; drain re-folds between -> B3 never bites
across steps (only within one actio's own bundle). Oracle
completeTask-only trigger == our status-events-only membra probe
(don't fire on nota/mutatio/creatio). Direct pendens->perfectum is
still a status event -> triggers correctly, no special case.
