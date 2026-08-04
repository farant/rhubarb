# silex worklog

## 2026-08-04 — v0: the novum verb (first strike)

Design banked on ledger res 01KZ4Y9A8T (single distribution binary,
sources within, VCS seed). v0 ships only `novum`: closure from the
FABRICA (the rhubarb checkout — the "where libraries come from" is a
parameter; capsula embedding is a distribution-day decision, not a
prerequisite), scaffold, record everything in a volumen, project to
disk.

Mechanics + traps:
- **Closure = BFS over quoted includes** with the aedilis convention
  (include/X.h → lib/X.c automatic). v0 scan is TEXTUAL (lineatim) —
  a `#include "x.h"` mention inside a comment block would be picked
  up (gesta.h and silva_lexicon.h both contain such comment mentions).
  Mitigation: unresolvable cited headers are a WARNING + skip, never
  fatal — real deps always exist in the forge, phantom mentions don't.
  Promotion path when it bites: silva parsura → directivae.
- **Projection reads FROM the volumen** (plagulam_promere), not from
  the in-memory buffers that were just condere'd — the promere path is
  exercised on every novum from birth, and probatio_silex pins
  disk == volumen for a vendored file.
- chmod +x via processus_exsequi (/bin/chmod) — non-fatal on failure
  (`sh aedificare.sh` always works). No POSIX in silex itself.
- First real strike: silicetum/001 (outside git, per the dogfood
  protocol) — 10 vendored + 5 generated; scaffold compiles under the
  FULL flag battery and its credo probatio passes. The closure
  correctly dragged postulata_posix.h (foundation quotes it since the
  Linux arc) — 001 is Linux-portable from birth without silex knowing
  anything about POSIX.
- Scaffolded seeds: latina/piscina/chorda/credo → closure pulled in
  chorda_aedificator too (transitive). 15 plagulae total; volumen
  ~450KB. Fine.

Next verbs (designed, not built): status (three-way sigillum compare),
condere (absorb working tree), proicere (re-project), historia (walk
acta). Then the MCP posture.

## 2026-08-04 — the ui verb (front door opens)

`silex` bare / `silex ui` opens a vitrea window: capsula-embedded
index.html (tools/silex_assets/, generated pair COMMITTED per the
vitrea_assets pattern), one `status` bridge call on load (versio +
fabrica + validity) so the C↔JS loop is proven from the first screen.
Bare-with-no-args = ui; fabrica optional for ui, still required for
novum. Build moved to ./tools/silex_struere.sh — compile_tools.sh
can't know about the capsula source (regenerates it when index.html
is newer). isatty() detection considered and deferred: no-args is
already an explicit signal; the tty check earns its place when bare
silex-in-a-pipe should print help instead (that file will then owe
postulata_posix.h).

## 2026-08-04 — VCS verbs I: status / condere / historia

The doctrine landed as designed: A COMMIT IS A NAMED POINT IN THE
EVENT LOG (a `conditio` actum), not an object. Trees are folds;
the manifest is the current fold; massae dedup means nothing is
stored twice. Working tree = projection, volumen = residence.

- status: pure reader. directorium_ambulare (includere_occultos
  FALSUM — solves .DS_Store, but also means DOTFILES ARE INVISIBLE
  to the VCS v1; named limitation). Praetermissa: bin/, build/,
  *.volumen(-wal/-shm). Compare sigilla vs manifest →
  MUTATA/NOVA/ABSENS; clean files just count.
- condere: runs status, refuses an empty diff (exit 1), absorbs in
  ONE transaction. Volumen grew caller-owned transactions
  (in_transactione flag — plagulam_condere/removere skip their own
  BEGIN when the caller holds one; sqlite forbids nesting) +
  plagulam_removere (actum plagula-remota + DELETE; massa stays —
  history still references it). Commit origo = "condita" (the
  ORIGINAL vendicata provenance survives in acta; renovare will
  derive three-way facts from history, not the manifest).
- historia: walks acta; entries = ortus + conditiones; tactae =
  plagula events since previous entry. novum now writes a birth
  conditio ("proiectum excusum (novum)") so new projects' history
  owns its birth files; PRE-EXISTING projects (001/002, scratch)
  show birth files rolled into their first user commit — honest
  artifact of retroactivity, not a bug.
- Retroactivity proven: silicetum/001 (born before the verbs
  existed) answers status (omnia munda, 15 plagulae) and historia
  (ortus) with zero migration.
- THE HOUSE TRAP FIRED AND WAS CAUGHT: countdown loop `i32 index;
  index >= 0` — always true (i32 unsigned), infinite loop. Examen
  flagged "comparatio vana" AT EDIT TIME, pre-run. s32 for
  descending counters, always. The lint paid for itself again.

Next: proicere (plan-default covenant, -scribere applies; later
-ad seq = time travel via fold), then renovare on top.

## 2026-08-04 — fabrica auto-detect (ascent from cwd)

Half of desideratum 01KZ6CSYME (the .silexignore half stays with
the volumen-bearing-scaffold arc). Red-team stumble III: --fabrica
was demanded even when cwd WAS the fabrica.

- `silex_fabricam_invenire(piscina, initium)` in lib/silex.c:
  via_absoluta(initium), then ascend via via_directorium looking
  for the marker = include/latina.h (file) + lib/ (directory).
  Termination: via_directorium("/") == "/" — parent mensura
  unchanged breaks the loop; 64-step guard against pathology.
  via.h had every piece (absoluta/directorium/currens) — no POSIX
  touched, no postulata_posix needed.
- Rung THREE in tools/silex.c: option > SILEX_FABRICA > ascent >
  NIHIL. ui benefits automatically (resolution happens before
  _ui_currere). Refusal message now names all three roads.
- Parameterized initium made it testable without chdir games:
  probatio ascends from "lib" (finds repo root, compared against
  via_absoluta(".")), from "." (finds), from "/tmp" (NIHIL).
- Dogfooded for real: `env -u SILEX_FABRICA bin/silex novum` from
  repo root scaffolded probe_ascensus into scratch; its
  aedificare.sh + probare.sh green. From outside the tree: loud
  refusal, correct.

## 2026-08-04 — proicere: the volumen writes the tree (VCS II)

Fourth VCS verb, the inverse of condere. Doctrine held all the way
down:

- **No HEAD to detach.** `proicere -ad N` folds acta up to seq N
  (volumen_plicam_ad — lives in VOLUMEN, it's the same replay G8
  replication wants) and writes that tree. The log never moves; the
  tree becomes old; status shows honest MUTATA; the next condere
  makes the past the present as a NEW point. Revert = new commit.
  The full loop is pinned in probatio AND was run by hand on
  probe_ascensus (historia reads: ortus → excusum → experimentum
  vandalicum → reversio ad puritatem).
- **Plan-default covenant**: bare proicere never writes; -scribere
  applies. Classes: SCRIBENDA (differs, content recorded → safe),
  CREANDA (missing from disk), INTACTA (count only, mtime never
  touched), OBEX, ALIENA (on disk, not in fold — named in the plan,
  never written, NEVER deleted; deletion stays manual v1).
- **OBEX = the content-addressed store paying rent**: before
  overwriting, sigillum the disk content and look it up in massae.
  Not found anywhere in history → unrecorded → -scribere refuses
  loudly ("conde prima"). One hash lookup makes "nothing unrecorded
  is ever destroyed" an absolute invariant, no force flag in v1.
  (Lookup is massam_promere for existence — reads the blob just to
  check; a volumen_massam_habet would be cheaper, on-pull.)
- **argumenta grew under this verb**: registering "-scribere" as a
  brevis exposed that _parsere_breve TRUNCATED every single-dash
  token to "-X" ("-scribere" matched "-s" — silent-false-match
  family). Fixed to exact whole-token matching; multi-char
  single-dash names (the house style) now work; unknown-option
  errors name the real token. All four other argumenta consumers
  use 2-char shorts / double-dash longs — truncation could only
  ever produce false matches, so no legitimate usage broke. Pinned
  in probatio_argumenta ("-scribere" ≠ "-s" asserted).
- Projection loop generalized from silex_novum's birth path (mkdir
  parents, massam_promere by the FOLD's sigillum — plagulam_promere
  only answers for the current manifest).

Next: renovare (three-sigillum comparison — missum-tunc /
proiectum-nunc / missum-novum) on top of proicere's machinery.

## 2026-08-04 — renovare: the three sigilla (VCS III / distribution I)

The verb the silex vision was named for: vendored libraries catch up
with the fabrica, content-compared, no network, no timestamps.

- **missum-tunc from acta, not manifest** — the decision condere
  recorded at ITS birth (origo "condita" preserves the last
  vendicata actum as provenance) paid off exactly as planned: the
  tunc fold is a walk over plagula-condita acta filtered to origo
  praefix "vendicata:", with plagula-remota dropping scope (a
  vendored file you deliberately removed is NOT resurrected).
- **missum-novum via the existing closure** — seeds = vendored
  include/*.h basenames, silex_clausuram_colligere recomputed
  against today's fabrica returns content for the whole set AND
  discovers new dependencies (ADDENDA) — if the new chorda.c grows
  an include, it's vendored in rather than silently breaking the
  build. Upstream-dropped files fall out as DERELICTA (the seed
  warning is the honest stderr trail).
- **Classes**: RENOVANDA (tunc==nunc, safe by construction — disk
  content IS the recorded massa), ADDENDA, VULNUS (yours, upstream
  immota), CONFLICTUS (both moved; v1 never merges), DERELICTA.
  nunc==novum≠tunc counts INTACTA (you updated by hand; stable).
  Vulnera/conflictus are RETAINED AND NAMED but do not block the
  rest — apt-style holding back, unlike proicere's OBEX refusal
  (there we'd be DESTROYING content; here we're just not helping).
- **Apply = conditio**: write + absorb origo "vendicata:<fons>" in
  one caller transaction + conditio actum "renovatio e fabrica" —
  an update is a named point in the log; proicere -ad reverts it.
  Absorbing with vendicata origo is also what makes renovare
  idempotent (tunc becomes the new sigillum).
- **XAR IS SEGMENTED** — caught pre-run: xar_obtinere(x, 0) cast to
  a flat array works until the first segment fills, then reads
  garbage. Seeds array built with piscina_allocare + fill loop.
  (Tabula exponentialis, not contiguous — worth remembering.)
- Probatio drives a **fabrica ficta** (mutable fake fabrica in the
  build area — the real repo can't be mutated mid-test): all five
  classes + idempotency exercised. Real-fabrica smoke: probe_
  ascensus "omnia recentia (10 intactae)"; hand-edit → VULNUS
  retained under -scribere.

VCS verbs now: status / condere / historia / proicere / renovare.
Next pulls when they bite: conflictus merge tooling, massam_habet
(existence check without blob read), capsula embedding
(distribution-day: fabrica ficta becomes fabrica in the binary).
