# silva_scribere worklog

## 2026-09-25 — T9: the byte writer runs on materia_scribere (silva-migratio)

silva_scribere.c went from 856 to 538 lines. The walk, token emission,
invocation dedup and reinsert merging are materia_scribere's (which was
ported FROM this file). What stays is C89:

- **Hooks over silva's own tokens.** No conversion: since T7b a
  SilvaToken IS a MateriaToken and since T8 a node is a MateriaNodus, so
  the hooks read silva's tail through the accessors. `_radix_unci` =
  the old `_radix_probata` plus the two refusals, with their causa strings
  kept verbatim ("lexema expansum sine contextu expansionis",
  "origo pasta/chorda/api ... (deferral nominatum)"). `_extentum_unci` =
  lookup by identity, THEN containment (`_extentum_continens`, for
  stringified arguments whose radix sits inside the invocation).
  `_valorem_unci` = new `silva_lexema_valorem_emittere_in` (the value
  half of silva_lexema_emittere_in, so there is still ONE
  continuation-reinsertion implementation in silva).
- **Whole-file extras (fontem):** directive lines, untaken branches and
  empty invocations are gathered as MateriaReinserendum, filtered by
  source file and sorted with materia_reinserenda_comparare.

Two gaps this measurement found in materia's writer, and what was done:

1. **Trailing reinserts were unreachable.** materia merged reinserts only
   BEFORE a tree token; one after the last token (a header guard's
   `#endif`) had nothing to merge it and was silently dropped. silva's
   fontem did `_reinserenda_fundere(OMNIA)` after the walk. materia now
   has `consilium.reinserenda_finire` (default FALSUM, so subtrees and
   the other clients are unchanged); test in probatio_materia_scribere
   pins both off (dropped) and on (present). The shim never saw this
   because it only ever compared `silva_scribere_valorem`, never
   `_fontem`. The oracle is the first judge of materia on fontem.
2. **EOF ordering.** silva flushes ALL reinserts, THEN writes EOF.
   Putting EOF in a list after the root would merge only reinserts with
   offset < EOF offset before it; with fons_index -1 and several files,
   a longer included file's reinserts would land after EOF. So fontem
   writes the tree (with the tail flushed), then appends EOF with
   silva_lexema_emittere_in: one extra copy, exact order.

`emissum_usque` is now `offset + valor.mensura` (materia) where silva
had `offset + longitudo`. They differ only for tokens holding a line
continuation, and the dedup test compares a LATER radix offset against
it, which is always past both ends. The oracle over 470 files agrees.

Known remaining gap: silva_frons's `_extentum_quaerere` hook (for
CONVERTED tokens, the shim and STML paths) has no containment fallback.
No corpus case reaches it (shim 401 identical); it dies with conversion
at T10.
