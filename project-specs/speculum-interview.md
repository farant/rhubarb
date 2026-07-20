# speculum — interview raw (2026-07-20)

Feature: standard "debug mode" for vitrea apps — an in-app UI where you can
read all of the binary's own embedded source. First step of the
self-embedding-binaries direction (board desideratum 01KY0P4NKB, seeds the
web-component library 01KY0P5NSE).

Format: question → answer (chosen option), with the reasoning that carried.

## Round 1 — structural

**Q1. Where does the machinery live?**
A: **Separate library** (working name speculum, confirmed in Q9). App calls
`speculum_praebere(inx, ...)` to register bridge methods; vitrea never learns
about it. Same pure-composition pattern the hospitium convergence proved.

**Q2. What counts as "the source"? (vendor/ forcing case: sqlite3.c = 9.1MB)**
A: **Rebuild set minus vendor.** Everything on the compile line the house
wrote: app .c, every lib/*.c, include/*.h, latina.h, the build script itself.
Vendor bodies excluded BUT listed in the manifest (name, size, hash) so the
inventory is honest — the app admits what it isn't showing.

**Q3. How does the debug UI appear?**
A: **In-page overlay.** Injected panel over the live app page; toggling
preserves app state; works in every origin mode (capsula and URL). Separate
window rejected to avoid coupling to the multi-window park (01KXZVGE0J).

**Q4. Which provenance stamps embed alongside the source?**
A: **All four**: git hash + dirty flag; compiler version + full flag list;
build timestamp (reproducibility tradeoff accepted); exact compile command.

## Round 2 — overlay mechanics + reader

**Q5. CSS isolation without shadow DOM?**
A: **Class-prefix discipline.** One root class (.speculum-*) + scoped reset on
the panel root. Doctrine-pure; these components seed the component library, so
the no-shadow-DOM idiom must be exercised by its very first member.

**Q6. Entry gesture?**
A: **JS-side shortcut.** The speculum bootstrap registers a keydown listener
(default Cmd+Shift+D). Zero native plumbing. (Discoverability cost accepted;
menu integration can come later via helper.)

**Q7. Data path C → JS?**
A: **One dump at open.** First toggle makes one bridge call returning
manifest + provenance + every file body in a single JSON payload (~1–2MB,
held client-side). No further bridge traffic; makes full-text search a
client-side loop.

**Q8. Reader features in v1?**
A: **All four**: line numbers; file-name filter (type-to-filter tree);
full-text search across all embedded sources with file:line jump links;
basic highlighting (comments, strings, C keywords + latina.h macros via a
small regex pass — not silva-grade).

## Round 3 — naming, homes, conventions

**Q9. Name?**
A: **speculum** ("mirror" — the app beholding itself; per speculum in
aenigmate). Yields speculum.h, speculum_* bridge prefix, .speculum-* CSS.

**Q10. Where do the JS/CSS assets (future <fons-arbor>/<fons-visor>) live?**
A: **Speculum-owned dir** (lib/speculum_assets/, mirroring hospes_assets
pattern). Components graduate to a shared home when a second consumer pulls
them — the mimen story. Every app linking speculum already shares them
de facto.

**Q11. Latin doctrine in JS/HTML/CSS?**
A (verbatim): "full latin sounds good, and i know this is a little contrarian
but i prefer snake_case to camelCase when possible."
→ Full Latin identifiers in front-end code, **snake_case** (which matches the
C side's fons_arbor style anyway — camelCase would be the foreign convention
in this repo). Custom elements <fons-arbor>/<fons-visor>.

**Q12. Manifest derivation?**
A: **Build-integrated + clang -MM.** A generator invoked BY the build script,
taking the exact .c list the script compiles; clang -MM for the true header
closure (same trick as vigilia's manifests since e73f081). Drift structurally
impossible. Hand TOML rejected (compile_tools/-framework class of rot).

## Round 4 — horizon

**Q13. Theme token seed?**
A: **Global tokens + fallbacks.** Speculum CSS consumes a small flat set of
GLOBAL tokens with baked fallbacks — var(--thema-fundus, #26201a) etc., ~8
roles. Zero machinery ships now; any app or the future theming layer can
re-skin by defining tokens on :root. The contract is born working.

**Q14. Source viewer or debug platform?**
A: **Tabbed: FONS + DE.** Overlay chrome is tabs from day one, shipping two:
FONS (source reader) and DE (about — provenance stamps + the vendor/quine
exclusion inventory). Provenance needs a home anyway, so multi-panel
architecture is real, not speculative. Future panels (fructus, bridge log,
piscina stats) join later.

**Q15. Does the binary carry its own lore?**
A: **Worklogs of embedded libs.** Every embedded lib/x.c with a
lib/x.worklog.md brings it along, shown in the tree as documenta (visually
distinct from fontes). The app carries the debugging history and design
reasoning of its own parts. CLAUDE.md/full specs NOT embedded.

**Q16. Strip policy?**
A: **Unconditional.** No strip flag. Linking speculum means embedding source,
always, release builds included. That IS the point; optional flags are config
axes that rot.

## Round 5 — edges

**Q17. The quine problem (generated capsula .c is on the compile line and
contains all other sources)?**
A: **List, don't embed; show inputs.** Manifest lists the generated file
(size + hash) marked "generatum — continet hos ipsos fontes", body excluded;
the generator's INPUTS (file list + speculum_generare itself) are embedded
instead. The mirror shows a label where it would otherwise reflect itself.

**Q18. Self-modification gestures in v1?**
A: **Strictly read-only.** The mirror only shows. Copy-to-clipboard allowed
as a freebie (reading, not writing). Edit/recompile-thyself is a later
project.

**Q19. First wearer?**
A: **probatio_vitrea_hospes.** The convergence app — vitrea + internuntius +
hospitium + capsula already composed; richest embedded tree (hospitium, http,
router, tcp + worklogs); proves the overlay over a real busy app; the
flagship demo becomes the flagship mirror.
