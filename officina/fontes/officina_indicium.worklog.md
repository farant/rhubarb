# officina_indicium worklog

## 2026-07-11 — natus (M3 chunk ③): the format module, one day

Scriptor + lector, ~1200 lines, probatio 51/51. The circuit: parse
fixture → demissio → colligere (parsura alive) → conexio →
machinula_creare (freeze) → scribere → REOPEN → query. Design
settled in the INTENTIO after recon proved the ENTIRE surface
needed is already public in the silva amalgam (SilvaOrigo union,
SemanticaSymbolum.typus, TypusC89 walkable, SilvaNodus.pater) —
zero silva-side changes.

**Design deltas vs spec §V (both recorded in INTENTIO):**
- INDEX_CURSUS section OMITTED v0: every variable-length list has
  exactly one consumer section, so runs live inline (typed
  sections with primus/numerus refs). Shared pool = named future
  when two consumers appear.
- X-macro schema DROPPED for the house pattern: the amalgam TU
  compiles against hand-written vanilla officina.h, which cannot
  share a fontes X-macro. Explicit enum (fontes .h + vanilla .h,
  hand-mirrored like every other officina type) + explicit
  magnitudines array in the .c + NUMERIC size asserts in the
  probatio (Caput 16, Sectio 8, Via 16, Modulus 4, Functio 32,
  Linea 16, Situs 8, Variabile 16) — ABI drift breaks the suite.

**THE FIND: MedullaFunctio.origo was NEVER assigned.** A designed
provenance slot (medulla.h, "origo SEMPER fertur"), empty since
M1 — demissio set instruction origo but never the functio's. All
6 first-run probatio failures (every semantica variable match)
had this one root cause: the ancestry test walked declarans→pater
toward a NIHIL target. One-line fix in _functionem (demissio.c:
d->functio->origo = nodus) — which also makes _lineam_colligere's
functio-level entry real. First-run score: 45/51 with the whole
pipeline (stacked macro entries, retro, determinism, magic
rejection, walk-pact pin) correct on first execution.

**Latina firings (two new, both header-side struct FIELDS):**
`magnitudo` and `registrum` as field names in officina_indicium.h
— caught INSTANTLY by clangd push + censor suspecta before any
compile. Plus the classic `chorda nomen` local in the probatio.
Field names are identifiers too; the reserved list applies.

**Amalgamator lessons (two new gate catches):**
1. Duplicate statics across the concatenated TU: _ch_de_silva /
   _chordae_aequales / _viam_internare / _lexema_primum collided
   with demissio/medulla internals → _ind_ prefix. The
   deliberate _lexema_primum duplicate (demissio keeps its own
   internal) is now honestly named.
2. **A field named `chorda` is forbidden in amalgamated sources**:
   the amalgamator's type rename (chorda → OfficinaChorda) is
   token-based and rewrote the FIELD access. Field namespace
   shares the token namespace with renamed types. IndiciumVia.
   chorda → .titulus.

**Variable table (collection-time, per interview DECISUS):**
registra names × semantica symbols, ancestry via pater, first
match wins. Types render RESOLVED ("unsigned int" not "i32" —
C89 typedefs are pure aliases, TypusC89 has no alias arm; honest,
documented). Params get medulla tags from MedullaParametrum;
locals via _tag_de_typo (mirrors demissio's mapping — small
deliberate duplicate). Shadowing (x_37): unmatched → IGNOTUS row
(the interview-blessed honest gap; lowering-time recording = the
named upgrade).

**Walk-pact pin lives twice:** probatio (indicium count ==
machinula_numerus_instructionum_planarum) and hospes ("indicium
pactum ambulationis") — collection walk and freeze walk are now
mechanically bound. Order drift is covered by the sweep (wrong
branch targets = instant corpus carnage); count drift breaks
these asserts.

Rough edges (at the reach):
- retro lookup is linear over VIAE before the binary search
  (fine at ~800 viae; sorted-viae index = pull).
- IndiciumLector loads whole file (fread+cast; mmap = POSIX pull).
- _internare appends bytes one xar_addere at a time (collection-
  time only; addere_multos if it ever shows in a profile).

## 2026-07-11 — M3 chunk ⑤: THE FORMAL BAR (probatio_officina_vindex)

probatio_officina_vindex.c = the M3 seal: breakpoint/step/inspect
probatio_piscina THROUGH THE FILE ONLY. Loader = interpretare's
path + indicium collection in the collect-before-destroy window
(C12 discipline: the debugger's loader bakes). Scoped world
{piscina, chorda, chorda_aedificator, credo} + the suite TU; write
→ REOPEN → all queries via lector. Four phases, 69/69:
1. RETRO punctum at probatio_piscina.c:37 (a CREDO line) →
   pergere → PAUSA at the exact site; stacked macro entries at the
   pause (root :37 prof 0 + CREDO_AEQUALIS_I32 frame — the bar's
   "macro-aware" clause demonstrated on a real test); "usus" found
   BY SOURCE NAME in VARIABILIA → registrum_legere == 0; walk pact
   re-verified on main.
2. Step-over-line (root changes ∧ depth ≤ entry) → lands on :39.
3. Function-name punctum CROSS-MODULE (piscina_summa_usus, module
   asserted ≠ suite module) → PAUSA at depth 2 (the :54 call),
   bottom frame = main; FINI → depth 1, back in main.
4. Tollere both → pergere → BENE codex 0; >40k instructions; the
   interpreted suite's own credo output flowed to stdout and
   reported 60/60 OMNIA PRAETERIERUNT.

TWO REAL FINDS by the bar (both fixed):
1. **machinula_gradus from a paused VM returned FALSUM** — the
   resume dance (restitue-grade-repone + currens) lived only in
   pergere, but STEPPING from a pause is the debugger's bread.
   Moved the resume seam INTO gradus (recursion for the
   punctum-step, then re-patch); pergere collapsed to
   `dum (gradus)`. One resume site, both consumers.
2. **No public accessor resolved IndiciumLinea.via → chorda** (it
   indexes VIAE, not the string table; the debug print showed
   'unsigned long' — a typus string — where a path belonged).
   indicium_via_chorda born (fontes + vanilla + hospes row).
   Chunk ③'s probatio never resolved a forward-table via — the
   BAR was the first real consumer of that field. Bars find what
   validation doesn't.

Numbers: bar runtime ~7s (world build ~6s: 800-header praebere +
5 TU demission; execution ~1s). Loader is the THIRD twin
(interpretare/cursor/bar) — shared-loader refactor pull now LIVE
at three copies, named for vindex chunk ⑥ (the TUI needs the same
loader; extract THEN, with a real fourth consumer on the table).
