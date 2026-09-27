# silva_frons — worklog

## 2026-09-25 — born: promoted out of the shim (silva-migratio-plan T6b)

**What moved.** The production half of `materia/instrumenta/shim_c89.c`
(~1,050 lines: the token tail `SilvaFronsCauda`, conversion of registry,
tokens, values and nodes, the origin hooks, the arbor frontend hooks and
the `FRONS_C89` table) became `silva/fontes/silva_frons.{h,c}`. Ported
verbatim by script, then three deliberate changes:
- the global `SHIM` became the opaque context `SilvaFrons`, passed
  explicitly through conversion and through the hooks' `datum`
  (`uncus.datum = frons`; only `_extentum_quaerere` needs it);
- `FRONS_C89` initialises all NINE fields (`nodum_ornare = NIHIL`): the
  shim had compiled under narrowed flags precisely because this table
  missed a field — silva/fontes compiles under the house flags;
- `_radix_silvae` (defined, never called) was not promoted.
The shim is now a 372-line driver (was 1,462) and compiles under the
HOUSE flags too — its narrowed-flag remission lost its reason and was
removed.

**API** (approved by Fran, two small additions): `creare`,
`valorem_convertere`, `nodum_convertere`, `scripturam_parare`,
`arborem_parare`, `lexemata_numerus`, plus `silva_frons_causa` (the shim
reported the lexicon's refusal by name) and `silva_frons_nodus_radicis`
(the commit's root value is a VALUE; shim and writer need the node).

**Traps hit.** `registrum` is a latina.h macro (`register`) — the
context field is `tabularium`. The porting script missed calls whose `(`
ended a line; the compiler named all three.

**What guards the macro attribute.** Plant: drop the `macro` attribute
from written origins → shim `-stml` STML 383/401 (18 dispar), exit 1.
But the double round trip and the tree comparator stayed green —
writer and reader agree with each other either way. Only the TEXT
comparison against silva sees it. When the shim retires (T13), the
oracle's `stml` column (whole-file writer, pinned silva) is what
guards such attributes.

**Build.** silva/fontes now includes materia headers, so every site
that compiles `silva/fontes/*.c` (silva runner, 20 tools, silex) gained
`-I materia/fontes` and a header guard over it; `lustrum.sh` has no
header guard at all (pre-existing, noted). The amalgam EXCLUDES
`silva_frons.c` and `silva_lexicon_c89.c` (`fontes_politica.sh`
`radices()`) until the seal: `amalgamare.sh` regenerated it
byte-identical. The tools' generated dependency lists did not change.

**The lexicon** moved to `silva/fontes/silva_lexicon_c89.{h,c}`
(generator `silva/instrumenta/lexicon_c89_generare.sh`; content
identical but for comments; symbols `LEXICON_C89*` unchanged). materia's
lexicon probatio compiles it through a TEST-ONLY include list.

**Numbers.** Silva 54/54; shim `-stml` 402 + 401/401/401 (the corpus is
`silva/fontes/*.c` among others — it gained exactly these two files,
both pass); oracle clean; materia 14/14; all 20 tools build and carry
`silva_frons_creare`.

## 2026-09-25 — T10b: hooks over silva's own tokens; the read-side extent bug

**Second hook set, no conversion.** `silva_frons_arborem_silvae_parare`
builds a MateriaArborConsilium whose origin hook, frontend table and
token form all work on silva's OWN tail (SilvaOrigo, standard,
continuations) through the accessors. SilvaToken is a MateriaToken
(T7b) and SilvaNodus a MateriaNodus (T8), so there is nothing to
convert. The table lives inside SilvaFrons (not static) because the
hooks need a context: extents READ from a document are registered in
`frons->extenta_lecta`, so a subtree read no longer needs an expansion
(silva's old reader refused 101 nodes with "extentum sine expansione").
The old converted-tail hooks stay until T10c retires conversion.

Ported semantics, deliberately identical to silva_arbor.c: extent lookup
by identity only; `longitudo = valor` on every read token (the old reader
created each token with silva_token_ex_fonte; materia creates trivia with
no hook, so `perficere` walks the tree, trivia, origin chains and read
extents; `silva_frons_longitudines_figere` is public for T10c's laminas);
the def-site is a synthetic token built from the def-f/l/c reference.

**The bug the all-nodes shim found (fixed in the old hooks too).** When
tokens were read back, the extent landed in the DERIVED token's tail, but
the extent hook searched `frons->lexemata`, which only CONVERSION fills.
So the extent of a function-like invocation was never found on the read
side, the cursor never walked `(3)\n`, and the token after the invocation
lost `initium_lineae` (its byte offsets were off too; the structural
comparator doesn't look at them). 20 nodes in lib/arbor2_glr_tabula.c,
invisible while the shim checked only each file's FIRST node. Fix: the
reader also records the extent on the invocation's own tail, and the hook
checks the radix first. Pinned by the shim's inline case "invocatio ante
lineam" (red without the fix).

## 2026-09-25 — T10c step 2: conversion retired, shim retired

With silva's node writer/reader (T10b) and parsura writer/reader (T10c
step 1) on materia through the hooks over silva's OWN tail, nothing used
the conversion any more except the shim, which would now compare silva
with itself. Removed: SilvaFronsCauda and the whole converted-tail hook
set (FRONS_C89, `_token_convertere`, `silva_frons_valorem/nodum_
convertere`, `_scripturam_parare`, `_arborem_parare`, `_lexemata_numerus`,
`_nodus_radicis`, the old origin hooks). Kept: the context, the hooks
over silva's tail, and `_extentum_ornare` (which they reuse).
1,205 lines at T6b → 991, all of it now production code.

Also retired: `materia/instrumenta/shim_c89.c`, `materia/shim_probare.sh`,
gate `materia-shim` (pythonica PORTAE) and its 'suitae probationum' row.
Its job passes to the oracle gate (pinned silva vs live: bytes, STML,
read-back, comparator), whose coverage was already a superset for silva.
The shim's last useful act was the per-node sweep in T10b (found the
read-side extent bug). Script comments that cited its object glob now
cite oraculum_silvae_struere.sh, which uses the same exclusion list.

## 2026-09-25 — sedes efficax = root of the origin chain (silva-migratio T14)

`_silvae_sedes_quaerere` returned an expanded token's OWN fields — its
definition site, i.e. bytes of a macro body in latina.h. materia's
contract for `sedes_quaerere` says "effective position for node
queries (extent, purity, geometry, comment)", and silva's own
`silva_nodus_extensionem` has always used the ROOT (stratum-0
invocation). The consequence surfaced the first time materia's
diagnostics walked a C89 tree: `materia_tractus_conferre` adopts the
file of the FIRST token it sees, so a node beginning with `interior`
(→ static) got an extent inside latina.h and every C89 diagnostic was
printed at a latina line (malum.c "8:25" in a 5-line file).

Now: FONS tokens report themselves; derived tokens report their root's
position with `est_fons = FALSUM` (purity and materia's byte-order
check still skip them); unrecoverable roots (paste, stringification,
API) report `byte_offset -1` and drop out of extents — "synthetica
omissa", never invented. The arbor writer only reads `est_fons` from
the sedes (coordinates come from the token itself), so STML is
unchanged — the oracle confirms.

The root walk became a const core `_silvae_radix_derivati` (starts at
an expanded token's origin, so every pointer it follows is non-const
and no cast-qual cast is needed); `_silvae_radix_quaerere` and the
sedes hook both call it. New accessor `silva_frons_uncus()` exposes the
hook to materia consumers other than the tree writer. Planted: sedes
back to def-site → the diagnostica gate's line assertion goes red (the
error node's extent then lands in latina's file and the
principal-file filter drops it: a real error VANISHES).

## 2026-09-25 — one printer for syntax deaths (silva-migratio T19b-1; …W87Q)

`silva_mortes_scribere(piscina, via, parsura, tabularium, fons, mensura,
machina, mappatio, datum, &linea_prima)` moved here from examen.c
(T15b): rows `via:line:col: [violatio] causa` + excerpt (hic coepit /
hic exspectatur), TSV in machine mode, summary for unlocated deaths.
examen, legatus (MCP legati + LSP) and briar call it; nobody copies
the format. The mapping callback relabels EVERY printed line (rows and
excerpt labels — lib/excerptum prints the caller's `linea`, it does not
recompute), which is what briar needs (silva line → .thistle line).
`linea_prima` hands back the first located death's mapped line so
briar can stop reporting the ERROR node's start (= unit start = region
start: the real cause of lapide bugs/001).

Public in the hand-written silva.h (SilvaChorda/SilvaPiscina/unsigned
int spelling); `SilvaLineaMappatio` added to the amalgamator's CADENDA
(silva.h owns the typedef). excerptum + utf8 became amalgam bodies:
fontes_politica.sh harvest bases (EXCLUDENDA_EXCERPTI/_UTF8 — unused
functions of a vendored lib become unused statics), manifest + harvest
regenerated; silva's test runner has a HAND list of lib deps
(compile_probationes.sh RADIX_FONTES) — excerptum, utf8 added there.

Live demonstration of …XHAW3H mid-task: I added `linea_prima` to
silva.h AFTER regenerating; briar compiled against the 9-arg header and
linked the 8-arg silva.c — the out-param was silently never written
(briar kept reporting the region start). `tools/amalgama_caput.sh
-comparare` said rc 3 "amalgama dimidiata"; the generata gate would have
refused the commit.

## 2026-09-25 — death messages say what, and why (silva-migratio T19b-2; lapide bugs/001 asks 2–3)

Built in `silva_mortes_diagnostica`, so examen, tools/diagnostica, legati
and briar all gain at once:
- **found token** in the author's spelling (radix of the origin chain):
  `lexema 'nomen' quod grammatica hic non accipit` — not 'typedef'.
- **latina.h hint**: the dead token is a macro expansion whose def-site
  lives in a source named `latina.h` (embedded copy or include/latina.h)
  → `- 'nomen' macrum latina.h est ('typedef'): nomen aliud elige`
  (numerals too: 'C' → '100'). GATED on the parser having EXPECTED AN
  IDENTIFIER: the first version flagged `integer` in `integer b = II`
  after a missing ';' — a correctly used type; there the state wants
  operators/';', not a name. Regression test pins the gate.
- **missing ';'**: the driver now stores, on each SYNTAXIS death, the
  token KINDS the first frontier state accepted with a normal action
  (`SilvaMors.exspectata`; grammar-neutral — the frontend checks
  SILVA_LEX_SEMICOLON without knowing the table). If ';' was expected,
  the dead token is first on its line and the previous token sits on an
  earlier line → `- ';' fortasse deest post ')'` + a third excerpt mark
  under the previous token ("hic ';' fortasse deest", where clang points).
  Not after ';' or '{' (found in the examen fixtures: "deest post ';'").
  One frontier state only (GLR reports the first) — a named approximation.
- No line numbers in message text (briar remaps lines; the excerpt
  carries them). Row buffer grew to M (messages can exceed CCLVI, and an
  overlong row was SKIPPED silently before). Excerpt marks now sorted
  (`_sedes_ordinare`), up to three.
Named costs: computus gold — base64/color only (the corpus files with
deaths): allocations +24/+25, usus +~2 KB (the exspectata lists);
lexemes and errors unchanged. Pins updated deliberately (examen_vectis
④ ×2, diagnostica_fumus XXII) — now also assert the found token.

## 2026-09-26 — closer hints, and a false ';' removed (T19b-2 follow-up)

Checked against lapide's own bugs/001 round-6 list (seven cases rebuilt
as scratch .thistle, run through the installed briar). Six were right;
**unclosed '{'** was wrong: the parser dies at EOF, EOF counts as "first
on its line", the previous token is '}' (allowed on purpose for
`struct s {…}`), and the dead state happens to accept ';' → "';'
fortasse deest" under the '}'. Misleading, worse than no hint.
Now: the ';' heuristic never fires at EOF; `_clausura_deest` says what
is true — EOF + '}' accepted → "'}' fortasse deest" (else ')'); dead
';' + ')' accepted → "')' fortasse deest" (clang's "expected ')'").
EOF deaths read "finis textus quem grammatica hic non accipit" (finis is
masculine — 'quem', not 'quod'; the first draft had it wrong). Tests:
both closer hints, and neither may carry the ';' hint.
Lapide's list now: missing ';' ✓, reserved C ✓, unclosed '{' ✓, nomen/
casus ✓, junk '@' ✓ (located, no hint), missing ')' ✓, two missing ';'
→ first reported, second parked (…M4X).
