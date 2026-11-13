# tabula_monstratorum worklog

## 2026-11-06 — born (silva budget item 6)

Pointer -> pointer map for side tables keyed by object identity. First
users: silva semantica's `typationes` (node -> SemanticaTypatio) and
`nexus` (node -> SemanticaNexus), ~48 M lookups + ~15 M inserts per
`percursus -semantica` run. Those were `tabula_dispersa` tables keyed by
the pointer's 8 bytes as a chorda.

Prototype measurement (whole run, 3 rounds each, report identical):
- HEAD tabula_dispersa: 346.35 G
- same table, pointer hash/compare as friatio/comparatio: 343.77 G (-2.6)
- this design (prototype, inlined in semantica): 339.62 G (-6.7)
So the hash itself was only ~2.6 G; the rest was the general table:
40-byte slots, two indirect calls per operation, float load factor per
insert, lookup-then-insert.

Design notes:
- Slot = {clavis, valor}, NIHIL key = empty. No deletion, so no
  tombstones; no iteration, so no order can leak into output.
- Index = hash & mask, so the mixer must spread LOW bits: drop 4
  alignment bits, fold the high word in (two 16-bit shifts so a 32-bit
  memoriae_index stays defined), xorshift-multiply-xorshift. A plain
  multiplicative hash would leave the low bits weak.
- `locus` = find-or-insert. A present key returns before the load check,
  so it never grows the table; a new key may grow it, which moves slots
  -> the returned pointer is valid only until the next `locus`. Piscina
  allocations in between are fine (arena blocks don't move).
- Growth leaves the old slot array in the piscina (like tabula_dispersa).
- Uses `piscina_conari_allocare`: `piscina_allocare` aborts the process
  when an arena can't grow, which would make the documented NIHIL return
  a lie. The test proves it with a fixed-size arena.
- Statics must be unique across silva's amalgam (one translation unit):
  the first name `_crescere` collided with lib/chorda_aedificator.c and
  lib/dispositio.c -> `_tabulam_monstratorum_crescere`.

Wiring a new lib into silva: adding the #include is enough for the
generated lists, BUT there's an ordering cycle in `bin/fabrica sanare`:
the silva tool source lists (`silva/*_fontes_generata.sh`) are only
regenerated after `amalgama_silva`, and amalgamare's censor-corpus check
needs a censor that already links the new lib (0/13 otherwise). Broke it
by hand with `./tools/silva_fontes_generare.sh`, then sanare + build were
green.
