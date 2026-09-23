
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
