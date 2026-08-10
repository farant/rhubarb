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

## 2026-08-04 — ui slice I: repositorium (commits + files in the window)

First surfacing of the VCS in silex ui, deliberately crude for
Fran's red-team rounds. Three READ-ONLY bridge handlers in
tools/silex.c (vcs_historia / vcs_plica / vcs_plagula, via
defaulting to the ui session's cwd) + <silex-repositorium> overlay
(tools/silex_assets/repositorium.js — app chrome, NOT part of the
mensa battery): conditiones | plagulae | contentum three-pane, a
"repositorium" chip top-right that only appears when a project is
found under cwd, Escape closes.

- Time-travel browsing came free and is the architecture's party
  trick made visible: every commit click is plicam_ad + per-file
  massam_promere — the working tree is NEVER touched, "arbor
  praesens" (seq 0) is just the fold with no ceiling. History
  browses like the present because in this design it IS the same
  read.
- _volumen_viam_invenire promoted to public
  silex_volumen_viam_invenire (renominare did the splices) — the
  handlers needed the identity rule and it's genuinely public API.
- The overlay scrolls INSIDE its columns — mensa's planum-never-
  scrolls law untouched (this is not a planum).
- Stale-object lesson repeated: struere links against build/*.o;
  after a lib rename, run the suite (which recompiles) BEFORE
  struere or the linker sees the old symbol table.
- Binary content over JSON is the named v1 limitation (C sources
  fine; images would garble) — massa:// vitrea scheme is the
  ripening answer.

## 2026-08-04 — ui slice II: the widgets become cards

Fran's call: the overlay was chrome bolted onto a thinkboard app;
the native form is cards. repositorium.js rewritten — overlay and
chip deleted, three DECLARED cards in index.html:

- silex-conditiones / silex-plagulae / silex-contentum, each
  extends MensaScida — drag, %-position persistence, z-order
  levatio, selection all inherited free. Update-in-place linking
  (conditio electa -> plagulae -> contentum) via document events;
  spawn-file-cards + drag-items-out = deferred by Fran, and with
  update-in-place the genus-registration seam mensa would need for
  dynamic cards is ALSO deferred (YAGNI paid immediately: declared
  singletons ride the existing declared-node path).
- Never-scroll honored: MensaPaginator (new battery primitive in
  logica.js) + ‹ › arrows in the card foot; contentum paginates BY
  LINES (24/page). Cards resize in WIDTH only (imago's ansa
  pattern) — height is a function of page size.
- v2.1 CSS law applied at birth: the three silex tags get their own
  grouped base-style block (new tags never inherit mensa's grouped
  selectors).
- Rows stopPropagation on pointerdown: card drags by its chrome,
  rows click freely (no accidental drag-select from list items).
- Known v1 roughness for the red-team round: row selection
  highlight doesn't survive page flips; selected seq/via not yet
  persisted in the fold (would survive restart if stored as
  internum — cheap follow-up).

## 2026-08-04 — ui slice III: the plagulae list becomes a spawner

- Click = linked in-place view (contentum follows), unchanged.
  DRAG a row past 6pt = a ghost chip follows the pointer; drop
  creates <silex-visus> via planum.chartamCreare — a PERMANENT
  card of that file at that seq, persisted in the mensa fold
  (via/seq/x/y/latitudo/altitudo/ordo), rebuilt on restart through
  the registered factory. × deletes (deletum through the fold).
- Line views (contentum + visus) rebuilt on SilexVisusLinearum:
  soft wrap (break-all, monospace), BOTH-axis resize, pagination
  recomputed live during the grip drag (rAF-gated; all arithmetic
  via the new logica trio). Anchor rule live: the top line you
  were reading picks the post-reflow page and glows while you
  drag (unless page one).
- A visus at seq 0 is a view of the PRESENT manifest — it will
  honestly change content after a condere. Views at seq N are
  frozen forever. Both are correct; worth feeling in the hands.

## 2026-08-10 — silex installed as a real binary
silex_struere.sh now installs to ~/.bin/silex (rm-before-cp, the
macOS SIGKILL-on-overwrite trap). Fran's zshrc alias removed —
aliases only exist in interactive shells, so scripts and subshells
never saw silex; SILEX_FABRICA moved .zshrc → .zshenv so
non-interactive zsh gets it too. Posture: the fabrica is a registry
you name (-f / SILEX_FABRICA), not a room you work in. Verified:
plain sh from silicetum/vocabularium resolves ~/.bin/silex, historia
works; status works even with SILEX_FABRICA unset (cwd verbs never
touch the fabrica). The "unplug the fabrica" test now fails only
where it should: novum/renovare.

## 2026-08-10 — the corpus lands: silex carries the registry inside
Spec/plan: project-specs/silex-corpus-{spec,plan}.md. Seven commits
216cd22..5164f36. What shipped:
- **SilexFons seam** (DISCUS|CORPUS): the one disk-reading function
  (_plagulam_e_fonte_colligere) reads through a two-op fons; the
  differential gate (hand-built CapsulaEmbed vs disk ficta) pins
  corpus==discus byte-exact.
- **Closure rules complete**: lib/X_macos.m twin when lib/X.c absent;
  vendor pairs normalized ('../vendor/y.h' -> vendor/y.h — capsula
  keys can't resolve '..'); vendor files are LEAVES (their contents
  cite windows.h/tclsqlite.h — never scanned; caught live on
  vocabularium, not by fixtures).
- **Renovare re-rooted**: seeds = includes of authored DISK files
  (pre-condere includes count — the vocabularium scenario was the
  pre-fix oracle, RED then GREEN) ∪ vendored capita; basis-compared
  filter (../fontes/x.h names an authored header; capsula_* generated
  names dropped).
- **partes verb**: closure oracle with ADEST/ABEST, -machina TSV;
  seeds union the manifest's vendored capita so ABEST == what
  renovare -scribere would vendor. v1 takes at most ONE named plagula
  (parser has two fixed positionals; plural when argumenta grows
  varargs).
- **Corpus in struere**: root toml generated (never hand-listed),
  gzip'd capsula (24MB raw -> binary 11MB->18MB), regen mtime-guarded
  so the stamp = last corpus-affecting build; SORDIDUM scoped to
  corpus content (Fran's staging files always dirty, not corpus).
  Build: 7s total including regen.
- **Resolution**: -f > ascent > corpus; SILEX_FABRICA deleted (code +
  ~/.zshenv).

THE WALK'S CATCH (why acceptance walks exist): first unplugged-fabrica
run resolved the fons to THE SCAFFOLD ITSELF — every silex project
carries the fabrica signum (include/latina.h + lib/), so ascent found
probandum as its own registry, xar.h "deest", corpus never touched.
Masked before by SILEX_FABRICA outranking ascent. Fix: a directory
with a root *.volumen is a PROJECT and never a fabrica candidate
(silex_volumen_viam_invenire == NIHIL joins the signum). Second walk:
AMBULATIO PLENA — novum/partes/renovare/aedificare/probare/condere,
all in-project, fabrica renamed away, then restored.

Also: /bin/sh set -e did NOT abort on a failing `a && b` list inside
the first walk script (condere ran after aedificare failed) — walk
scripts now guard each step with explicit `|| exit`.
