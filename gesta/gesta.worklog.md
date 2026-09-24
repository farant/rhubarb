
## 2026-07-15 — K2 seal-bar false alarm: WAL-blind sqlite copy

Seal bar II (annales==acta) failed on the store COPY with "linea
extra in cauda (superset)" while the live store was perfectly
consistent (72=72=72). Cause: `cp tabularium.db scratch/` without
`-wal` — recent transactions still lived in the WAL, so the copy's
acta were a prefix while the copied jsonl was complete. Verificare
honestly named exactly what it saw; the corruption was in my copy
procedure, not the store. Law: verification copies of a WAL-mode
sqlite db = copy db+wal+shm together (or `PRAGMA
wal_checkpoint(TRUNCATE)` first). Same family as the stale-object
traps: the artifact you inspect must actually be the artifact.

## 2026-07-29 — renovare arc: the spawn-once guard was a live-only find

The fori proxy's start-if-absent guard (`hic_manens genitus`) was
"spawn once per proxy lifetime" — correct storm protection until
the daemon gained a renovare tool whose contract is "exit cleanly,
next request respawns me." The unit tests could never see this:
the stub-launcher specimens exercise the tabularium machine, and
the proxy sits outside it. Only driving the real daemon through a
real renewal cycle surfaced it. Fix: re-arm the flag on every
successful connection — still at most one spawn per OUTAGE, but a
renewed daemon's respawn is allowed. Law reinforced: a lifecycle
feature isn't shipped until the LIVE cycle has been walked once.

## 2026-07-30 — captura fragmentorum (arc ② of 01KYRGEGV4)

Articulus blocks now mint res. `<ideas>` + lines `<# (> text` /
`<(> text` → daemon-side pass at addere + gerere-mutatio: mints
ULIDs, stamps 12-char prefixes into the text, creates the genus if
absent (definitio res, campus textus, origo `captura:<articulus>`),
creates res (titulus + campus_tituli field), diffs titles on later
saves, soft-flags orphans (`orphanus` key; cleared on reappearance,
counted as `redivivae`). One atomic gesta_fascis_scribere batch —
a crash can never leave unstamped text next to created res, so
re-saves never duplicate.

Traps found and paid for:

- **The folded datum DROPS the genus key.** `_rei_applicare` moves
  genus to the res-table COLUMN; reading `gesta_res_datum` and
  checking `datum.genus` silently never matches. Cost an hour of
  everything-looks-right; the fix is `_cap_genus_rei` (SELECT genus
  FROM res). Law: state columns are projections — the datum object
  is not the whole state.
- **Custodia notes echo your key names forever.** The G-series
  field judge flagged `orphanus` as `clavis extra campos` on every
  orphan event, and the violation nota's TEXT contains the word —
  making "assert absent after remotio" unsatisfiable by strstr.
  Fix was semantic, not test-side: `orphanus` joined
  `_clavis_systematis` (it IS a machine key, set and cleared by the
  machine; judging it was noise).
- **gesta/compile_probationes.sh must run from the REPO ROOT.**
  Fixtures and the daemon script are root-relative; from gesta/ the
  world cannot even open and every test fails from initialize —
  looks like total breakage, is just CWD.
- **moneta_ulid is monotonic ⇒ same-ms twins differ only in the
  LAST char.** Short prefix stamps are structurally impossible with
  the standard mint. New `moneta_ulid_fortuita` (fresh randomness
  per call, stateless) makes 12-char stamps (ts 10 + rand 2) work;
  extension-on-collision guards the rest, and a re-mint loop keeps
  an earlier stamp from prefixing a later sibling's full id.

Line grammar is deliberate (not stml_legere): author bytes outside
the stamp splice stay untouched. The stamped form's STML validity
is pinned in lib/stml (captio fragmentorum, 32a1e00).

## 2026-07-30 (later) — the LIVED bar caught two things the suite missed

Fran's first real saves (articulus 01KYSPA4JBKV: genus
`project-management-genera` with 8 res, then `articuli-features`
with 2) worked — stamps landed, genera born, titles right. But the
live ledger showed every captured res carrying a spurious custodia
nota: "genus ignotum ad creationem (definitio-generis deest)". The
batch validated genus against the registry, which only receives
user definitio-res at FOLD time — the same-batch definitio was
invisible to it. My test store had the same notes all along; the
suite never asserted their ABSENCE, so nobody looked. An
unasserted side effect is an unseen side effect. Fix: GestaObumbra
gained `clavis_definitionis`, the builder fills it for definitio
creations, and the creatio check consults `_definitio_in_obumbris`
before condemning (fields not judged — progressive law; the
registry judges from the next save on). Pin added: captured res
shows NO "nota (machina)".

Second live find: Fran wrote `</>` expecting an anonymous closing
tag and it LOOKED like it worked — actually the line scanner just
ignored it and the corpus happened to end there (fourth member of
the silent-degenerate family). Filed as a real stml feature +
scanner awareness: 01KYSPRF9R ("tagus claudens
anonymus").

## 2026-09-23 — the doctrine was 57 % invisible; the NOVA greeting never arrived

Claude Code truncates an MCP server's `instructions` at 2048 characters
(measured 2026-09-21, quaestio 01M32WHJ1Q). TABULARII_DOCTRINA had grown
to 4790, so every session saw the first 43 %: mostly tool GRAMMAR that
the tool schemas already carry (schemas are not truncated). Lost past
the cut: all of MORES (search before filing, parcatum->tractum on
pickup, lying table worse than prose, the one-home division), the
renovare-after-CAUTIO rule, and the whole forum correspondent contract.

Worse, found while fixing: the reader greeting (F4, "NOVA: n eventa...
acta {ab_lecto} legenda") was APPENDED after the doctrine, i.e. always
past the cut. The feature was dead since birth and nothing noticed,
because the tests assert `strstr(r, "NOVA:")` on the raw response,
which our server sends whole. The host is the component that drops it,
and no test models the host.

Fix: doctrine rewritten to 1625 chars carrying only what no schema
repeats (contract, mores, orientation, the one-home rule, the stale
warning rule, a forum pointer). NOVA goes FIRST. What moved: the state
machines -> gerere.novus; ictus -> gerere.actus (it was missing from
the schema's actus list; the doctrine was its only mention); the rest
of the correspondent contract -> acta.ab_lecto, read at the moment the
greeting is acted on. RAMI, agere/processes, renovare mechanics were
already in their tool descriptions. Also corrected gerere's description,
which still said machine violations "non obstant" — they have been
refused since decision 01M32X21NR.

Gate: `TABULARII_LIMES_INSTRUCTIONUM` (MM + XLVIII) and
`TABULARII_NOVA_MAXIMA` (CXCII, now also the sprintf buffer, so the
reservation is structural) in tabularium.h; probatio_tabularium asserts
doctrine + NOVA_MAXIMA <= LIMES at initialize and NOVA-before-doctrine
in the F4 section. Born red on the old text (lines 442, 2459, 2461).
Headroom today: 2048 - 1625 - 192 = 231. The limit is the HOST's,
not ours — another host may cut elsewhere; the constant names the
number so it can move.

## 2026-09-23 — mappa health: a region's TAGS name it too

The "tags recurrentes nullam regionem nominantes" line compared recurring
tags against region TITLES only. Two faces of one gap (01M35C05AD):
`silva` stayed a "candidate region" although 'C89 development stack'
carries it as a tag, and after the first ordering session created ~45
regions tagged `regio,regio-tabularii`, those two tags headed the list —
tags carried by nothing BUT regions. Fix: the comparison list is region
titles plus every tag in each region's datum.tags; one change covers
both faces. Consequence worth knowing: tagging a region is now how you
tell the map "this area covers that topic" — a topic tag disappears from
the candidates as soon as any region carries it.

Test: a second fixture tag `ligatura` on the ten typographia notes, then
a region 'Rg glyphae' carrying it as a tag (not in its title); born red
at probatio_tabularium.c:4911 before the fix.

## 2026-09-23 — addere {intra}: placed at birth; and two tools lost their last parameter

`addere` takes `intra` (a region or parent park). The parent is resolved
BEFORE anything is written: unknown or ambiguous refuses the whole call
("intra RECUSATUM - nihil scriptum", candidates listed when ambiguous).
The item and its link are ONE batch (`gesta_fascis_scribere`: creatio
with a pre-minted res_id, creatio of the nexus, members a and b) — both
or neither. The capture path (articles with STML capture blocks) appends
the same three events to its own batch. Deliberately unlike `ad`, which
accepts an unresolvable target as raw text and writes item and link
separately ("res creata sed nexus respondet-ad fractus" is reachable).
Why it exists: placement used to be a second call that mostly didn't
happen — 70 parks sat in no region until the ordering session.

Found while wiring it: `ARG_ADDERE` had 13 entries registered as XII and
`ARG_GERERE` 14 as XIII, so each tool's LAST parameter (`datum`,
`ramus`) was missing from the published schema — the server read it,
agents were never offered it. Every registration now uses
`ARGUMENTORUM_NUMERUS(tabula)` (sizeof-derived). Gate: the tools/list
test parses the schema and asserts the last-declared parameter of
addere (`intra`, `datum`) and gerere (`ramus`) is published; planted
by restoring gerere's hand count XIII -> red at probatio l.508.

Also noticed: an unknown argument name is silently ignored by every
tool (the intra test was red on BEHAVIOUR, not on a refusal). Not
changed here.

## 2026-09-23 — addere {ad} brought to the {intra} shape (01M37JYNMT)

`ad` (reply-to) resolved its target AFTER writing the item, accepted an
unresolvable target as the raw-text member b, and wrote item and link
as separate writes ("res creata sed nexus respondet-ad fractus" was
reachable). Before changing it I checked whether raw text was ever
LEGITIMATE: 72 respondet-ad links across both logs (9 tabularium, 63
forum), zero raw-text targets; the forum app's `mittere {ad}` replies
to an item already on screen, so it always sends a res_id. So: same
shape as intra. `_intra_eventa_componere` became
`_vinculi_eventa_componere(verbum, ...)`; `_alterum_nati_solvere`
resolves intra and ad TOGETHER and collects every cause, so a call with
both bad refuses once naming both ("addere RECUSATUM (2 causae) -
nihil scriptum"). Item + up to two links = one batch (VII events max;
the capture path extends its own batch by up to VI). The response now
names the reply target by title, like intra.

Tests: unknown ad refused with nothing created (born red, 3 of 4);
both-bad names both causes (written after the fix, so planted: skipping
the ad resolution -> 7 failures incl. l.5013/5015).

## 2026-09-23 — unknown argument keys are refused (01M37JYP2W)

Every tool used to read only the keys it knew (`_arg`) and silently drop
the rest, so a misspelled or unpublished parameter did nothing and said
nothing — the addere {intra} test was red on BEHAVIOUR (item created,
no link), not on a refusal. Now `_toolscall_tractare` first compares
the argument keys with the tool's PUBLISHED schema and refuses:
"argumenta RECUSATA (N causae) - nihil actum", one line per unknown key
with the nearest known name when close enough (Levenshtein <= 2, or
len/3 for keys over 6 chars), then "ARGUMENTA '<tool>': <all keys>".
The schema construction moved out of tools/list into
`_instrumenta_componere()` so both use one source (the same derived
counts fixed in e6fdaaa5 — a hand-miscounted table would now refuse
its own last parameter, loudly).

Before turning it on, audited every caller: each handler's `_arg` reads
vs its table (all read keys declared); the forum app's addere/gerere/
legere keys; pythonica goes through frigida (direct C, no tools/call);
the linux smoke sends census {}. All five gesta suites green, so the
fori/tabulariumd paths send nothing undeclared either. Tests written
after the code, so planted (check short-circuited -> 8 red).

## 2026-09-23 — ids resolve by their END too, case-insensitively (01M37KBAWV)

A ULID's first 10 chars are the millisecond timestamp only; the house
minter is monotonic, so ids minted in the same millisecond share 25
chars and differ at the LAST one. Once addere {intra,ad} began creating
item + link in one batch, a 10-char "short id" of such an item was
ambiguous with its own link (seen live on 01M37JYNMT...P8J/P8K). Fran's
idea: match the fragment against either END. The end is where a ULID's
entropy lives — the git-short-hash equivalent. `_fragmentum_ulid`
(6..26 Crockford chars, lowercase accepted and upper-cased per the
Crockford spec) + `_res_per_fragmentum` (`LIKE f||'%' OR LIKE '%'||f`,
a scan of ~3k rows; a reversed indexed column is the move if it ever
needs to be fast). Ambiguity refusals now print each candidate's
shortest unique suffix (`_finis_brevis`, >= 6), so the refusal hands
you the short handle. User-facing sites only: `_res_solvere`, the
candidate lister, legere's res/nexus_ad. Capture stamps and code
citations stay PREFIX-only on purpose — their uniqueness guard
(`_cap_praefixi_numerus`) counts prefixes.

Test trap hit and avoided: "item and its link share the 10-char
prefix" is TIMING-dependent (the link id is minted at resolution, the
item's at write — usually the same ms, not always); the first run
failed on exactly that. Asserted the timing-free property instead (the
item's 6-char suffix resolves to it); the deterministic ambiguity case
lives in the prefix section. Plants: prefix-only + case-sensitive -> 5
red; empty shortest-suffix -> the refusal-hint assertion red.

## 2026-09-23 — parata: placing in a region no longer pins (decree 01M35650Z4, second site)

After the ordering session parata's AD CONSILIUM went 13 -> 77: every
dormant park placed in a region showed up as "ready to plan". Decree
01M35650Z4 (placement does not pin) had been implemented in ONE of two
places — the "parca visa, non fixa" footer's SQL excluded `intra`-to-
region links, but `_parata_computare` let every live `intra` link admit
both ends into the graph. One rule, two sites, one missed; invisible
until regions actually held things. And `addere {intra}` (shipped the
same day) would have made it grow with every filing.

Fix (Fran chose option A): nodes carry `fixus`. Seeded for tasks,
design questions and items assigned to Fran; set for both ends of any
canonical link EXCEPT `intra` whose parent is a region. Region links
still build the parent tree, so `parata {intra: <region>}` scoping is
intact. Unpinned open items get no class in the top-level view; in a
region-scoped view they get their own section "VISA, NON FIXA (in hac
regione)" so an ordering session can promote one deliberately. The
footer now also counts non-park kinds placed-but-unpinned ("alia in
regionibus collocata, non fixa: desiderata N · quaestiones N · cetera
N"); lone desiderata with no region stay uncounted (knowledge base, not
chain).

A 09-22 map test asserted an ordinary desideratum placed in a region
sits in AD CONSILIUM — it pinned the old reading. Its real point (only
the vision flag hides visions) is kept: the ordinary one is asserted in
the new section instead. New assertions born red (8 of 8).

## 2026-09-23 — the organizing frontier is ordered by activity

Fran's framing (decretum …RYBYR4): visions are ongoing, and a region
without one is the organizing activity's leading edge — derived by
mappa, never filed as a task. But the line named its first three
regions in CREATION order, so the edge led with the oldest placeholders
(Solarium, Brighton...). Now the no-vision candidates are sorted by
subtree weight (open parks incl. dormant + design questions + defects +
tasks), then most recent touch, then id, and each named region shows
its weight "(aperta N)" so the order explains itself. Xar is segmented,
so the candidates are copied to a flat arena array before qsort.

Test: a region created LAST with six parks — unnamed under creation
order (born red, 2 assertions), named with "(aperta 6)" after. The old
assertion that the empty 'Rg horizon vacuus' is NAMED in this line
pinned creation order; it now only asserts the line exists (the empty
region is still counted, and 'regiones vacuae' still names it).

## 2026-09-24 — inventarium {forma: machina} + frigida -inventarium (portae debitae T1)

Python must read inventory cells (portae debitae, project-specs/
portae-debitae-spec.md) without re-folding ledger events. The cold path
already had read verbs that are just MCP tool calls (`-res`, `-mappa`), so
`-inventarium <res>` = `inventarium {actus: tabula, forma: machina}`: one TSV
line per PRESENT cell (`ordo \t lens \t genus \t valor`), rows then lenses in
table order, no header; `\`, tab and newline escaped as `\\`, `\t`, `\n` so a
reader splits lines and columns unambiguously. Unknown `forma` is refused
naming the legal one. The escaping test sets a value holding a real tab, a
backslash and a newline; the test literals were GENERATED (three escaping
layers: machine text inside JSON inside a C literal) rather than hand-typed.

frigida.c's `FORMAE_NUMERUS` was a hand-kept `VI`; a seventh form would have
made it lie silently (the form simply unknown). Now derived from the array
like `ARGUMENTORUM_NUMERUS`; planting the old `VI` back turns probatio_frigida
red (4). frigida_fumus.sh VI checks the LIVE inventory by invariants (>= 30
lines, four fields per line, a known row) - planted by making frigida send the
human table: 43 malformed lines, red.

Note on forma as an unknown key: before this change the machine-form request
was REFUSED (unknown argument), so the "no INVENTARIUM header" assertion passed
vacuously while red elsewhere - it only guards something now that forma exists.

## 2026-09-24 — canonical link verbs refuse an unresolvable `alterum`

`gerere nexus` resolved `alterum` and, finding nothing, wrote the literal string
as the link's far end (refused only when the verb was an inverse, where the
string would become `res`). Filing expeditio's tasks I miscopied a suffix (R for
T) and got `T2 --impeditur-a--> "RDRNYX6JB"`: an impediment that can never
close. The `res` side already refused ("res ignota") and caught two other
miscopies the same day - only `alterum` let one through.

Now `_nexum_praeiudicare` refuses, for a CANONICAL verb or a synonym of one,
an `alterum` that does not resolve (cause "rem non solvit ... vinculum ad nihil
numquam clauditur"), and lists candidates when it is ambiguous (the resolver
used to be called with NIHIL for ambiguity, so an ambiguous title also became
a literal). Free verbs keep literal targets (external references); `denexus`
keeps its literal fallback (old links must stay dissolvable - it is how the bad
one was removed). `_verbum_canonicum_est` reads the same genus table as
`_verbum_iudicare`, so a world seeded before the canonical verbs refuses
nothing (progressive law). Measured before changing: over the whole ledger
exactly ONE canonical link ever had an unresolved end - this one - so no
history is invalidated. Born red: 9 assertions (the free-verb one stayed green,
as it must). Desideratum …607XVD.

## 2026-09-24 — state modules table; expeditio folded, judged, seeded (expeditio v1 T2)

Wiring a second event-owning genus (expeditio) the inventarium way would have
duplicated two branches (the fold in `_statum_transformare`, the custody check
in `_eventum_validare`). Instead `MODULI_STATUS[]` in gesta.c: {genus,
violation prefix, eventus_est, applicare, validare} per module, one lookup
(`_modulum_status_invenire`), count derived from the array. Inventarium's
existing assertions pass unchanged - the notes read the same ("violatio
inventarii: res non est inventarium"). A third module is one line.

The same-batch case is proven, not assumed: T3 writes creation + snapshot in
ONE gesta_fascis_scribere batch, and validation reads the target through the
batch overlay (obumbrae) - the test writes exactly that and asserts seq + 2 (no
custody note). Seeded genus `expeditio` (semen v11): `aperta -> clausa |
relicta`. Worth knowing: before the seed, `addere {genus: expeditio}` already
SUCCEEDED (progressive law: unknown genera pass) - it just had no state
machine, so the red test was the missing `(expeditio, aperta)`, not creation.

Formatter aside: -scribere realigned an untouched 'hic_manens integer'
declaration in probatio_gesta.c and -delta then called it a NEW finding -
writer and checker disagree (desideratum …HYFTF); the two lines were restored.

## 2026-09-24 — MCP tool `expeditio` (expeditio v1 T3)

`_tab_expeditio` + `_expeditionem_creare`. Every write is ONE
gesta_fascis_scribere batch: creare = creation + snapshot + natum-de (3) +
intra? (3); facere = tick + inventory cell; promovere = opus creation + intra
(3) + promotion. Refusals go through the same validators the engine uses
(gesta_expeditio_validare; for the cell, gesta_inventarium_validare against the
inventory's CURRENT state) before anything is written, all causes at once.

Decisions made while writing it:
- A cell marked non-applicabile never matches a filter (neither = nor !=):
  a row n.a. for the job's lens is outside the job (spec §3 updated).
- Re-ticking a `factum` row is allowed and re-stamps the rubric version.
- claudere counts a row open when it has no tick or was reopened; promoted rows
  are not open (their opus carries the work). `vis: "verum"` overrides.

Tests: all 41 were green on the first run, so I planted before believing
them - (1) facere without the cell -> 2 red; (2) n.a. passes the filter -> 3
red. That exposed a gap: my "atomicity" case only hit the expeditio's OWN
validator. Added the real one - a row removed from the inventory after the
snapshot: the tick is legal, the cell is not, so the WHOLE batch is refused and
the row stays open (plant: skip the cell refusal -> 4 red, and the failures show
the tick slipping through). Lint caught `validatoris` as a new word; renamed
the parameter to `causae_machinae` instead of growing the glossary.

## 2026-09-24 — expeditio `tabula` + parata lines (expeditio v1 T4)

One counting routine (`_expeditionis_numeros`) serves both the tabula header
and parata: facta = `factum` OR promoted-with-opus-`perfectum`; promota =
promoted with the opus still open; aperti = untouched or reopened. Parata puts
an expeditio in AD LABOREM while aperti + promota > 0 - a spec refinement: the
first wording ("no apertum rows") would have filed a job under "ready to close"
while its promoted opus was still pending. `_statum_rei_legere` moved up so
parata can use it.

Parata seams (all planted red): pinned like opus (`fixus`); EXEMPT from the
"open children -> skip" rule (else a promoted opus hides its job - plant: 2
red); classification by aperti alone (plant: 1 red). `relicta` joins the
derelict statuses. The inventory never reaches parata by construction: it has
no state machine, so it is never 'vitalis'/open - the natum-de pin worry was
moot, but it is asserted.

Two things met on the way: the formatter had re-aligned code committed in T3,
so two anchor-based edit scripts failed their exact-match asserts (nothing
written) and were redone from the current text. And ONE run of the tabularium
runner failed 9 assertions, then 19 straight runs (4 forced rebuilds) passed;
the logs were overwritten before I read them. Filed as quaestio …HGWJNC with
the hypothesis (background build/ writer vs a live path) and what would settle
it.
