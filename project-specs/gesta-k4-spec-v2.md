# GESTA K4 SPEC v2 — RAMI (branches: fork, judge, discard, merge)

2026-07-16. **SELF-CONTAINED IMPLEMENTATION SOURCE — build from THIS
file, never from summaries.** Supersedes gesta-k4-spec.md (v1,
explorer target). Companions: gesta-k4-interrogatio.md (14 decisions
+ 3 charter rulings), gesta-k4-exploratio-{1,2,3}.md (verbatim
explorer reports — E1 oracle, E2 seams, E3 usage). Oracle:
../bun-libraries/smaragda.ts (branch module 5740-6076, bootstrap
1265-1307, read path 638-721, schema 269-424).

## I. WHAT K4 IS — AND WHY (the E3 challenge, answered)

Event-log branches: fork the ledger at a seq point, write JUDGED
events on the branch without touching the trunk, read the branch as
a coherent parallel present, then DISCARD (the flagship case) or
merge. Branch is a per-call PARAMETER — never resident state.

RAISON D'ÊTRE (what a ramus buys that the store + git cannot):
1. **Compaction-surviving speculation for a live resident.** Git
   versions files for humans at commit grain; a ramus gives an
   agent session a judged, queryable scratch-timeline of PROJECT
   STATE inside the running store — fork, think in records for an
   hour, discard or merge as ONE act.
2. **The sync substrate.** The original plan (Demo 1/8) built
   branch_id for device/client replication — the new-internet seam.
   K4 wakes the column and owns its semantics; sync later stands on
   it.
3. **E3's honest verdict bounded the scope**: the oracle's branches
   were demo-ware (2 ever; workspaces won parallel-org; conflict
   machinery ran ZERO times). Hence DISCARD-FIRST: abicere is the
   flagship; fundere ships simple; per-res resolution PARKED.

## II. DECISIONS (interview 14 + charter round 3)

| # | Decision |
|---|----------|
| 1 | Park split done: K4 park retitled in place (ritual binding holds); K5 palatium filed |
| 2 | Rami only; PARAMETRIC everywhere (oracle deleted kernel.currentBranch after a real concurrency bug — git 53b0372; we start where they finished); sessio/actor = park (sessions.db had 0 rows) |
| 3 | Folds: TRUNK EAGER (untouched), RAMI LAZY (chain query per read); no cache v1 |
| 4 | Merge = replay-on-merge (copies, new seq, origo "merge:<t>", actor/datum PRESERVED — oracle's single source column destroyed attribution, B4-E1; ours cannot) |
| 5 | Merged events = RAW copies (no re-validation — E2-B3) + annales each + ONE plicare + probe sweep over copied STATUS events (net-new; oracle never advanced processes on merge) |
| 6 | ramus = genus on trunk {titulus, parens, punctum; activus->fusus\|abiectus}; trunk sentinel '' stays (no history rewrite); rendered "truncus" |
| 7 | Engine trunk-only (falls out of fold purity — zero engine code) |
| 8 | Genera trunk-only; generis event with ramus = REFUSED mechanical (oracle silently swallowed these — stricter on purpose) |
| 9 | Conflict = same res touched both sides after punctum; report both states; vis (force) flag; per-res resolution PARKED |
| 10 | Surface: tool ramus {actus: creare\|enumerare\|comparare\|fundere\|abicere} + optional 'ramus' param on gerere/res (addere-on-branch: yes, via gerere-style creatio? NO — addere gains ramus param too, but title-resolution on branch requires res_id: E2-B1) |
| 11 | Annales: one interleaved jsonl, branch_id per line; restitutio ALREADY round-trips branch_id (col VIII, gesta.c ~2662 — K1 foresight, no change) |
| 12 | FTS/census/tabula trunk-only; tabula + census gain RAMI lines |
| 13 | Custodia judges on BRANCH state (lazy per-res chain fold on write path); notae travel at merge |
| 14 | Full ritual (EXECUTED — this document is its product; the ritus-signaculi instance carried it) |
| 15 | CHARTER: ship, DISCARD-FIRST (abicere flagship; fundere simple; E3 inversions honored) |
| 16 | verificare TIGHTENED: branch_id joins the line-vs-row comparison (physical-truth instrument; dec 11 'untouched' → 'extended', owned) |
| 17 | Nested merge REFUSED unless parens fusus/truncus (merge bottom-up; nesting stays readable) |

## III. KERNEL — READ PATH (rami lazy; E1 §2 + E2 §2/§3)

### Chain walk (with the E1-B2 CORRECTION — not a port)
Oracle's _collectBranchTessellae (5740-...) puts NO seq ceiling on
intermediate parents: a grandchild sees its parent's post-fork
writes (leaky nested reads). OURS CAPS EVERY LINK:

_catena_rami(m, ramus, piscina) -> Xar of {ramus_id, tectum_seq}:
walk parens chain from the ramus res state (trunk-resident genus
ramus); for chain [truncus, A, B(=ramus)]: truncus capped at
A.punctum, A capped at B.punctum, B uncapped. Ramus ignotus/
abiectus at HEAD of read: readable (archaeology, E1-B8 parity) —
only WRITES to non-activus rami refused.

### Per-res branch fold
_res_in_ramo_capere(m, res_id, ramus, piscina) -> GestaResOrdo:
SELECT genus_eventus, datum, actor, creatum FROM tessellae WHERE
res_id=?1 AND ((branch_id=?t AND seq<=?c) OR ... per link) ORDER BY
seq — then fold IN MEMORY via the decomposed pure reducer (below).
Genus column: from creatio datum during fold. Status/titulus:
projections of folded state (same rules as _rei_applicare).

### THE FOLD DECOMPOSITION (E2 §3 — the chunk-A move again)
_rei_applicare (gesta.c:746, logic 785-1072) splits:
- `_statum_transformare(m, status_obiectum, genus_columna_in/out,
  creatum_columna_in/out, res_id, genus_eventus, datum, actor,
  creatum, piscina)` — PURE: JSON state in/out, ALL reducer
  branches (K1 verbs, K2 membra fold, K3 cursus/opera/actae).
  Needs m ONLY for genus status_initialis lookup (trunk genera —
  decision 8 makes that correct on branches too).
- `_rei_applicare` (trunk wrapper): _res_capere → transform →
  INSERT OR REPLACE (existing behavior byte-identical — G-bar).
- Branch driver: fold loop calling the transform over the chain
  query rows, accumulating state in memory. No table writes.
- _membris_applicare (the INDEX) stays TRUNK-ONLY untouched: branch
  membra live in folded state.membra; _membrum_validare already
  reads state.membra from ordo.datum (E2 §3 confirmed line 562).

## IV. KERNEL — WRITE PATH (branch writes judged)

- ramus threaded through _fascis_scribere/_eventum_praeparare/
  _eventum_validare/_eventum_inserere as `constans character*
  ramus` ('' / NIHIL = truncus; ALL existing behavior byte-
  identical — the G9-bar pattern).
- _eventum_inserere: bind branch_id explicitly (E2-B6: bind ''
  for trunk, never rely on DEFAULT).
- _annalem_appendere: branch_id param replaces hardcoded "" —
  line field already exists.
- VALIDATION SWAPS when ramus given (decision 13):
  - status machina check: _res_capere → _res_in_ramo_capere.
  - creatio duplicata: judged against branch chain state (a trunk
    res born AFTER punctum is invisible to the branch — honest
    fork semantics).
  - **E2-B2 MUST-FIX**: _membrum_validare swaps ALL THREE reads as
    a set (container ~512, member existence ~525/598, standing-
    membra datum ~562) to the SAME branch fold — or spurious
    "membrum inexistens" custodia on branches.
  - genera reads stay trunk (decision 8).
- generis events + ramus → _fractum "genera non ramificantur".
- Writes to ramus non-activus → _fractum (fusus/abiectus are
  closed timelines).
- Custodia notae land on the SAME branch in the same fascis.
- Branch writes: NO sordidae, NO excite (trunk indexes/engine
  blind by construction).
- TRUNK FOLD PURITY (E2 §1 — the one-seam gift): _consumptorem_
  plicare's scan (gesta.c:1196 area) gains `AND branch_id=''`.
  hwm semantics NATURALLY correct (threshold, not counter — E2 §4:
  skipped branch seqs never stall or refold). Census/FTS/salus/
  nexus/engine trunk-purity all FALL OUT. FTS drain query also
  gains the guard if it scans tessellae (it scans sordidae — no
  change; verify in build).
- Migration IV: `CREATE INDEX idx_tessellae_ramus ON
  tessellae(branch_id, seq);` (serves trunk scan + chain reads).
- agere/processum/quaerere/census: NO ramus param v1 (trunk
  citizens — E2 §2).

## V. KERNEL — RAMUS LIFECYCLE

- gesta_ramum_creare(m, titulus, parens, piscina, res_id_out):
  parens '' (truncus) or an ACTIVUS ramus; punctum =
  gesta_seq_ultima(m); duplicate titulus among ACTIVI refused
  (E1-B9: oracle O(n) name walk — ours: one SELECT on genus ramus
  + titulus). Writes creatio {genus:"ramus", titulus, parens,
  punctum} ON TRUNK via normal path.
- gesta_ramos_enumerare(m, piscina) -> Xar {titulus, parens,
  punctum, status, res_id}.
- gesta_ramum_abicere(m, ramus) — THE FLAGSHIP: one status event
  activus→abiectus on the ramus res (trunk). Events remain;
  chain reads still work (archaeology); writes refused. CHEAP BY
  DESIGN — one event.
- gesta_confligentia(m, ramus, piscina) -> Xar of {res_id,
  status_trunci, status_rami} (both folded states as JSON text):
  SELECT DISTINCT res_id appearing in (branch_id='' AND seq >
  punctum) INTERSECT (branch_id=ramus). Genera events excluded
  (can't exist on branch).
- gesta_ramum_fundere(m, ramus, vis, piscina, exitus{fusa,
  confligentia_numerus, copiata}):
  1. ramus activus? parens fusus aut truncus (decision 17)? else
     refuse.
  2. conflicts && !vis → refuse with report (exitus carries
     count; tenant renders pairs).
  3. ONE scrinium txn: loop ramus events seq ASC →
     _eventum_inserere-style RAW copy (fresh event ULID, new seq,
     branch_id '', origo "merge:<titulus>", actor/genus_eventus/
     datum/res_id PRESERVED, annales line each — K1 crash-law
     order) — **E2-B3 MUST-FIX: raw loop, NEVER through
     _fascis_scribere/_eventum_validare (re-validation would spawn
     phantom machina notae — the branch already lived
     record-don't-block)**; then the ramus status→fusus event
     INSIDE the same txn (also raw, actor preserved as caller's);
     COMMIT.
  4. gesta_plicare once.
  5. Probe sweep: per copied STATUS event → _provectionem_excitare
     (interior, same file — callable; decision 5: merged 'opus
     perfectum' advances its process).
- gesta_res_in_ramo_datum / _status (public read pair for tenant +
  tests; parallel _ramo entry points per E2 §7 — existing publics
  undisturbed).

## VI. TENANT (tabularium.c)

- Seed v4: + genus ramus (SEMINA_NUMERUS IX→X, below
  SEMINA_BOARD_NUMERUS — no board attributa). Datum: {titulus,
  species absent (ordinary genus), status_initialis "activus",
  machina [[activus,fusus],[activus,abiectus]], attributa
  titulus/parens req + punctum numerus}.
- Tool ramus {actus: creare|enumerare|comparare|fundere|abicere,
  titulus?, parens?, vis?, res?}: comparare {res, titulus} renders
  both states; fundere renders copied count or the conflict pairs;
  abicere prominent in the description (flagship).
- Optional 'ramus' param on addere/gerere/res: **E2-B1 MUST-FIX:
  on the branch path res MUST be a res_id — title resolution reads
  trunk state and would mis-resolve; respond "in ramo res_id
  requiritur" for titles.** addere+ramus writes creatio on branch.
- census: 'rami activi N' line. tabula: RAMI section (activi:
  titulus, punctum, eventa count). Both trunk projections
  otherwise.
- Doctrine v5: branch grammar; "truncus = veritas, rami =
  hypothetica; abicere gratis - fingere audacter"; res_id law on
  branches; merge behavior.

## VII. GOLDENS (probatio_gesta m6-world unless noted)

| G | proves |
|---|---|
| G1 | fork isolation: branch write invisible to trunk reads/census/FTS/tabula; trunk write after punctum invisible to branch read |
| G2 | chain read: branch sees trunk-to-punctum + own events folded correctly (status/membra/notae) |
| G3 | E1-B2 correction: grandchild does NOT see parent's post-fork writes (per-link ceiling) |
| G4 | branch write judged on BRANCH state: machina violation vs branch status → custodia nota ON branch; trunk clean |
| G5 | E2-B2: membrum-additum on branch where both res exist only-on-branch → NO spurious custodia |
| G6 | creatio dup on branch judged vs chain (trunk res pre-punctum duplicates; post-punctum doesn't) |
| G7 | generis event + ramus refused; write to fusus/abiectus ramus refused |
| G8 | trunk fold purity: hwm advances past branch seqs, no stall/refold (property: interleave writes, refold, byte-equal) |
| G9 | THE BAR: no-ramus behavior byte-identical — all existing suites green untouched |
| G10 | abicere: one event; reads still work; writes refused; census/tabula RAMI reflects |
| G11 | confligentia: same-res detection exact (pre-punctum touches don't count); both states rendered |
| G12 | fundere happy: copies preserve actor/datum/genus_eventus; origo merge:<t>; annales lines; folds land; verificare TENET |
| G13 | fundere refuses on conflicts; vis overrides; refuses non-activus and unmerged-parent (decision 17) |
| G14 | probe sweep: merged opus-perfectum status advances a trunk process instance |
| G15 | verificare TIGHTENED catches branch_id mismatch (fixture: doctor a line) |
| G16 | restitutio round-trips branches (rebuild → chain reads byte-equal) |
| G17 | (tabularium) tool ramus lifecycle end-to-end; ramus param on gerere/res; res_id law error text |
| G18 | (tabularium) census/tabula RAMI lines; seed v4 idempotent |

## VIII. DIVERGENCE LEDGER (from oracle)

| D | divergence | why |
|---|---|---|
| D1 | Per-link seq ceiling in chain reads | E1-B2: oracle leaks parent post-fork writes to grandchildren |
| D2 | Merge atomic (copies + status in ONE txn) | E1-B3: oracle wraps only the copy loop |
| D3 | Attribution preserved through merge | E1-B4: oracle's single source column destroys actor |
| D4 | Probe sweep post-merge | E1-B5: oracle never advances processes on merge; our decision 10 demands it |
| D5 | generis-on-branch refused | E1-B6: oracle silently swallows |
| D6 | Trunk = '' sentinel, no branch res for it | E1-B7: oracle's main is a res; ours predates rami and annales are immutable |
| D7 | No per-branch eager indexes AT ALL | E1 §7-9: six index families + four merge hooks = where oracle's merge bugs lived; lazy-onto-trunk inherits zero |
| D8 | Discard-first posture | E3: conflict/force ran zero times; sandbox was the only compelling sketch |
| D9 | Branch param, never context state | E3: oracle deleted currentBranch after concurrency bug; currentContext.branchId still resident — ours stricter |

## IX. CHUNKS (suites green + G9-bar after each)

**A — decompositio + lectio** (gesta.{h,c}): migration IV (index);
_statum_transformare split (G9-bar: trunk folds byte-identical);
_catena_rami + _res_in_ramo_capere + public _ramo read pair; NO
writes yet. Goldens G2, G3, G9 partial.

**B — scriptura + vita rami** (gesta.{h,c}): ramus threading through
write path + validation swaps (E2-B2 set) + fold-purity WHERE +
annales branch_id + generis refusal; gesta_ramum_creare/enumerare/
abicere/confligentia/fundere + sweep; verificare tightening.
Goldens G1, G4-G8, G10-G16.

**C — tenens + deploy**: seed v4, tool ramus, params + E2-B1 law,
census/tabula RAMI, doctrine v5; live deploy; G17-G18; **seal via
the RUNNING ritual**: complete the 'frusta' opus → porta-sigilli
opens → sigillare fires claudere-cum-decreto on the K4 park
AUTOMATICALLY (park must be tractum — pulled at build start; if
sigillare fires early it recusata+retries: the self-heal IS the
demo). Note: the decree text carries the instance's cur_sigilli
bound at instantiation (mentions the pre-split park name — accepted
quirk, RELATIO notes it).

## X. PARKS AT SEAL

per-res conflict resolution · branch fold cache (TS cold-cache trap
documented) · branch search/FTS · speculative processes on branches
· sessio/actor entities (E3: 0 rows — still parked) · sync/device
replication (the Demo 8 seam — big, named, not soon) · branch-aware
quaerere
