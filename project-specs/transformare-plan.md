# transformare - plan

Spec: `project-specs/transformare-spec.md` (DECIDED 2026-10-24, N1-N4;
ledger decretum …KNW98). Inline execution, one task per checkpoint,
Fran approves each.

*Grounding (2026-10-26, read before planning):*
- Nodes are materia nodes (`#define SilvaNodus MateriaNodus`): a
  registered kind + slots; calls are kind `vocatio` (grammar table).
- `silva_nodus_extensionem(nodus, fons, &min, &max)` = the node's byte
  range in the ORIGINAL file, through macro origins;
  `silva_nodus_geometria_fida` = tokens map 1:1 onto source (Latin
  keyword macros si->if are 1:1 and fine; multi-token expansions are
  not) -> the rewrite-safety test.
- The tree is POST-expansion: a pattern written with `si`/`ZEPHYRUM`
  and a file spelling `if`/`0` compare equal on the tree, while every
  captured hole is copied from the file's own bytes. So the census
  groups spellings that mean the same thing (refines spec §III, which
  assumed literal tokens).
- STML raw line capture (`<si! (>...`, as `<verbum! (>` in
  aedificatio.stml) holds patterns with `<` and `"` verbatim - resolves
  the spec's quoting AUDIENDA.

## Tasks

**T1 - fragment parsing (silva library).**
`silva_fragmentum_parsare(piscina, textus, genus)` with genus
EXPRESSIO or SENTENTIA: `$name` -> reserved identifier `__trf_name`,
`$...name` -> `__trfv_name` (never reach a real file), wrapped as
`void __trf(void) { (frag); }` / `{ frag }`, parsed with the house
context (latina.h), returns the expression/statement node + the hole
table. Test: fragments with holes, Latin keywords, string literals;
malformed fragment = named error.

**T2 - matching (silva library).**
`silva_forma_congruit(forma, nodus, capturae)`: kind-by-kind tree
comparison on token VALUES (trivia-blind); a hole matches any
expression subtree and records it; a repeated hole requires equal
subtrees (new trivia-blind subtree equality, built on
silva_arbor_aequalitas); `$...` matches the rest of an argument list.
Test: matches, non-matches, repeated-hole equality, `$...` tails,
`si`/`if` and `ZEPHYRUM`/`0` equivalence.

**T3 - census (tool).**
`silva/instrumenta/principalia/transformare.c`, `./silva/transformare.sh
-census FUNCTIO viae...`: every call to FUNCTIO whose callee token's
root lies in the file itself (else listed as `in macro`); consumer =
walk up through parentheses to the first node that uses the value;
shape = the consumer's tokens with the call's arguments as $1..$n;
rows grouped and counted with locations; `-machina` TSV. Test: a
fixture file with each consumer kind.

**T4 - rules + plan.**
Rules STML `<transformatio functio="remove" codex="SCRIPTURA_PRAETERITA">
<regula><si! (>PATTERN <fac! (>REPLACEMENT</regula>`; per site the first
rule whose pattern matches the consumer (or an ancestor up to the
statement); edit = the matched node's byte range -> replacement text
with each hole filled by the captured subtree's ORIGINAL bytes;
refused sites (no rule, not geometria_fida) listed by shape. Plan
prints unified diffs, writes nothing. Test: fixture + rules ->
expected diff; refused shapes named.

**T5 - apply with guard.**
`-scribere`: per file in memory - apply edits (back to front),
re-parse (no new errors), re-run examen (the named code loses exactly
the rewritten sites, no other code grows, no REICE), `clang
-fsyntax-only` with house flags; any failure = file untouched + named
reason; then formator on the touched lines only. Test: a rule
producing a wrong arity is caught; a good rule writes byte-exact
output. Plants for each guard.

**T6 - wrapper + first rules library.**
pythonica `silva.census(functio, viae)` / `silva.transformare(regulae,
viae, scribere=False)` (argv in, TSV out); rules for the fabrica-8 sweep
in `silva/transformationes/`: remove/unlink -> filum_delere, rename ->
filum_movere, mkdir -> filum_directorium_creare*, rmdir -> filum_delere,
chmod -> filum_modum_ponere, write-mode fopen -> lectiones_fopen; code
97's message names its rules file (spec N4). Then the sweep itself
(fabrica-8, area by area) is ordinary use of the tool.

*T3 done (2026-10-26):* statement-level calls are the "expression
statement" consumer (`sententia-expressionis|remove ( $1 ) ;`). Also
decided while building: `(vacuum)` casts are see-through like
parentheses (the void cast discards the value; the row is the
statement); an initializer's consumer is the whole declaration; bodies
of si/dum/per/fac/commutatio print as `...`; rows group by the
post-expansion key and show the first site's own spelling, with
`[aliter scripti: n]` when spellings differ; calls whose parentheses
come from a macro body OR a macro argument are `in macro`. Census by
PATTERN (spec III "functio or pattern") waits for T4: the fopen census
mixes read and write modes, a pattern `fopen($p, "w")` separates them.

*T4 done (2026-10-26):* rules are tried in file order, and each rule
from the CALL node up to the enclosing statement (nearest wins within a
rule; first matching rule wins). Rules file = `<transformatio
functio=..>` with `<regula>` holding raw-line `<si! (>` and `<fac! (>`
(quotes and `<` verbatim). Safety = parts walk: between consecutive
tokens/captures of the matched node the source holds only whitespace
(comment/directive -> `trivia`, other text -> `non fida`, e.g.
`remove(VIA(v))`). Added: `recursio` refusal (the substitution would
call the function containing the site - lib/filum.c's own remove),
`[numquam congruit]` for unused rules, bare-call SENTENTIA patterns
work (X8 canonical reading). Exit 0 = every site covered, 1 = some
refused. Plan output = unified diff (stdout, `patch -p1` applies) +
summary and refusals by shape and cause (stderr).

*T5 done (2026-10-26):* `-scribere` (wrapper passes the house clang
flags after `--`). The guard moved out of emendare.c into
`silva/instrumenta/silva_emendatio.{h,c}` (emendare now calls it).
Per file: examen on the new text (rules `codex="97"` - numeric, no
public name table - loses exactly the applied edits, nothing else grows,
no REICE), clang -fsyntax-only on the ORIGINAL (not compiling = not
verifiable, refused) then on the new text, then `filum_scribere`.
Formatting deliberately NOT done here: the formator has no line-range
mode and commissio formats whole files at commit anyway.

*T6 done (2026-10-26):* pythonica `silva.census(functio, viae)` ->
Census(situs, fractae) and `silva.transformare(regulae, viae,
scribere=False)` -> Transformatio(rescriptiones, fractae, nuntii, rc).
Rules library `silva/transformationes/` = remove, unlink, rmdir,
rename, chmod, mkdir (mode 0755 only), fopen (write-mode literals ->
lectiones_fopen, call level), each header naming where behaviour
differs. Tool: GENERATUM files never rewritten; `<inclusio>` adds the
replacement's header. code 97's message names the rules file
(N4). Dry run over the whole tree (minus vendor/archivum/reference),
2026-10-26: 341 sites rewritable - remove 133/135, unlink 33/34, rename
6/9, mkdir 34/40, rmdir 20/23, chmod 13/16, fopen 102 write-mode (194
fopen refusals are mostly read mode = code 96, not this sweep). Manual:
officina machinula (implements remove/rename/... for its interpreter,
int convention), CREDO_* macro arguments, filum's own wrappers
(recursio), mkdir modes other than 0755.

**Next: the sweep (fabrica-8), area by area**, each a normal commit:
`./silva/transformare.sh -regulae silva/transformationes/X.stml AREA
-scribere`, then the manual leftovers, then code 97 -> VIOLATIO with a
plant.

## Open (decide when reached)

- **Who is exempt from code 97 when it becomes VIOLATIO?** (found in
  sweep step 1, 2026-10-26) Code 97 has NO exemption today: it fires
  36 times inside lib/filum.c itself and in lib/credo.c. Both were
  left out of the sweep by hand. filum.c IS the channel (it calls
  lectiones_notare before each raw call; lectiones_fopen inside filum
  would invert the layering and note every write twice). credo.c is
  the test harness that probatio_filum runs under - routing its suite
  cleanup through filum makes a filum bug corrupt the harness that
  reports it, and adds filum + lectiones to every test binary's
  closure. Options: an exemption by unit (filum, lectiones, credo as
  "canalis ipse" in unitates.stml) vs debts in canalis_debita.tsv
  (ratchet) vs TOLERA lines with causa at each site. Decide before
  the VIOLATIO flip.

## Sweep log

*Step 1, lib + tools (2026-10-26):* 21 files, 47 sites, all through
`-scribere` (guard: 97 drops by exactly the edits, nothing grows,
clang before/after). lib: aedilis_silva, mensura, specimen (gains
filum.h). tools: 18 files. Excluded by hand: lib/filum.c, lib/credo.c
(see Open). Left for manual: tools/natura_examen.c (14 fopen calls
inside its APERIRE macro), filum's recursio sites, credo mkdir with
a non-0755 mode. GENERATUM files skipped by the tool. Nine touched
tools have no gate (aemulator_vttest, canon_coquere,
librarium_collector, manus_instrumentum, natura_examen, runae_corpus,
runae_oraculum, tabula_nodorum_generare, vates_fumus): each built and
linked by hand via aedilis.

Step 1 took four commissio attempts, none from a wrong rewrite: (a) my
hand link check via `build/aedilis/X/struere.sh` left strays in
build/aedilis/obiecta that the canon action then "wrote outside its
trace"; (b) hand-written link lists (tabula_nodorum_generare.sh,
runae_oraculum.sh) lacked lib/lectiones.c - the guard's clang
-fsyntax-only cannot see linking; (c) the 96 debt ratchet: six tools
had no raw IO left, so their canalis_debita.tsv lines had to go.
Pre-commit routine since: examen over every touched file, debts,
`bin/fabrica sanare` of the in-tree producers, grep the build scripts
for hand link lists.

*Step 2, probationes (2026-10-26):* 10 files, 63 sites. Tests:
imago_png, mensura, specimen, vigilia, entitas_repositorium, herbarium,
vates, vicus_quaestio_interna, praeconium (all nine suites green after)
+ qr_assets/qr_gyrus.c (hand link list in probatio_qr_gyrus.sh and its
porta `currit=` gained lib/lectiones.c; oracle 40/40; its 96 debt
paid). Excluded: all of probationes/fixa/ (sondae_auditus = frozen
audit probes, evidence of filed defects; many probe filum itself and
must stay raw - the 97 exemption question covers them too) and
probatio_lectiones.c (tests the channel: toggles FABRICA_LECTIONES and
forks writers into its own book). Left in macros: CREDO_AEQUALIS_S32
around rename in probatio_specimen.c.

*Exemptions decided (2026-10-26, Fran; ledger decretum …24P17):* per-site
`<tolera>` for both 96 and 97, as the 96 spec already said - not a unit
exemption. Done for filum, lectiones, iter_directoria, via, credo and
probatio_lectiones (62 sites, debt lines deleted). One comment above a
call can now carry both codes: examen's tolera window also covers the
line right after the comment ENDS (4bcf5a69). probationes/fixa/ stays
as frozen input (97 is domestic there, like 96). Writing the reasons
found three unnoted reads in filum (copiare's source, the existence
stats of _si_necesse and _cum_modo; park …JVE1) and the lock-file
question (…M7QN) - their tolera say LACUNA and name the record.

*Step 3, gesta (2026-10-26, after merging main 5863a8e9 as 65ed2cca):*
7 files, 65 sites (remove 59 in the tests, write-mode fopen 6: the
ledger append in gesta.c x2, tabularium's snapshot, three in tests).
The ledger writes are meant to go through lectiones: outside a gate
nothing is noted; a gate that wrote the real ~/.rhubarb ledger would now
show it as a write outside its trace - a real bug made visible. Read-mode
fopen refusals belong to 96. All seven gesta suites green.
Tool bug found: `<inclusio>` inserted the new #include after the last
include LINE, which in tabularium.c ends inside a two-line trailing
comment - the include was swallowed, lectiones_fopen undeclared, the
guard refused (code 44). `_directivae_finis` now skips a trailing
comment to its end; pythonica test + plant (red at the new check, read
from the acta - the compendium quirk again).

*Step 4, knotapel (2026-10-26):* one site - demo_125/main.c's
D125_VICTORES write (env-gated, so the live-vs-snapshot regression
output cannot change; built and linked via a redirected aedilis
script). Snapshots (demo-snapshot.c, GENERATUM) are never rewritten
and are now frozen by declaration: role `testimonium` via the new
`<munus plagula=...>` (exact file name), and examen-corpus asks the
judge `examen_corporis`, which leaves out testimonia and archivata
(decision …0X3C2; pins for 130-133 + archivum glr_quaestio removed;
fixtures stay judged). Gate plant (testimonium back into the judge's
roles) red naming exactly the four snapshots.
Routine fix: format with `tools/formare_viae.sh` (the commit judge),
never `formator.sh` over every changed file - it reformatted
knotapel's remissa main.c wholesale (restored, re-applied).

*Step 5, silva (2026-10-27):* 10 files, 24 sites, all in the tools
(silva/instrumenta), none in the parser; 36 read-mode fopen refusals
(96). Four rewritten files are bootstrap generators (silva_amalgama,
silva_coquere, silva_canon_coquere, latina_datum) with hand-kept lib
lists: lectiones added to silva/tessera/officina amalgamare.sh,
latina_datum_generare.sh, and as silva/build/lectiones.o to
amalgama_excludenda_generare.sh + amalgama_ligare.sh (same "warm
object" contract as its siblings; warmed once by hand). The generator
build's own list (generare_fontes_generata.sh) is derived but sits
downstream of the generator - regenerated with the current aedilis
(./tools/silva_fontes_generare.sh), the bootstrap cycle. All generated
outputs then judged fresh by output comparison, byte-identical; silva
57/57; nexus fumus sanum; three 96 debts paid.

*Step 6, officina (2026-10-27):* 27 mechanical sites in 8 files
(tests, tools, officina_indicium). machinula's guest libc builtins by
hand, keeping exact libc semantics: fopen -> lectiones_fopen and stat
-> lectiones_stat (same calls, noted); remove/unlink/rename/mkdir note
S first, then the raw call under a per-site `<tolera>` (errno, int
value, any mkdir mode). A gate running a guest script that writes now
sees the write; probatio_officina_machinula section VI asserts S/L/X
in a private book (plant: guest fopen raw -> red at L). Supporting:
lectiones_notare preserves errno (lib/lectiones.worklog.md);
officina's amalgam policy gains lectiones as an excludable base
(EXCLUDENDA_LECTIONUM = lectiones_ambitus); officina test libs gain
filum; officina/build/mech_lectiones.o warmed once. Debts paid:
machinula, renovatio. Left: chmod inside CREDO_VERUM (renovatio),
a few macro/shape refusals in legatus/cursor tests.

*Step 7, apps (2026-10-27):* only apps/forum/forum.c - most apps keep
their logic in lib (already swept). Rewritten: the background image
(privata/forum/tergale.imago: two mkdir 0755, its write, its remove).
Kept raw on purpose: forum's auth token ~/.rhubarb/forum.tessera
(mkdir 0700, write, read, chmod 0600) - a secret never goes through
lectiones (fabrica would fingerprint it); per-site `<tolera>` "secretum
servi", the custodia.c/horreum.c precedent (the write carries both 96
and 97 in one comment). Built and linked in the scratchpad; the manual
gate apps/forum/fumus.sh (runs the server) left for Fran.

*Step 8, the small subsystems (2026-10-27):* briar, crusta, materia,
oratio, tessera, ludus_tessera (css, html, md, toml, saltuarius,
instrumenta, book_assets: nothing to rewrite; book_assets/capsula_libri.c
is beyond examen's limits and holds no write calls). 15 sites in 14
files, mostly write-mode fopen in tests and oracles; briar_fabrica's
chmod. tessera's test runner gained lectiones (hand list); crusta's
oracle debt paid. Suites: briar 20/20, crusta 20/20, oratio 19/19,
materia 14/14, tessera 15/15, ludus_tessera 8/8.

**The mechanical sweep is complete.** Next: the manual leftovers (calls
inside CREDO_* macro arguments, mkdir with modes other than 0755, the
few odd shapes the tool refused), then code 97 -> VIOLATIO with a plant.
