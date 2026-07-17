# gesta.c worklog

## 2026-07-15 — chunk A birth (one find)

**BIND-NULL TRAP (the chunk's one bug, found by golden 13b):**
`json_ad_chorda` on a missing/empty-context value returns a chorda
with `datum == NIHIL`; `scrinium_ligare_textum` passes `.datum`
straight to `sqlite3_bind_text`, and a NULL pointer there binds SQL
NULL — so restoring the empty `branch_id:""` from an annales line
hit `NOT NULL constraint failed`. Guard `_chorda_tuta` (NIHIL datum
→ "") wraps every json→bind seam in restituere. General law for
future consumers: never feed a json_ad_chorda result to a bind
without the guard — empty-string and SQL-NULL are different animals
at that seam.

Design notes live in gesta/phase-log.md (chunk A INTENTIO: eager
plicare, validation-before-write + custodia nota, state = one JSON
object w/ column projections, canonicalization at the door, annales
before COMMIT, emendatio = whole-replace, genus res_id = titulus,
remotio via object rebuild).

## 2026-07-16 — K4 chunk A: fold decomposition + branch read path

**The _chorda_tuta trap struck in a NEW spot.** The chain query
binds branch_id per link; the trunk link carries chorda "" whose
datum is NIHIL (it comes from json_ad_chorda of the ramus's empty
parens field). sqlite3_bind_text with NULL data binds SQL NULL, and
`branch_id = NULL` matches NOTHING — the trunk segment of every
chain read came back empty. 561/562: every negative assert (trunk
key invisible on branch) passed trivially; the ONE positive
trunk-through-branch assert (trunk nota visible in branch fold)
caught it. Fix: _chorda_tuta at the bind site. Lesson recorded at
gesta.c:106 already said "decipula NOT NULL, inventum
restitutionis" — same trap, third context (restitutio, now chain
binds). When a test section is all-negative asserts, it proves
nothing about the positive path: always include at least one
assert that requires data to FLOW.

**Decisions this chunk (not pinned by spec-v2, now owned):**
- Kernel ramus param = RES_ID of the ramus res (branch_id column
  carries res_ids). Tituli are unique only among ACTIVI — a reused
  titulus after abicere would collide in the column. Tenant
  resolves titulus→res_id (chunk C, E2-B1 spirit).
- _catena_rami is STRICT on ramus datum: parens (chorda) AND
  punctum (integer) both required on non-trunk links, else "datum
  rami corruptum". A lenient default (punctum 0) would silently
  show an empty parent view.
- Depth guard GESTA_CATENA_ALTITUDO_MAXIMA = XVI with a cycle test
  (parens self-loop via mutatio) — no hang, clean vacua.
- BOUNDARY SHIFT (owned): the trunk-purity WHERE (branch_id='' in
  _consumptorem_plicare) was spec'd for chunk B but pulled into A —
  the raw branch fixtures would otherwise be eaten by eager trunk
  folds on the next write. E2 §4 held exactly as predicted: hwm is
  a threshold, not a counter; no stall, no refold.
- Transform outcome is TRI-STATE (IGNOTUM/INANE/MUTATUM), not a
  boolean: unknown events never write; recognized-but-inane events
  rewrite the row ONLY if it exists (refreshing mutatum) — folding
  both into one flag would have broken byte-identity either way.

G9 bar: 498/498 green with kernel edits BEFORE any test additions
(decomposition + WHERE + migration IV are behaviorally inert on
trunk); then 562/562 + 114/114 with the K4 goldens (G2, G3).

## 2026-07-16 — K4 chunk B: scriptura + vita ramorum

**Zero bugs in flight** — first-run green (680/680, +118 asserts).
The E2 must-fixes were sewn into the code before any test ran, and
the chunk-A read path meant every validation swap had an already-
tested fold underneath it.

**Decisions this chunk (spec-silent, now owned):**
- actor param ADDED to gesta_ramum_creare/abicere/fundere beyond
  the spec signatures. K3 learned this the hard way (actor was
  retrofitted in chunk C with a perl sweep over every call site);
  this time the attribution surface ships with the op.
- Merge copies PRESERVE creatum (the event happened when it
  happened); merge time is visible through new seq + origo
  "merge:<t>". Attribution law (D3) extended to time.
- _copiam_inserere writes sordidae per copied row — owned addition
  the spec didn't mention: merged res change trunk state, so trunk
  FTS must reindex them. Verified by G12's quaerere("pacificus")
  finding the branch-born res post-merge.
- The copy loop MATERIALIZES branch rows into an Xar before
  inserting (SELECT closed before INSERT on the same table —
  sqlite live-scan semantics not worth gambling on).
- fundere refusals follow the agere pattern: VERUM + fusa FALSUM +
  causa ("ramus non activus" / "parens non fusus" / "confligentiae
  stant"); FALSUM reserved for apparatus/ignotus.
- _res_validationis_capere is the ONE seam for all validation
  reads (dup-check, machina, all three membrum reads) — the E2-B2
  "swap as a set" is structural now: you cannot swap half of it.

**Test-law payoff from chunk A**: every isolation section carries
at least one POSITIVE through-the-seam assert (trunk nota through
branch fold, FTS finding trunk titles, archaeology reads after
abicere) — the all-negative-section trap from the chunk-A NULL-bind
bug informed the whole golden set.
