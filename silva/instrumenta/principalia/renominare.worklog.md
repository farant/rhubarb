# renominare.worklog

## 2026-07-31 — birth day: all four stages in one arc (re-entry note)

Whole tool built in one day (park 01KYWYQMJ7, board trail has
per-stage notes; silva/phase-log.md has INTENTIO/RELATIO per
stage). Commits: f210cb0 (v0 file-scope) → 868bb79 (macro bodies
token-true + promotion) → 4677981 (locals -intra) → 680ad60
(members -membrum) → a116efa (prototype ripple) → 6fe7581
(file-scope streaming). Gate: `./silva/renominare.sh -proba` = 20
golden cases over fixtures in probationes/fixa/renominationis/
(goldens = aureum_*.tsv, applied results pinned under
post_scriptionem/). REGENERATE goldens when fixtures change — the
scriptio cases diff whole files.

**Architecture in one paragraph.** Per file: full examen-recipe
analysis (lexicon composition, capita praebita, bis-analysis).
Classification walk over the committed tree fills a token→class
map (folium → symbolum_nodi binding; ACCESSUS → typed base;
declarators → registration pointer identity, leftovers ALIENA;
labels/members/typedef-uses by node genus). Then the LEX
RATIONARII pass: every stratum-0 identifier token spelling vetus
must be in the map, in a directive line, or non-FONS (macro
expansion → testimonium evidence) — anything unexplained is a
REFUSIO culpa, never a silent skip. Splices are (via, offset)
rows, dedup'd cross-TU, applied back-to-front with a byte guard;
post-apply re-analysis checks survivor arithmetic (survivors ==
aliena rows + in-directiva rows for that file, EXACTLY).

**Traps for future editors:**
- The survivor arithmetic is load-bearing and subtle: only ALIENA
  rows and manualis rows with nota exactly "in directiva
  praeprocessoris" count as expected survivors. Adding a new row
  class that carries an offset in an edited file WILL break it —
  think about whether the row's token survives in the lexeme
  stream before adding.
- Directive-line tokens are NOT in parsura->lexemata; macro
  bodies live in SilvaMacroDef.corpus (real tokens), untaken
  branches in SilvaRamus.lexemata_cruda. The expander keeps
  everything (underestimation law, 5th firing).
- Entity identity differs by mode: file-scope = name+linkage with
  optimistic-extern streaming (static found mid-stream → refusal;
  lone static needs -via); locals = symbol POINTER (never cross
  TU); members = per-TU tag pointer resolved by NAME (typedef
  first, then file-scope tag).
- All modes STREAM (destroy per-file arenas after collect).
  Later pointer comparisons (symbolum_localis, tag_electus)
  compare VALUES against dangling pointers — legal, never deref.
  an->errores carries numerus_errorum past the destroy.
- Macro-body promotion evidence attaches to the FIRST origo step
  (the macro whose body carried the token), aggregated globally
  across TUs; SEDES counts as entity evidence.
- The wrapper derives candidates: plain → nexus rows for vetus;
  -intra → nexus sedes of the FUNCTION; -membrum → reverse
  include closure (awk BFS over inclusiones.tsv) + word grep
  prefilter (safe by construction).

**Deliberate non-features:** fn-pointer/typedef doc-param rename
(no binding identity); MCP tool (multi-second plans would freeze
the single-threaded resident — doctrine pointer in legatus.c
instead); comments never auto-edited; git is the undo (no
transaction machinery; -scribere refuses on dirty affected
files).

**Open doors:** macro renames themselves (directive splices —
sedes+invocations already nexus rows); struct TAG names (tags
aren't registrations — currently refuses "symbolum non
inventum"); generated-file gate in the plan (amalgams get spliced
rather than named-for-regen); accessus rows in nexus.
