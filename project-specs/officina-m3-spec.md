# officina M3 spec — indicium + vindex

*2026-07-11. Gradus 4/4 of the M3 on-ramp. Derived from:
officina-m3-fundamenta.md (grounding, four explorers) →
officina-simulatio-5.md (C1-C13) → officina-m3-interrogatio.md (twelve
DECISUS, Fran). Read those in that order on re-entry; this spec states
WHAT ships and the bars, they carry the WHY. Milestone bar (spec-v2
§XI): breakpoint/step/inspect a real credo test through the format file
only; macro-aware line mapping via token origo.*

## I. What M3 is

Three artifacts, one seal:

- **Congelatio plana** — machinula freezes each linked functio's
  instructions (and operand pool) into contiguous arrays at
  machinula_creare. Perf AND the stable instruction coordinate
  (functio, index) that everything else addresses by.
- **Indicium** (name SEALED) — the flat, RDI-inspired debug format:
  line tables (root site + full macro stack), variable table, string
  sections. Baked per-WORLD by vindex's loader while provenance is
  alive; written to disk; read back; the debugger drives through the
  FILE only.
- **Vindex** — the TUI stepper (officina/instrumenta/principalia/,
  saltuarius pattern). Formal bar = a HEADLESS probatio doing
  break/step/inspect on probatio_piscina through the file; the TUI
  must additionally exist and be daily-drivable (polish doesn't gate).

## II. DECISUS (interview 2026-07-11 — see interrogatio for verbatim)

1. Name: **indicium**; file extension **.indicium**; header magic pins it.
2. Baking: **per-world v0**; per-module = named future (addressing is
   already link-independent, nothing in the format forecloses it).
3. Producers: **vindex-only v0** (bake+write+REOPEN); fusor -indicium =
   named pull.
4. Inspect typing v0: **declared-type string + medulla type tag** per
   variable; type graph = M4-era pull.
5. Vindex home: **instrumenta/principalia** + wrapper script.
6. v0 UX surface: core (file:line puncta, perge, step-over, step-into,
   source pane, locals pane) **plus finish, memory pane,
   flight-recorder pane, function-name puncta** (+ run-to-cursor sugar).
7. Pausa sentinel: **MACHINULA_OP_PAUSA = MEDULLA_OP_NUMERUS+1,
   machinula.c-private**; medulla.h + vocabularium untouched.
8. Macro UX v0: **display-only** — position line shows the expansion
   stack; stepping by ROOT line; step-into-macro = named future.
9. In-memory line maps: **always-on in ALL consumers** (cursor,
   interpretare, vindex) — every halt report gains root-site file:line.
10. M3 seal: **headless bar probatio (formal) + daily-drivable TUI**.
11. Freeze seal: **parity (sweep stdout byte-diff + suites green) +
    measure-and-record** (probatio_vim + sweep wall-time to worklog);
    no perf threshold gates.
12. Command tongue: **single keys for the hot loop + ':' Latin command
    line** (:punctum, :functio, :memoria, ...).

## III. Chunk ① — congelatio plana (the opener)

New machinula-side derived structure (IR IMMUTABLE, conexio DECISUS
honored):

    /* officina_machinula.c — privata */
    nomen structura {
        constans MedullaInstructio* instructiones; /* contiguae      */
        i32                         numerus;
        constans i32*               blocci_initia; /* +1 clausurae   */
        i32                         blocci_numerus;
        constans MedullaOperandum*  operanda;      /* piscina plana  */
        i32                         operanda_numerus;
    } FunctioPlana;

- 4th world-build pass in machinula_creare: per conexio function index
  → walk blocci/instructiones in CANONICAL order (§III.a), memcpy into
  m->piscina allocations, record blocci_initia; flatten
  functio->operanda the same way. planae[] is parallel to the conexio
  function table — the index VOCARE already resolves
  (machinula.c:2450-2456) and initium resolves at entry.
- Tabulatum: + `constans FunctioPlana* plana`; `instructio` becomes the
  FLAT index; `bloccus` removed from the hot path (derived by binary
  search over blocci_initia at PRINT time only — halt report, ring
  rendering).
- Dispatch: fetch = &plana->instructiones[t->instructio]; SALIRE/RAMUS
  = t->instructio = plana->blocci_initia[target]. Cached
  `tabulatum_summum` pointer in Machinula, updated at push/pop (Xar
  element pointers stable) — fetch drops 3 xar calls → 0.
- Ring (AnulusFigura): stores the flat index.
- MEASURE at seal (recorded, non-gating): probatio_vim before/after;
  sweep wall-time before/after (fork-child freeze cost, sim C4; lazy
  per-functio freeze = named fallback if grievous).

**Seal ①:** cursor sweep stdout byte-diff parity + all suites green +
measurements recorded in officina_machinula.worklog.md.

### III.a — The canonical walk contract (sim C5; do this FIRST)

Indicium bakes intra-functio indices at DEMISSION; the freeze computes
them at LINK. One canonical enumeration, owned by medulla, used by both:

    /* officina_medulla.h */
    /* Ordo canonicus: blocci in ordine Xar, instructiones in ordine
     * Xar. Index planus instructionis = positio in hac ambulatione.
     * PACTUM: demissio, congelatio machinulae, et indicium eundem
     * ordinem sequuntur — probatio_officina_medulla id figit. */

Plus a pinning probatio: lower a corpus TU, enumerate indices at
demission, freeze at link, assert identical. Any future block
reordering/elision breaks the probatio loudly instead of desyncing
every line table silently.

## IV. Chunk ② — lineae ubique (the never-exercised seam)

- **Distiller fix (root site):** `_lineam_colligere` walks
  `silva_token_radix(lexema)` and records the ROOT (use-site) line in
  MedullaLineae instead of the token's own (spelling) line. Sole
  consumer (halt report) strictly improves: CREDO arg-setup moves from
  credo.h:148 to probatio_x.c:NN. Instructions inside real functions
  (credo.c bodies) are unaffected.
- **Wiring (always-on, DECISUS 9):** interpretare and cursor call
  demissio_lineas_colligere per TU BEFORE destroying the arbor piscina
  (interpretare.c:179 region; cursor's cached parent demission), stash
  one MedullaLineae per module (module-piscina lifetime; survives fork
  COW), feed machinula_lineas_praebere after machinula_creare.

**Seal ②:** a forced VITIUM in a credo suite prints [file:line] at the
ROOT site in the halt report, via cursor AND interpretare; sweep parity
unchanged.

## V. Chunk ③ — indicium (format + module)

New unit-tested module officina/fontes/officina_indicium.{c,h}
(scriptor + lector), plus format constants generated the house way.

**File layout** (host-endian, this-toolchain-only, documented; no
pointers, all cross-refs u32 indices; sections 8-aligned):

- Header: magic "INDICIUM" (8 octeti — the sealed name), versio (u32,
  lector hard-rejects mismatch), directory offset/count.
- Directory: genus-indexed fixed array {offset, magnitudo} — section
  genus IS the array index; element counts = magnitudo /
  elementi_magnitudo[genus].
- Sections v0:
  - **CHORDAE_DATA** (u8 blob) + **CHORDAE_OFFSETA** (u32[]; chorda i =
    [off[i], off[i+1]) — chorda-on-disk, deduped at bake; index 0 = nil).
  - **VIAE** {chorda_idx; retro_first; retro_numerus} — file table.
  - **MODULI** {titulus_idx}.
  - **FUNCTIONES** {titulus_idx; modulus_idx; instructiones_numerus;
    lineae_first/numerus; variabilia_first/numerus}.
  - **LINEAE** (forward, per functio, PC-ordered): parallel arrays —
    sorted intra-functio instruction indices (+1 closing entry per
    functio) ∥ records {via_idx; linea; profunditas; macro_chorda_idx}.
    Range [idx[i], idx[i+1]) → record i. **Macro frames = consecutive
    entries at the SAME instruction index, root first (profunditas 0)
    then deeper** — the RDI inline-site encoding carrying our expansion
    stack. NIHIL-origo instructions inherit no entry (principled gaps —
    lookup rounds down to the previous range).
  - **RETRO** (reverse, per via): sorted ROOT line numbers ∥ ranges into
    an INDEX_CURSUS run of sites {functio_idx; instructio_idx}. Indexed
    by ROOT lines (users break on lines they see); a header via's sites
    span modules — vindex queries every module's via entries (linear,
    fine v0).
  - **VARIABILIA** (per functio): {fons_titulus_idx (source name from
    semantica, pre-disambiguation); registrum; typus_tag (medulla
    type); typus_chorda_idx (declared C type, display)}. No extents
    (DECISUS: whole-frame lifetime is the truth of our lowering).
  - **INDEX_CURSUS**: shared u32 pool for variable-length runs,
    referenced (first, numerus).
- Schema discipline: one X-macro table generates the section-genus
  enum + elementi_magnitudo[] + a size-assert in the probatio (stops
  producer/consumer drift; RDI's rdi.mdesk instinct, house form).

**Scriptor:** fed during per-module demission (parsura ALIVE — the only
moment the origo chain is walkable): per instruction in canonical-walk
order, distill the full root→…→spelling stack (silva_token_radix +
per-frame nomen_macro/linea); per functio, harvest register names +
semantica types. indicium_scribere(via) emits the flat file.

**Lector:** indicium_aperire = fread the whole file into one 16-aligned
piscina block; validate magic + versio; cast-and-index accessors
(indicium_linea_de_indice, indicium_sitas_de_linea,
indicium_variabilia_functionis, ...). mmap = named later (POSIX pull).

**Seal ③:** round-trip probatio — bake a fixture world (a real credo
TU), write, reopen, assert: forward lookup at a CREDO-line instruction
returns the stacked entries (root = test file line, deeper = credo.h);
reverse lookup of the test-file line returns the instruction sites;
variable lookup returns {name, register, tag, type string}; size-assert
table green.

## VI. Chunk ④ — pausa / gradus / puncta (VM surface)

- New halitus genus **MACHINULA_PAUSA**: loop exits, NO exitus
  packaging, all state resumable (grounding fact 4).
- API: **machinula_gradus(m)** — execute exactly one instruction
  (extracted loop body); **machinula_pergere(m)** — run until
  pausa/halt; machinula_currere stays as aperire+pergere compat.
  (Standing rule: new public rows ⇒ hospes calls, same change.)
- **Puncta by patching the FROZEN copy** (sim C11 — legal, machinula
  owns it): swap op at (functio, flat idx) for MACHINULA_OP_PAUSA
  (= MEDULLA_OP_NUMERUS+1, machinula.c-private; DECISUS 7), saving the
  original. Dispatch's sentinel case → pausa BEFORE execution.
  Continue-past = restore, gradus once, re-patch. Zero hot-path cost
  for run-to-punctum.
- **Step semantics** (vindex-side over gradus, using indicium ranges):
  step-over = resolve current forward-table range once, gradus until
  index leaves [idx_i, idx_i+1) at frame depth ≤ entry depth;
  step-into = same without the depth guard; finish = gradus until
  depth < entry depth. Binary search only at line boundaries.
- **MEASURE (sim Q12):** gradus-as-function call overhead in the -O0
  daily build, recorded at seal. Acceptance posture: structure wins;
  if grievous (>~10% on probatio_vim), the named fallback is pergere
  keeping an internal loop that shares the extracted body.

**Seal ④:** probatio driving the API directly on a fixture world —
punctum hit at the expected flat index, register read mid-pause,
gradus/pergere/resume semantics, depth-guard step-over; plus the
overhead measurement recorded.

## VII. Chunk ⑤ — THE BAR (headless)

probatio_officina_vindex.c (or vindex_nucleus probatio): build the
world containing probationes/probatio_piscina.c via the interpretare
load path (steps 1-6; factor a small shared loader helper if the copy
exceeds ~a screen — decide at implementation, name it if deferred),
collect lineae, machinula_creare (freeze), **bake indicium, WRITE the
file, REOPEN it, and from that point use ONLY lector data**:

1. Set punctum at a CREDO line of probatio_piscina.c via RETRO lookup.
2. pergere → assert MACHINULA_PAUSA at an expected site.
3. Inspect a local by SOURCE NAME via VARIABILIA (assert value, type
   tag renders the raw word correctly).
4. Step-over → assert the ROOT line advanced; position stack shows the
   macro frame (display string assembled from stacked entries).
5. finish → assert frame depth decreased; continue → suite runs to
   BENE with stdout intact.

**This probatio is the FORMAL M3 seal** (DECISUS 10) — mechanically
re-verifiable forever.

## VIII. Chunk ⑥ — vindex TUI

officina/instrumenta/principalia/vindex.c + officina/vindex.sh
(saltuarius pattern: thin principale on officina.h + tessera.h).

- Frame loop verbatim from saltuarius: draw-all → tessera_praesentare →
  eventum_expectare → event→command mapper → per-pane handlers →
  view-model mutation.
- **Paint functions take a PLAIN view-model snapshot struct, never the
  live Machinula*** (headless cell tests via tessera_pons_memoriae +
  PROBA_CELLA on literal fixtures — the saltuarius seam).
- Panes v0: source (with punctum markers + current-line highlight +
  macro-stack position line), locals/registers (name, typed value, raw
  word), call stack (frames from m->tabulata with root file:line),
  memory (hex+ascii at address, follow-pointer), flight-recorder (ring
  tail rendered like the halt report), puncta list.
- Commands: keys for the hot loop (gradus/step-over/step-into/fini/
  perge) + ':' Latin command line (:punctum <via>:<linea>, :punctum
  <functio>, :memoria <inscriptio|%reg>, :curre-ad <linea> sugar).
  Final key map at implementation; help row always visible.
- App-side chorda_aedificator discipline: pre-size + reset per frame,
  never finire (saltuarius lore).

**Seal ⑥ (non-gating polish bar):** daily-drivable — one real
debugging session on a real suite performed in it; headless cell
probationes green.

## IX. Named deferrals (owner/landing spot)

- Per-module indicium baking + manifest — pull: incremental/caching
  workflows (format already permits it, sim C7).
- fusor -indicium standing artifacts — pull: first real reach.
- Step-into-macro action — pull: after display-only proves demand.
- Full C89 type nodes / struct-member walking — M4-era; would subsume
  the interpretare aggregate hex-dump deferral.
- Variable lexical extents — landing: any register-reuse/liveness work.
- mmap lector — POSIX pull at scale.
- Watchpoints / conditional puncta — post-v0 vindex pulls.
- Lazy per-functio freeze — fallback if the ①-seal sweep measurement
  bites.
- Carried from instrumenta prima, unchanged owners: lector suffix-pair
  strictness; typedef-uses silva export; silva-lsp (post-M3).

## X. Process notes

- Chunk order ①→⑥, each independently sealable; Fran commits at seals.
- Every chunk RELATIO carries the instrument debrief
  (adhibita/fructus/asperitates/desiderata); **at M3 close, audit
  sim-2 §5's usage prediction against organic-use data** (the standing
  caveat: instrumenta-prima numbers were mostly validation).
- Worklogs: officina_machinula.worklog.md (freeze + gradus measurements),
  new officina_indicium.worklog.md, vindex notes in its own worklog.
- Small corrections to fold in while passing (fundamenta §VII):
  machinula.h:14 "registra" comment (vindex's register pane makes it
  true — or fix the comment at chunk ④); ring size note in the worklog.
