# GESTA K3 SPEC v2 — ACTIONES + PROCESSUS

2026-07-16. **SELF-CONTAINED IMPLEMENTATION SOURCE — build from THIS
file, never from summaries.** Supersedes gesta-k3-spec.md (v1, explorer
target). Companions: gesta-k3-interrogatio.md (16 decisions),
gesta-k3-exploratio-{1,2,3}.md (verbatim explorer reports — E1 oracle,
E2 seams, E3 live-system mining). Oracle: ../bun-libraries/smaragda.ts
(Actions 3180–3673, Processes 4655–5428 — both now fully mapped).

## I. WHAT K3 IS

1. **ACTIONES** — recipes as data: gated preconditions, typed
   argumenta, atomic effect bundles from a closed verb vocabulary,
   execution history as events on the recipe's own res.
2. **PROCESSUS** — orchestration as data: 2–3 named lanes of steps
   (opus/actio/porta), instances as ordinary res that snapshot their
   definition at start, advancing via an eager post-commit drain loop.
3. **New kernel primitive: the atomic bundle** (gesta_fascis_scribere)
   — N events, one transaction, N annales lines before COMMIT, folds
   once after.

Machinula reducers = separate park (01KXNT1ZPFRW9ZQ6BE3BPY8AH0). The
tier-2 seams stay named (§VIII).

## II. DECISIONS (interview 14 + explorer round 2)

| # | Decision |
|---|----------|
| 1 | Scope: actiones + processus |
| 2 | Gate BLOCKS, effects RECORD (custodia intra fascem) |
| 3 | Execution = actio-facta event on the action's res; NO side tables |
| 4 | Closed verb list, 1:1 onto event kinds |
| 5 | Tokens: $arg.X, $res.X, $nunc, $novus.N |
| 6 | species:"actio", opes/argumenta/effectus, whole-JSON, emendatio-in-place |
| 7 | Affordances: gesta_actiones_rei + res-tool 'actiones:' line |
| 8 | Mechanical failure: rollback + actio-recusata (gate refusals too) |
| 9 | Steps v1: opus/actio/porta (fetch+ramus parked as a PAIR) |
| 10 | Advancement: eager drain loop, STATUS-events-only membra probe |
| 11 | Seed v3: opus + claudere-cum-decreto + ritus-signaculi |
| 12 | MCP: one new tool agere |
| 13 | Recipes are code (no MCP definition surface) |
| 14 | Full ritual (executed: 3 explorers → this spec) |
| 15 | Cross-lane joins: PORTA-ONLY (oracle model; 'post' parked as sugar) |
| 16 | Refused actio-gradus = BLOCKED step, retries on next trigger (mechanical failure = gradus defectus) |

## III. FASCIS ATOMICUS (kernel primitive; E2 §1 design adopted)

Decompose _scribere_crudum (gesta.c:1176–1440) into transactionless
helpers + a driver:

- `_eventum_praeparare` — resolve res_id / canonicalize / flags.
  **Creatio ULID minting moves OUT to the caller** (the actio executor
  mints so it can bind $novus and list ids in actio-facta — E2 B4).
- `_eventum_validare` — the record-don't-block block verbatim; pure
  reads; returns violatio literae aut NIHIL.
- `_eventum_inserere` — INSERT + seq readback + creatum readback +
  annales append + sordidae; NO BEGIN/COMMIT; runs inside open txn.

```c
nomen structura {
    constans character* event_id;   /* NIHIL = cuditur */
    GestaEventum        eventum;
} GestaFascisEventum;

b32 gesta_fascis_scribere(GestaMundus*, constans GestaFascisEventum*,
    i32 numerus, character* res_ids_out /* NIHIL aut numerus*27 */);
```

Driver: per event praeparare+validare; build flat work-list
interleaving each event with its custodia-nota event immediately after
(notae land INSIDE the bundle — the K1 second-write at gesta.c:1477
dies); ONE BEGIN; loop inserere; ONE COMMIT. Annales lines in seq
order before COMMIT — crash ⇒ superset, verificare names it (K1 law
unchanged). **$nunc = the creatum of the bundle's FIRST row, reused**
(strftime advances between statements; one stable timestamp per
bundle). gesta_scribere becomes a 1-element wrapper + plicare + drain
(§VI) — observable behavior byte-identical (chunk A bar).

**Intra-bundle validation overlay (E2 B3, decided):**
_eventum_validare consults an in-memory overlay of res created earlier
in the SAME bundle (a small list of {res_id, genus} threaded through
the driver) — a later membrum-additum referencing $novus.N sees the
just-created res; no spurious "membrum inexistens" custodia. Overlay
covers existence + genus only (status checks against overlay use the
genus's status_initialis).

## IV. ACTIONES

### Definition (genus document, species:"actio") — FLAGSHIP, seed v3

```json
{
  "titulus": "claudere-cum-decreto",
  "species": "actio",
  "opes": [
    {"titulus": "parcum", "genus": "parcum",
     "status_necessarius": "tractum"}
  ],
  "argumenta": [
    {"titulus": "cur", "typus": "textus", "necessarium": true}
  ],
  "effectus": [
    {"verbum": "creatio", "ut": "decretum_novum",
     "datum": {"genus": "decretum", "titulus": "$arg.cur"}},
    {"verbum": "membrum-additum", "res": "$novus.decretum_novum",
     "datum": null},
    {"verbum": "nota", "res": "$res.parcum",
     "datum": {"textus": "clausum: $arg.cur"}},
    {"verbum": "status", "res": "$res.parcum",
     "datum": {"novus": "clausum"}}
  ]
}
```

CORRECTION to the linking effect above (spec-authoring honesty): a
link is a nexus RES (K2), so "link decretum to parcum" = THREE
effects: creatio {genus:"nexus", verbum:"natum-de"} ut:"vinculum" +
membrum-additum on $novus.vinculum {pars:"a", membrum:"$novus.decretum_novum"}
+ membrum-additum on $novus.vinculum {pars:"b", membrum:"$res.parcum"}.
The seed v3 flagship carries all six effects (creatio decreti, creatio
vinculi, membra duo, nota, status). Verbum "natum-de" gets sanctioned
in the vocabulary nota at deploy. This exercises $novus twice and the
B3 overlay thrice — deliberately.

### Verb slate (sanctioned; 1:1 onto event kinds)

creatio (cum 'ut'), mutatio, remotio, status, nota, membrum-additum,
membrum-remotum. All non-generis → sordidae correct (E2 B5).

### Substitution (action layer)

$arg.X (whole-token keeps JSON type; embedded coerces — TS 3376–3391
parity) · $res.X (ops binding res_id) · $novus.N (earlier creatio ut:N;
forward reference = mechanical failure) · $nunc (bundle timestamp).

### Executor: gesta_agere

```c
nomen structura {
    b32                 facta;        /* FALSUM = recusata */
    constans character* causa;       /* si recusata */
    chorda*             res_novae;   /* $novus ids, ordine 'ut' */
    i32                 novae_numerus;
} GestaActioFructus;

b32 gesta_agere(GestaMundus*, constans character* actio_titulus,
    constans character* ligamina_json, constans character*
    argumenta_json, Piscina*, GestaActioFructus* exitus);
```

1. Load def; species != actio → refuse (mechanical).
2. GATE: each ops binding exists + genus matches + status_necessarius
   matches; each argumentum present + K2-typed. Failure → write
   actio-recusata {ligamina, argumenta, causa} on the action res
   (single-event bundle) → return recusata.
3. Substitute; mint creatio ULIDs; resolve $novus; mint event ULIDs.
4. Build ONE bundle: all effect events + actio-facta {ligamina,
   argumenta, eventus:[pre-minted ids]} appended last, on the action
   res. Write via fascis. Effects validate record-don't-block WITH the
   B3 overlay; machina violations = custodia notae inside the bundle.
5. Mechanical failure at any stage (unknown genus, bad forward ref,
   allocation, duplicate creatio res_id) → nothing written (or full
   rollback) → actio-recusata → return.

### Affordances

gesta_actiones_rei(m, res_id, piscina) → Xar of action tituli whose
opes[i] could bind this res now (genus + status_necessarius match).
Full genera-table scan — 9 rows post-seed-v3 (E2 §5); species column
parked for scale.

## V. PROCESSUS

### Definition (genus document, species:"processus") — ritus-signaculi
(seed v3, PORTA-ONLY rewrite per decision 15)

```json
{
  "titulus": "ritus-signaculi",
  "species": "processus",
  "opes": [ {"titulus": "propositum", "genus": "parcum"} ],
  "ordines": ["consilium", "aedificatio", "sigillum"],
  "gradus": [
    {"titulus": "interrogatio", "ordo": "consilium",
     "genus_gradus": "opus", "positio": 0},
    {"titulus": "spec", "ordo": "consilium",
     "genus_gradus": "opus", "positio": 1},
    {"titulus": "exploratores", "ordo": "consilium",
     "genus_gradus": "opus", "positio": 2},
    {"titulus": "porta-spec", "ordo": "aedificatio",
     "genus_gradus": "porta", "positio": 0,
     "condiciones": ["spec"]},
    {"titulus": "frusta", "ordo": "aedificatio",
     "genus_gradus": "opus", "positio": 1},
    {"titulus": "porta-sigilli", "ordo": "sigillum",
     "genus_gradus": "porta", "positio": 0,
     "condiciones": ["exploratores", "frusta"]},
    {"titulus": "sigillare", "ordo": "sigillum",
     "genus_gradus": "actio", "positio": 1,
     "actio": "claudere-cum-decreto",
     "ligamina": {"parcum": "$ops.propositum"},
     "argumenta": {"cur": "sigillum ritus: $arg.cur_sigilli"}}
  ],
  "argumenta": [
    {"titulus": "cur_sigilli", "typus": "textus",
     "necessarium": false}
  ]
}
```

- Sequencing: (ordo, positio) — in-lane strictly sequential; lanes
  concurrent and unordered (E1: lane position decorative — we omit
  lane positions entirely; `ordines` is just the sanctioned name
  list). Cross-lane joins ONLY via porta condiciones (all-of, step
  tituli — E1 §2/§3).
- **Named opes + $ops templating + process argumenta are BUILT
  features** (E1 B4: no oracle machinery — startProcess has one bare
  context_res_id; action-step params verbatim). Two-layer
  substitution: the PROCESS resolves $ops.X and $arg.X in an
  actio-gradus's ligamina/argumenta before calling gesta_agere; the
  ACTION then resolves its own tokens in effects. Opus-gradus may use
  $ops/$arg in titulus/descriptio fields too.
- Opus steps may carry: opus_titulus (default = gradus titulus),
  descriptio, prioritas (E3: keep cheap, no logic on them).

### Instance model

- Instance = res; created by agere {processus: titulus, ligamina,
  argumenta}: creatio event whose datum SNAPSHOTS the definition
  (gradus + ordines + resolved opes + argumenta) — **snapshot-at-start**
  (divergence from oracle live-join: E3 §2 showed genus evolution
  retroactively rewriting instance step-lists into nonsense —
  "completed" with pending steps). Whole-replace emendatio makes
  live-join strictly worse for us; running instances stay coherent,
  refold-stable.
- Instance genus = the processus titulus; status machine: currens →
  perfectus | defectus | omissus (E1 B5; omissus = cancel, no cascade
  — orphan opera parked).
- Spawned opera: res of genus opus, linked instance
  --pars "<gradus titulus>"--> opus via membra (divergence from
  oracle's attribute linkage, E1 B3 — deliberately: one link
  machinery, and the advancement trigger rides the membra index).
  **Requires E2 B1 fix**: _membrum_validare (gesta.c:436-440) and the
  solutum-purge (:967) currently demand species:nexus — gate becomes
  "genus declares partes OR species processus" (processus instances
  carry implicit partes = their gradus).
- Instance stream events: gradus-incohatus {gradus}, gradus-perfectus
  {gradus, effectus?}, gradus-defectus {gradus, causa},
  porta-perfecta {gradus}, opus-genitum {gradus, opus}, actio-acta
  {gradus, actio_facta_id}, processus-perfectus/-defectus. Blocked
  portae emit NOTHING (divergence from oracle's gate_evaluated{false}
  noise re-emitted every pass — E1 B7; our eager trigger would
  amplify it).

### Advancement engine (E2 §2 shape adopted verbatim)

- **Trigger**: in gesta_scribere's wrapper, after plicare: for each
  newly-written STATUS event only (E1: oracle triggers only from
  completeTask ≡ our status-transition path; direct pendens→perfectum
  included naturally), ONE probe:
  `SELECT m.res_id, m.pars, r.genus, r.status FROM membra m JOIN res
  r ON r.res_id = m.res_id WHERE m.membrum = ?1` (idx_membra_membrum)
  → filter species:processus + status currens. No hit = done (chunk B
  inert for all non-process writes).
- **Drain loop** `_provehere_ad_fixum`: flat transactions, NO
  recursion (scrinium has no nested BEGIN — scrinium.c:213):
  loop { compute ready steps from CURRENT folds → none = fixpoint →
  else write their events as one micro-bundle → plicare → repeat }.
  Micro-bundles per advancement pass keep B3 confined to within a
  single actio's own bundle (E2 addendum: each pass re-folds, so
  later spawns see earlier spawns).
- Per-lane walk (oracle algorithm, E1 §3): terminal → next; ACTIVE
  opus → lane parked; next step: porta (all condiciones perfectae? →
  porta-perfecta + continue; else silently parked); actio → resolve
  $ops/$arg, gesta_agere: facta → actio-acta + gradus-perfectus,
  continue; RECUSATA → step stays pending, retried next trigger
  (decision 16 — recusata events still land on the action res, so
  the attempt ledger is intact); mechanical → gradus-defectus, lane
  parked; opus → creatio opus + membra link + opus-genitum +
  gradus-incohatus, lane WAITS.
- Completion: all gradus perfecti → processus-perfectus + instance
  status perfectus. Any defectus && nothing active && not all done →
  processus-defectus (E1 §7 cascade).
- **Replay safety**: engine lives ONLY in the scribere wrapper;
  replicare/restituere call plicare only — advancement events replay
  as data (decision 10; E2 confirmed).

## VI. TENANT (tabularium.c)

- **Seed v3**: +3 SEMINA_GENERUM entries (opus, claudere-cum-decreto
  six-effect flagship, ritus-signaculi), SEMINA_NUMERUS VI→IX,
  absent-by-title idempotent. **E2 B2 must-fix**: scope the v2
  attributa-fusion loop to the five board genera (split
  SEMINA_BOARD_NUMERUS = VI) so board attributa never stamp onto
  opus/actio/processus. opus genus datum: {titulus req textus;
  assignatum, prioritas, effectus textus; pendens→susceptum→
  perfectum|omissus + pendens→perfectum + pendens→omissus} (E3: the
  susceptum state stays legal but nothing routes on it).
- **agere tool**: {actio?, processus?, ligamina? (JSON obj titulus→
  res_id|titulus exactus), argumenta? (JSON obj)}. Actio → facta
  summary (novae res, event count) or recusata causa. Processus →
  instance res_id + first-spawn summary. Dispatch/tools-list/doctrine
  landing sites: E2 §6 (all pins additive-safe).
- **res tool**: 'actiones:' line (affordances) after salus block.
- **Doctrine v3**: teaches agere + recepta-sunt-codex + opus lifecycle.
  (Pin "CONTRACTUS SCRIBAE" preserved.)
- Vocabulary nota: sanction link verb "natum-de" at deploy.

## VII. GOLDENS (probatio_gesta unless noted; oracle/report refs)

| G | proves |
|---|---|
| G1 | fascis atomicity: mechanical failure mid-bundle → zero rows, zero annales lines |
| G2 | fascis: N events = N annales lines in seq order; verificare TENET |
| G3 | custodia notae land INSIDE the bundle (violation effect → nota event adjacent, same txn) |
| G4 | $nunc identical across all bundle events |
| G5 | gate refusals ×5 (ops absent/inexistent/wrong-genus/wrong-status/arg missing/arg type) → actio-recusata + ZERO effects |
| G6 | flagship six-effect execution: atomic, actio-facta lists all pre-minted ids (E2 B4) |
| G7 | $novus ×2 + B3 overlay: no spurious "membrum inexistens" custodia on intra-bundle refs |
| G8 | effect machina violation records (custodia) and bundle completes (decision 2) |
| G9 | gesta_scribere unchanged: K1/K2 suites byte-identical behavior (chunk A bar) |
| G10 | affordances: status-dependent (tractum parcum affords claudere-cum-decreto; parcatum doesn't) |
| G11 | wart-killer: emendatio-generis on an action redefines in place — one genus, history in annales |
| G12 | instance snapshot: emendatio on processus genus mid-flight → running instance unaffected |
| G13 | lanes concurrent + in-lane sequential (E1 §3 walk semantics) |
| G14 | porta all-of; blocked porta emits NO events; passes exactly once |
| G15 | opus status → advancement (incl. direct pendens→perfectum); non-membra status writes don't trigger (probe negative) |
| G16 | refused actio-gradus stays pending, self-heals when precondition status later arrives (decision 16) |
| G17 | mechanical actio-gradus failure → gradus-defectus → processus-defectus cascade |
| G18 | instance→opus membra links draw NO custodia (E2 B1 fix proven) |
| G19 | replicare == standing WITH advancement history (events replay as data; engine silent) |
| G20 | (tabularium) seed v3 idempotent + B2 scoping: board genera keep attributa; opus/actio/processus carry none of them |
| G21 | (tabularium) agere: flagship happy + recusata surfaces causa |
| G22 | (tabularium) res shows actiones: line; census unchanged shape |
| G23 | (tabularium) ritus-signaculi instance starts; consilium opera spawn; tabula shows instance |
| G24 | actio res FTS corpus stays bounded as facta stream grows (E2 B5) |

## VIII. MACHINULA SEAM (named, deferred — park 01KXNT1ZPF…)

Action 'fons' field (C89 source) alternative to 'effectus' — K3
refuses fons-bearing actions by name. Genus 'reducer' (K1-reserved).
Processes: fetch+ramus PAIR parked (E1 §6: both are reads-off-context
substrate; branch needs fetch; forward-only same-lane skip semantics
documented in exploratio-1).

## IX. DIVERGENCE LEDGER

| D | divergence | why |
|---|---|---|
| D1 | No input/action_taken tables — execution is events | db = projection law; E3 §4: effect-level provenance already reality |
| D2 | Whole-JSON defs; no action/process reducers; evolve* last-writer-wins unported | D8 pattern; emendatio-in-place kills checkout_v4 wart (E3 confirmed only v4 ever ran) |
| D3 | Effects record-don't-block inside gated bundle (TS throws) | decision 2 |
| D4 | $novus creata refs (TS cannot) | flagship needs; B3 overlay supports |
| D5 | create_log/create_error → nota | K2 D4 |
| D6 | fetch+ramus deferred as a pair | context-flow substrate absent by design |
| D7 | opus↔instance via membra, not attributes (E1 B3) | one link machinery; trigger rides the index |
| D8 | Process opes/ligamina/argumenta + $ops/$arg templating = BUILT (E1 B4: zero oracle machinery) | flagship cannot run without it |
| D9 | Instance snapshots definition at start (oracle live-joins) | E3 §2: live-join + evolution = nonsense states |
| D10 | Blocked portae silent (oracle re-emits gate_evaluated{false} every pass) | eager trigger would amplify noise (E1 B7) |
| D11 | Refused actio-gradus = blocked-retry (oracle fails step) | decision 16; recusatio isn't an error |
| D12 | Advancement = status-only membra probe + flat drain loop (oracle: completeTask hook + recursive re-reads) | scrinium has no nested txns; generalization owned (E1 B2, E2 §2) |
| D13 | Cancel (omissus) does not cascade to spawned opera | oracle parity actually; cascade parked |

## X. CHUNKS (E2 §7 boundaries; all additive, suites green after each)

**A — fascis + actiones** (gesta.{h,c}): §III decomposition (G9 bar:
existing behavior byte-identical), gesta_fascis_scribere, B3 overlay,
_species_est generalization, gesta_agere, gesta_actiones_rei, verb
dispatch, facta/recusata. Goldens G1–G11, G24.

**B — processus + provectio** (gesta.{h,c}): definition parsing,
instance snapshot creation, membra linkage (WITH E2 B1 fix), the
advancement drain + status-probe trigger, porta semantics, statuses.
Inert without instances — zero behavior change to existing suites.
Goldens G12–G19.

**C — tenens + deploy**: seed v3 (WITH E2 B2 scoping fix), agere tool,
res actiones: line, doctrine v3, vocabulary verb sanction; live
deploy; ritus-signaculi instance started with K4's park as propositum;
seal bars. Goldens G20–G23.

## XI. SEAL BARS

1. Suites green; examen ACCIPE on every touched file.
2. Live store: annales==acta TENET; replicare==standing byte-equal
   (WAL-complete copy — the K2 lesson).
3. Flagship executed via agere against the LIVE store: a real tractum
   park closed, decretum filed AND linked, in one atomic bundle whose
   actio-facta names every event.
4. ritus-signaculi instance CURRENS with K4's park bound as
   propositum — the store orchestrating its own next milestone.
5. **The K3 seal itself performed via agere** (the ledger bar).
6. Doctrine v3 delivered on reconnect; vigilia confirms fresh binary.

## XII. PARKS BORN (file at seal)

fetch+ramus pair (context substrate) · 'post' sugar over portae ·
cancellatio cascadens (orphan opera) · piscina growth under bulk agere
(E2 §2 caveat) · species column in genera (scale) · MCP definition
surface (safety decision, from Q13) · susceptum routing (if claiming
ever matters — E3 says it doesn't yet)
