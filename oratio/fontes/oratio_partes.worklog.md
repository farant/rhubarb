# oratio_partes worklog

## 2026-10-21 — where annotation memory and time go (measured, nothing changed yet)

Trigger: probatio_oratio_partes V (corpus) hit the credo 256 MiB section
ceiling. Probes are in build/sonda_partes/ (gitignored; rebuild with
`./tools/sonda.sh -domus build/sonda_partes/<x>.c`).

**Baseline, cicero.txt** (92 KB, 13,369 words, 141,985 analyses, house -O2):
791 ms. The pool grows +126 MiB over the 12 MiB parse, about 9.7 KB and
170 allocations per word. Process peak RSS goes 65 -> 300 MiB.

**Memory census of the annotated tree** (sonda_census.c), new bytes only:
- loci: 1.71M x 24 B = 39 MiB. Analysis nodes carry ~10-15 loci, most of
  them empty accidence slots.
- derived tokens: 441K x 64 B = 27 MiB, plus their text 10.5 MiB stored
  THREE times (la_describere copy -> `_copia` in _derivatum_ponere ->
  copy inside oratio_lexema_derivatum).
- lists: 41K new, 53% slack (first Xar segment holds 4, most umbra lists
  hold 1) = 10 MiB.
- nodes: 171K x 24 B = 4 MiB.
- Only 8,590 DISTINCT derived strings (246 KiB) and 136,660 distinct
  (word, value) pairs. Most tokens are duplicates within one word's
  analyses: every analysis of `qui` carries its own `qui` lemma token.
- analysis-pronominis = 73K of 142K analyses.

**Time** (`sample`): ~70% is `oratio_vocabularium_la_lemma`, which, for
EACH analysis, walks the whole 1,797-entry inflection table through
xar_obtinere/xar_numerus calls (those two accessors alone are 53% of the
samples) to find the first matching ending.

**Experiments** (applied locally, measured, then reverted):
1. Mark/reset scratch around each oratio_partes_vocabulum_annotare call
   (`piscina_notare` / `piscina_reficere` in _annotare): peak 300 -> 228
   MiB. Census of the tree identical. Scratch was accumulating every
   word's lookups for the whole document.
2. Drop `_copia` in _derivatum_ponere (oratio_lexema_derivatum copies
   anyway) and give the describe calls in vocabulum_annotare `scratch`
   instead of `piscina`, since descriptions only feed nodum_struere,
   which copies: pool +126 -> +109 MiB, allocations 2.26M -> 1.57M,
   census identical. Should be checked under PISCINA_VENENUM before
   committing.
3. Lemma index: at load, per effective part of speech, an array of
   pointers to the `clavis == I` inflections in table order (PACK bucket
   = PACK + PRON, matching _congruit). The lemma loop scans only its
   bucket. A hash of the lemma of ALL 39,335 stems is identical
   (20d83d95). Full lemma pass 55 -> 7 ms; cicero annotation 791 -> 272
   ms (2.9x). Holds because a lemma depends only on the stem's class,
   and _congruit rejects on part of speech first.

**Not yet tried:**
4. Share derived tokens within one word: 441K -> 137K tokens (~-26 MiB).
   Need to confirm nothing mutates a derived token or relies on its
   identity.
5. MateriaValor 24 -> 16 B: pack the list length beside the genus.
   That is ~40 direct `datum.lista` uses outside the silva amalgam, and
   it touches every materia client (park 01M5EGAWD5NT504BX9ZCR6JC37).

## 2026-10-21 (later) — options 1-4 applied (Fran approved)

All four are in, with no public signature changed:
- `oratio_partes_nodum_struere` and `oratio_partes_vocabulum_annotare`
  keep their old copying behaviour through `derivata = NIHIL`.
- one public function added, `oratio_lexema_derivatum_possessum`: a
  derived token whose value the caller already owns.

What each change does:
- **1. scratch reset:** mark/reset scratch around each word in `_annotare`.
- **2. extra copies removed:** no `_copia` before `oratio_lexema_derivatum`;
  describe calls write into scratch.
- **3. lemma index:** `_primas_indicare` at load. Per effective part of
  speech, pointers to the `clavis == I` inflections in table order; the
  lemma loop scans only its bucket, with a bounds guard on `st->pars`.
- **4. interning + shared tokens:** `annotare_dialecto` builds an
  `OratioDerivata`: an interner in the TREE's piscina, plus a cache of
  up to 32 (interned value -> token) entries that resets whenever the
  origin token changes. Every analysis of one word shares its lemma /
  nativum / sensus tokens; all derived text is stored once per document.
  Enclitic paths still copy (rare).

**Safety check for 4:** nothing in materia/oratio writes a MateriaToken
field after `materia_token_creare` (grep for field writes: the hits are
other structs). Tokens are only compared against NIHIL. silva_glr's
pointer comparison is on silva's own tokens.

**Verification, cicero:** the census of the annotated tree is identical to
HEAD:
- 187,497 nodes, 1,853,352 loci, 441,462 derived-token occurrences;
- hash of all derived text, by occurrence, 86196b7a;
- the same under ASan + `-DPISCINA_VENENUM=1`, so no read of reset
  scratch survives.

Lemmas of all 39,335 stems: hash 20d83d95, identical. oratio suite 19/19.

| cicero (-O2) | HEAD | now |
|---|---|---|
| annotation pool | +126 MiB | +81 MiB |
| allocations | 2.26M | 858K |
| peak RSS | 300 MiB | 187 MiB |
| time | 791 ms | 277 ms |

**Trap hit during verification:** sonda caches objects by mtime. Adding
`-DPISCINA_VENENUM=1` to its generated struere_sonda.sh rebuilds nothing
until `build/sonda/<T>/obiecta` is deleted. The first "poisoned" run was
not poisoned at all. Check the `[obiectum]` count in struere.log.

**Side finding (not caused by this):** probatio_oratio_stml's corpus
section is at 988 MiB alone (HEAD identical) against its 1024 MiB
`gravis` ceiling. In one parallel run it measured 1024.5 and failed.
That is the STML-projection weight (park 01M5EGAWD5NT504BX9ZCR6JC37),
and the strongest case for shrinking MateriaValor (#5).

**After 1-4, the test's 357 MiB was not annotation.** probatio_oratio_partes V
round-trips hilarius' annotated tree through STML. 11.5 KB of Latin
becomes 9.3 MB of STML (810 B per source byte). `materia_arbor_scribere_nodum`
builds the full StmlNodus projection first: +116 MiB, about 12.7 B per
output byte. Reading it back adds +130 MiB. That is the projection
weight again (park …JC37), not oratio_partes. The suite now gives the
round trip its own `gravis` section (V b, 258 MiB), and corpus annotation
V is at 89 MiB under the normal 256 MiB ceiling, with no signum.
