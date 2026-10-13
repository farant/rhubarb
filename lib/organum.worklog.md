# organum worklog

## 2026-10-10 — S1: manifest loader (status-plan S1)

Built `organum` (include/organum.h, lib/organum.c) + `organum.canon`
(root; registered `<organum>` in canones.registrum) + embedded copy
`lib/organum_canon.c` (GENERATUM by tools/organum_canon_infigere.sh,
generator action `organum_canon` in aedificatio.stml - same pattern as
norma_canon: the loader never reads a file).

Two-level judgment, deliberately split:
- canon (shape): closed vocabulary, choice lists, `scriptor` must cite
  a declared `<actio>` (citatio), unique campus/actio/ligamen names.
- loader rules (what the canon's doctrine refuses: co-occurrence,
  split values): sedes=motus -> rhythmus=mobilis + vita=transiens;
  typus=series -> sedes=motus; orbatus -> referentia; referentia
  'ligamen/genus' with a declared ligamen; absens valid for typus
  (electio: among the optiones); optio only under electio, electio
  needs one.

Generated outputs:
- island canon: one `<attributum>` per campus with sedes=insula
  (motus fields excluded), root element = organum genus (per-branch
  convention), electio optiones copied (escaped).
- `<domini>`: campus with sedes=insula + scriptor + NO aspectus (an
  aspect is the system's). genus="ephemera" for now (S2 keeps the
  island split).

Evidence that it is real: probatio_organum `domini` section reads the
LIVE apps/pictor/canones/domini.stml and requires the generated owner
for each of the 11 shared attributes to match. 7 plants caught (each
loader rule family, canon skipped, motus in generated canon, aspect in
domini, base duplicates).

Naming: the lint refused `kineticus` (Greek). The house Latin word for
the kinetic tier already existed - `mobilis` (motus.h "genus MOBILE",
pictor-spec state table) - so the rhythm is `rhythmus="mobilis"`.

Follow-up: tools/organum_canon_infigere.sh is a near-copy of
norma_canon_infigere.sh - with two consumers, a generic
`canon_infigere.sh <fons> <titulus>` is due (must keep norma's output
byte-identical: generata gate).

## 2026-10-10 — S2: pictor loads its manifest

apps/pictor/organum.stml (real manifest) + apps/organum_commune.stml
(base: id, focus, focus_acervus, superficies_*). pictor_montare reads
both, merges the base, GENERATES one canon (set on BOTH islands while
the split lasts - slightly looser per island than the two old canons;
the decision point after S5 settles the split) and the owner table.
apps/pictor/canones/ (durabilis.canon, ephemera.canon, domini.stml)
deleted; the old owner table frozen as
probationes/fixa/organum/domini_ante_s2.stml.

No behaviour change, checked three ways:
- types: every attribute shared by the old canons and the manifest has
  the same type (script diff at S2; only the dead declarations differ).
  `instrumentum` stays `nomen` (tests use "nihil"; electio = a
  tightening for later).
- owners: probatio_organum `pictor_vivus` - the live manifest's 16
  generated owners equal the frozen old table; the only old owners
  absent are the dead ones (cursus, gradus, sigillum).
- suites: organum, 19 pictor, 12 vicus green.

Differences from the S0 draft, on purpose: semen stays on the island
(written there today; moves to motus later), latitudo/altitudo kept
(Fran: until a clean migration), dead declarations dropped.

Gap found by plants and closed: nothing tested that the MOUNT applies
canon and owners (planting "skip the ephemera canon" stayed green - and
before S2 a missing canon file was silently tolerated too). New
probatio_pictor_montatio goes through pictor_applicatio_aedificare:
unknown attribute refused on both islands, wrong type refused, zoom
owned by motus, focus by dispensator (base), palette ownerless (aspect);
missing manifest = mount refused (two guards: no organum, then no
canon). Gotcha: an owner check needs a value that CHANGES - writing
the current value is a no-op the gate lets through.

Now pictor refuses to mount without its manifest (before: missing canon
files = mounted without a canon). Fail-loud, intended.

Follow-up: the "read manifest + base, generate, set on branch" code
lives in pictor_applicatio.c and a test helper
(probationes/pictor_organum_probationis.h); scriba (S6) is the second
consumer - then it becomes a generic function (header to Fran).

## 2026-10-10 — S4: starting values from the manifest

"Default" had been two facts: the STARTING value (hard-coded init
string in pictor_montare) and the READING fallback (~15
`attributum_s32(ramus, "zoom", I)` calls). They disagreed for
`instrumentum` (starts "penicillus", fallback/S0 note "nihil").

Now: `organum_initium` = ' titulus="absens"' for every island campus
with an absens, manifest order; pictor_montare's ephemera init =
organum_initium + `focus="tabula"` (a pictor component id on a BASE
field - the base cannot know it). Durable init stays derived from the
drawing. instrumentum gets absens="penicillus". The mounted island now
also carries exemplar, magnitudo_lineae, pan_x, pan_y explicitly (same
values the fallbacks assumed - no visible change).

Reading fallbacks stay until typed accessors (S9); for a mounted widget
they are unreachable (every defaulted field present from mount). Test
`probatio_pictor_montatio initium`: the mounted island holds every
manifest absens (10 fields). Plant note: "edit a manifest default" is
NOT a meaningful plant - mount and expectation both follow the
manifest; the real plant is "mount ignores the manifest" (caught).

## 2026-10-12 — S6a: shared mount module (organum_montatio)

Rule of two (scriba is the second consumer): pictor's mount steps moved
into include/organum_montatio.h - `organum_legere_viam` (manifest +
common base, errors prefixed) and `organum_ramum_parare` (generated
canon, starting element `<genus [id] absentes addita/>`, restore,
owners, dimissio registration). `StatusSystemata` moved here from
pictor_applicatio.h. Pictor keeps only what is pictor's: the drawing
size + `focus="tabula"` as `attributa_addita`, and binding its drawing
for relatio (only it knows the document). Dead pictor helpers deleted
(manifestum_legere, organum_pictoris, elementum, _via).

The test helper pictor_organum_probationis.h was NOT deleted (as first
proposed): three suites build bare islands by hand and assert exact
owner counts - organum_ramum_parare would also initiate the branch. It
is now a thin wrapper over organum_legere_viam. No behaviour change: 21
pictor, 13 vicus, ludus_tessera 8/8; plants 6/6 (dimissio registration
proven through pictor's palette suite).

## 2026-10-12 — S6b: scriba on its manifest

apps/scriba/organum.stml; scriba_montare(..., StatusSystemata*) mounts
through organum_ramum_parare (addita: page size + focus="pagina"; modus
from its absens "normalis"). apps/scriba/canones/ deleted (ephemera.canon,
domini.stml - frozen as probationes/fixa/organum/scriba_domini_ante_s6.stml;
probatio_organum `scriba_viva`: generated owners == old table, 16/16).
Types identical to the old canon (script diff).

Classification (Fran): cursor persists; mode/selection/visual kind,
pending chord (clavis_praecedens, fd_*), nuntius, capsa (S6b: per pane,
transient; S6c app-wide) transient. First persisted scriba state ever.

Finding: the page a pane shows was ALREADY persisted - volume plagula
scriba/visus/<id> (vicus S2b). The island's pagina_positio and
paginae_numerus are derived copies -> transient (like pictor's size).
Removing paginae_numerus needs the book in ScribaCompositio (a header
change not approved) - left as a derived copy; candidate for the
"derived" category (census).

Finding: probatio_scriba_actiones and _ramus still loaded
apps/scriba/canones/durabilis.canon, deleted in C2 - canonem_legere
returned NIHIL silently and the tests ran WITHOUT that canon since C2.
A silent no-op the gates could not see. Now generic test helper
probationes/organum_probationis.h (manifest path in);
pictor_organum_probationis.h delegates to it.

vicus: ONE StatusSystemata allocated in vicus_applicatio_aedificare,
pointed to by both ContextusScribae and ContextusPictoris; whichever pane
mounts first fills it (systemata_parare) from the repo its mount
receives; dimissio bound to the dispatcher if created.
Tests: probatio_scriba_conservatio (round trip; canon/owners in a real
mount), probatio_vicus_conservatio latus_scribae; root 278/278,
ludus_tessera 8/8; plants 4/4.

## 2026-10-12 — S7c-1: manifests embedded in the library

Trigger: terminale is the one widget installed to ~/.bin, run from any
directory - reading apps/terminale/organum.stml at runtime would break it
(pictor/scriba only got away with disk reads because they run from the
tree via .sh). Fran: embed for ALL consumers.

Mos organum_canon/norma_canon, not capsula (capsula = big assets,
compressed, gitignored build/ output, per-binary build step - every
consumer incl. compile_tests would need the step; fresh clone breaks).
tools/organa_infigere.sh -> lib/organa_infixa.c (GENERATUM, committed):
one named array per manifest (apps/organum_commune.stml + every
apps/<genus>/organum.stml, C order; key = directory name) + a table;
`organum_infixum_textus(genus, p)`. fabrica action `organa_infixa`
(ingressus `plagulae via="apps" suffixa=".stml"` - a new manifest is
covered without editing the action; verified stale on change).

organum_montatio: `organum_legere_genus(p, in, genus, praefixum)` =
embedded manifest + embedded base; `organum_legere_viam` stays for tests
and tools reading from disk. One parser path (manifestum_ex_fonte).
pictor, scriba, vicus switched; their `radix` parameter is now dead
(`(vacuum)radix`), removed in S7c-2.

Embedded ONLY at runtime, no disk fallback (two truths = an installed
binary silently differing from the tree). Dev-loop cost: edit a manifest
without regenerating -> the app runs the old one. Guarded three ways:
probatio_organum_montatio `infixa_fontes` walks apps/ and compares each
manifest byte-for-byte with its embedded copy, naming the stale file
(plant: comment added to apps/pictor/organum.stml -> red "infixum
stalum: apps/pictor/organum.stml"; restored -> green); generata gate at
commit; session hook STALUM.

probatio_pictor_montatio `manifestum_absens` (bad radix -> mount refused)
asserted exactly the removed behaviour; became `sine_arbore` (radix
"/via/nulla" -> mount succeeds, canon + owners present). Absent manifest
= unknown genus, covered by `legere_genus`.

New lib file -> ./tools/compile_tests_fontes_generare.sh (link list).
Second link list: ludus_tessera/compile_probationes.sh keeps a HAND list
of lib objects - a new lib file must be added there too (commissio's
ludus_tessera gate went 0/8 on link errors until it was).

## 2026-10-12 — S7c-2: the dead `radix` parameter removed

After S7c-1 `radix` ("praefixum viarum canonum") only located manifests;
every app passed NIHIL. Removed from scriba_montare,
scriba_applicatio_aedificare, pictor_montare, pictor_applicatio_aedificare,
vicus_creare, vicus_applicatio_aedificare, the VicusMontator type (and
its 3 lib + 10 test implementations), and the Vicus struct. 80 call
sites by a paren-balanced script (arg at a fixed index, asserted to be
NIHIL / radix / v->radix / getenv("RHUBARB_RADIX") / "/via/nulla" -
anything else stops the run). ludus_tessera tests had passed
getenv("RHUBARB_RADIX") because that suite does not run from the root:
the reason is gone with the manifests embedded (RHUBARB_RADIX itself is
a general house variable - toml, css, diagnostica - untouched).

pictor_montatio `sine_arbore` could no longer say "no tree" through a
bad radix: it now chdir("/") around pictor_applicatio_aedificare (back
BEFORE asserting - later sections read the tree). Plant: organum_legere_genus
reading apps/pictor/organum.stml from disk -> red "manifestum abest"; reverted
-> green (silva.planta with a function gate; the gate's Porta must have
fracturae=[] or objects with .nomen, not strings - planta's summary crashes).

Tool candidate (not built): silva has renominare for renames but nothing
for "remove argument k from every call of F + the parameter from its
definitions/prototype/function-pointer type". This slice did it by
script; a silva-aware `argumentum_tollere` would make it one command.

## 2026-10-12 — status-plan S8b: mounting at depth

Toy widget `ludibrium` (probationes/fixa/organum/ludibrium.stml, read
from disk - test fixture, not embedded): one persistent field `valor`
(absens 0), one transient `nota`, action `ludibrium.agere`, and a
declared `<montatio nomen="liber" genus="ludibrium"/>` (organum.canon
accepts montationes; nothing reads them yet - the parent mounts
explicitly). It mounts itself three deep with the SAME id "liber" at
every level, as a real recursive child slot would - which is what makes
id-keyed anything collide.

S8a already carried most of it: organum_ramum_parare on a nested branch
got the canon at depth, initiation under the parent and owners by path
for free (`gradus`, `conservatio` green before any S8b change). Changes:
- relatio `rami_aequales` compared elementum+id -> two "liber" branches
  under different parents were one; now compares `via` (red first:
  second registration refused).
- organum_montatio's root check uses `via` (same meaning, clearer).
- persistence key: conservatio keeps the caller-given clavis; nested
  mounts pass the path (`status/ludibrium:liber/ludibrium:liber/...` -
  ':' and '/' fine in volume paths). Plant: keying by id -> all three
  levels share one plagula -> `conservatio` red 14/16.
dimissio still uses `ramus->id` as its namespace - S8c unifies
namespace and path.
