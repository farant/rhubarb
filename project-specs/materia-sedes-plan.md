# materia sedes Plan — positions in the projection, diagnostics from declarations

**STATUS: OPEN 2026-09-17** — written the day the spec was approved;
nothing executed. INTENTIO filed in `materia/phase-log.md` with this
plan's commit.

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> (inline, per house convention — no subagents) to implement this plan
> task-by-task, ONE TASK PER TURN with Fran's approval. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** Every materia client gets an opt-in positioned STML view whose
elements carry their source range (so pattern rows carry lines), the STML
engine compares repeated captures under TRANSPARENTIA, and crusta and css
report located diagnostics DERIVED from registry declarations, printed in
the compiler format with a source excerpt by one instrument.

**Architecture:** `project-specs/materia-sedes-spec.md`. Part A: one range
function beside `materia_sedes_tokeni` (`materia_nodus`), stamped by the
writer (`materia_arbor`) when `consilium.sedes_scribere` is set, verified
per client by a shared library gate (`materia_sedes`), consumed by
pythonica. Part B: declarations in `<cliens>.registrum.stml` baked by
`materia_coctor` into a separate `<P>_DIAGNOSTICA` table, one walker
(`materia_diagnostica`) derives records, crusta adds its one
non-declarable class, `lib/excerptum` prints, `tools/diagnostica.sh`
routes by suffix, `silva.diagnostica` wraps it.

**Tech Stack:** C89 via latina.h, house flags (`tools/vexilla.sh`), credo,
piscina, chorda, chorda_aedificator, Xar, tabula_dispersa, stml +
stml_macros, materia (`materia/fontes`), `./materia/coquere.sh`, canon,
pythonica (`silva.commissio`, `silva.commissio_umbra`, `silva.planta`),
bash 5.2 at `/opt/homebrew/bin/bash` (measurement only).

**Spec:** `project-specs/materia-sedes-spec.md` (commit `855589fb`). §2
the range, §3 the view, §4 the engine, §5–6 A's surfaces and gates, §7
declarations, §8 derivation, §9 crusta and css, §10 the printer, §11–12
B's surfaces and gates, §13 order. Where this plan's §Deltas differs from
the spec, the plan governs (measured while planning) and closure writes
the spec's "as built" section.

## Global Constraints

- **Latin throughout** — identifiers, comments, messages. English only in
  specs, plans, worklogs, phase-log, CLAUDE.md.
- **Every latina.h word is a FORBIDDEN identifier** (`nomen`, `registrum`,
  `magnitudo`, `structura`, `casus`, `brevis`, `duplex`, `si`, `per`,
  `dum`, `fac`, `exire`, `interior`…); single capitals `I V X L C D M` are
  Roman numerals; two-letter locals are refused by the lint; a digit
  suffix is a NEW word. Check every new word with `./oratio/quaere.sh
  w1 w2…` BEFORE staging (measured unknown today: `undula`,
  `structuraliter` — do not coin them).
- **`chorda` is NOT null-terminated. `i32`/`i64` are UNSIGNED**; offsets,
  sentinels and anything subtraction-derived take `s32`.
- **`<tag>` at the START of a comment is an annotation anchor; `*/`
  inside a comment closes it; `??-` in a string is a trigraph.**
- **New C files through `./silva/scribe.sh <via> <<'EOF'`**; a new lib
  source also needs `./tools/compile_tests_fontes_generare.sh`.
- **House format**: `./silva/formator.sh <viae> -scribere` then `-vitia`
  (lines ≤ 72, banners 50 `=`); the formator never reflows comments —
  wrap them by hand.
- **Generated files are never hand-edited**: `<cliens>_registrum_coctum.{h,c}`
  come from `./materia/coquere.sh <declaratio> -scribere`; the client's
  registrum probatio compares bytes.
- **Exit 2 from any runner = NOTHING RAN.** Never gate on exit 0 alone.
  Logs: `build/test_logs/<runner>.log`, one probatio's credo lines
  `build/test_logs/singulae/<probatio>.log`.
- **Every gate red at birth by a planted fault** that fails the ASSERTION,
  not the build: `silva.planta(via, vetus, novus, porta_nomen, filtrum)`
  for C; plants in `.py`, `.sh`, `.stml` are done by hand (edit, run, see
  red, revert, see green). Never `silva.Editio` on a `.sh`.
- **Docs first, gates last, then commit** through `silva.commissio(msg,
  viae, portae)` or, for the long client suites, `silva.commissio_umbra`
  — explicit paths, never `git add -A`. Messages end with
  `Co-Authored-By: Claude Opus 5 (1M context) <noreply@anthropic.com>`.
- **Every materia writer/reader commit** (A1, A2, B2) runs
  `./materia/shim_probare.sh` green and ALL client suites: `materia`,
  `css`, `md`, `html`, `oratio`, `crusta`, `briar`.
- **Never stage Fran's files**: `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`; never touch untracked
  `project-specs/exempla/*.html`. Never `git stash` in exploratory lines.
- **Silva stays FROZEN.** `lib/stml_macros.c` is root lib, not silva.
- After any `.stml`/`.canon` edit: `./tools/natura_struere.sh`.
- **Oracle binary** `/opt/homebrew/bin/bash` 5.2.15, environment
  `env -i LC_ALL=C PATH=/nonexistent`, never `/bin/bash`.

## Deltas from the spec (measured while planning, 2026-09-17)

| # | spec said | measured | plan does |
|---|---|---|---|
| D1 | end = start advanced over the value (§2) | md and oratio have DERIVED tokens today (`ORIGO_MD`, `ORIGO_ORATIONIS`: `est_fons` FALSUM, value possibly decoded, `oratio/fontes/oratio_stml.c:16-28`) | a derived token's range is a zero-width POINT at its carried position; `MateriaTractus` gains `est_fons`; the gate skips the slice check for points and counts them |
| D2 | the writer "can collect element pairs" (§1.2) | `paria` are collected but `sedes_valorum` is never filled (`materia_arbor.c:1843`, no writer) — a dead half-port | untouched; the gate pairs elements by document order instead |
| D3 | both equality sites change (§4) | `_congruentia_silvae_aequales` serves only `stml_congruere_strictum`, which has no TRANSPARENTIA context by design (`lib/stml_macros.c:7414`) | only `_ligamen_ponere` (loose matching) changes; strict stays byte-exact |
| D4 | a client without declarations gets an empty table (§7) | an empty array is not C89 and five generated files would churn | the coctor emits `<P>_DIAGNOSTICA` only when the declaration has any; others regenerate byte-identical |
| D5 | `vacua` = written list with zero elements (§7) | `{ }` leaves `grex/liberi` UNWRITTEN; `{ ; }` holds a `malum`; `_lista_plena` ignores `separator`, `heredoc`, `malum` (`crusta_arbor.c:647-670`) | `vacua` = unwritten, or every element a node of a genus declared `inanis="verum"`; new genus attribute `inanis` |
| D6 | every class declarable (S7) | `ramus/probatio` counts only for `elif` (`crusta_arbor.c:708-716`: `tok_deinde` written or probatio a list) — a conditional | parser-emitted into `CrustaParsura.diagnostica`; `materia_diagnostica_derivare` merges an `emissa` list |
| D7 | counters equal derived counts class by class (S9) | the arithmetic machine increments at several sites for one tree trace (`crusta_arithmetica.c:89-547`) | gate = per-class PRESENCE agreement + `sana` ⇔ zero diagnostics, exact; counts printed with disagreeing cases |
| D8 | AUDIENDA II (§15) | `[[ a b ]]` → `iudicium-binaria/tok_operator` absent; `(( 1 2 ))` → `binaria/tok_operator` absent; `(( 3 ! ))`, `$(( 1 + ))` → `binaria/dexter` absent; unterminated heredoc → `heredoc/tok_delimitator` absent (probes via `crusta/arbor.sh`) | declared `absentia`; the rest measured in B3 |
| D9 | AUDIENDA IV (§15) | `bash -n` exit 0 for `[[ a b ]]` (with a message), `(( 1 2 ))`, `(( 3 ! ))`, `$(( 1 + ))`, and a heredoc at EOF (warning); exit 2 for `{ }` | those four classes are `gravitas="monitum"`; a new differentia pin "no erratum ⇔ bash exit 0" |
| D10 | the instrument routes and links clients (§11) | crusta's and css's builds each carry their own materia/lib objects; linking both collides | `tools/diagnostica.sh` compiles its own object set into `build/diagnostica/`; its TSV adds `initium finis` so pythonica can cut `textus` |
| D11 | repeated node captures compare their forests byte-wise; only TRANSPARENTIA needs removing (§4) | measured during A3: the byte-wise write carries the AUTHOR'S FORM — layout `spatia_*` (the first child after an open tag owns the newline, deeper nodes carry deeper indentation), `</>`, `(>`, `<t\>` `indentatio`. Two identical `imperium` in `crusta/arbor.sh` output: 330 vs 329 bytes, never equal, with or without TRANSPARENTIA (quaestio 01M2Q2BDH4). Scan: 58 repeated names in tracked rules, 0 node captures outside the A3 fixtures | with Fran: equality compares CONTENT always (`_nodum_comparandum` drops author form, plus TRANSPARENTIA parts when active); strict matcher untouched |

---

## Task A1: The range, the view in the writer, the materia gate

**Executed 2026-09-17 (`325180d5`).** Gate 63/63 at first compile; plant `finis + I` red at `t.finis == 13`; shim 396/396; all seven client suites green. Every consilium goes through `nudum` (Step 1 clean).

**Files:**
- Modify: `materia/fontes/materia_nodus.h` (after `materia_sedes_tokeni`,
  line 175), `materia/fontes/materia_nodus.c` (after
  `materia_sedes_tokeni`, line 52)
- Modify: `materia/fontes/materia_arbor.h` (`MateriaArborConsilium`, last
  field after `forma`, line 320)
- Modify: `materia/fontes/materia_arbor.c` (scriptor struct 47-71,
  `_trivium_scribere` 884-995, `_scribere_lexema` 1166-1453,
  `_scribere_nodum_internum` 1644-1794, `_arborem_struere` 1876-1964,
  reader 3641-3648)
- Create: `materia/probationes/probatio_materia_sedes.c`
- Create: `materia/fontes/materia_arbor.worklog.md`
- Modify: `materia/CLAUDE.md` (new section "Sedes (2026-09-17)")

**Interfaces:**
- Consumes: `materia_sedes_tokeni(uncus, token, MateriaSedes*)`,
  `chorda_aedificator_*`, `stml_attributum_addere_chorda`.
- Produces: `MateriaTractus {initium, finis, linea, columna, linea_finis,
  columna_finis, fons_index, est_fons}`;
  `vacuum materia_tractus_lexematis(constans MateriaOrigoUncus*,
  constans MateriaToken*, MateriaTractus*)`;
  `b32 materia_tractus_nodi(constans MateriaOrigoUncus*, constans
  MateriaNodus*, MateriaTractus*)`;
  `vacuum materia_tractus_conferre(MateriaTractus* summa, b32* inventum,
  constans MateriaTractus* pars)`;
  `MateriaArborConsilium.sedes_scribere` (`b32`); envelope
  `visio="sedes"` / `"partialis sedes"`; reader causae
  `"visio sedes: non arbor"`, `"visio partialis sedes: non arbor"`
  (`"visio partialis: non arbor"` unchanged).

- [x] **Step 1: Confirm no hand-built consilium escapes `nudum`.**
  Run `grep -rn 'MateriaArborConsilium' --include='*.c' . | grep -v
  '^./build'` and check that every variable is filled by
  `materia_arbor_consilium_nudum` (which memsets) or a client helper that
  calls it (`md_stml_consilium`, `oratio_stml_consilium`). A positional
  initializer or a missing `nudum` gets its fix in this commit (a garbage
  `sedes_scribere` would stamp views silently).

- [x] **Step 2: Write the failing gate** with `./silva/scribe.sh
  materia/probationes/probatio_materia_sedes.c`. Its lexicon and
  registry are `probatio_materia_arbor.c`'s (copy lines 25-67: `FORMA`,
  `G_*`, `GENERA`, `LEXICON`, `LOCI`, `GEN`, `REG`) plus `_lex`
  (lines 131-144). Source bytes of the view fixture: `"  /*c*/x\n"` —
  spatia `"  "` @0 1:1, commentum `"/*c*/"` @2 1:3, ident `"x"` @7 1:8,
  nova linea `"\n"` @8 1:9.

```c
/* probatio_materia_sedes.c - Tractus et visio sedium
 * (materia-sedes-spec par. II-III)
 *
 * Tractus lexematis: fontis per octetos valoris promovetur, derivatum
 * punctum est, syntheticum sine sede. Tractus nodi: lexemata locorum
 * sola (trivia numquam), REFERENTIA non descensa, fons primus solus.
 * Visio: attributa sedes/octeti in elementis nodorum, lexematum et
 * triviorum; involucrum visio="sedes"; lector eam recusat; proiectio
 * sine visione intacta.
 */

#include "latina.h"
#include "credo.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

/* ... FORMA, G_*, GENERA, LEXICON, LOCI, GEN, REG, _lex as in
 * probatio_materia_arbor.c ... */

interior vacuum
_sedes_derivata (
                   vacuum* datum,
    constans MateriaToken* lexema,
             MateriaSedes* sedes)
{
    (vacuum)datum;
    sedes->byte_offset  = lexema->byte_offset;
    sedes->linea        = lexema->linea;
    sedes->columna      = lexema->columna;
    sedes->fons_index   = lexema->fons_index;
    sedes->est_fons     = FALSUM;
}

hic_manens constans MateriaOrigoUncus UNCUS_DERIVATUS = {
    NIHIL, _sedes_derivata, NIHIL, NIHIL
};

interior i32
_numerare (
    constans character* textus,
    constans character* forma)
{
    i32 numerus = ZEPHYRUM;
    constans character* p = strstr(textus, forma);

    dum (p != NIHIL)
    {
        numerus++;
        p = strstr(p + I, forma);
    }
    redde numerus;
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_materia_sedes",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));

    /* --- tractus lexematis --- */
    {
        MateriaTractus t;

        imprimere("\n--- tractus lexematis ---\n");
        materia_tractus_lexematis(NIHIL, _lex(piscina, (s32)G_IDENT,
            "abc", (s32)10, (i32)II, (i32)III), &t);
        CREDO_AEQUALIS_S32 (t.initium, (s32)10);
        CREDO_AEQUALIS_S32 (t.finis, (s32)13);
        CREDO_AEQUALIS_I32 (t.linea, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna, (i32)III);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)VI);
        CREDO_VERUM (t.est_fons);

        materia_tractus_lexematis(NIHIL, _lex(piscina, (s32)G_IDENT,
            "a\nbc", ZEPHYRUM, (i32)I, (i32)I), &t);
        CREDO_AEQUALIS_S32 (t.finis, (s32)IV);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)III);

        materia_tractus_lexematis(NIHIL, _lex(piscina,
            (s32)G_NOVA_LINEA, "\r\n", (s32)V, (i32)I, (i32)VI), &t);
        CREDO_AEQUALIS_S32 (t.finis, (s32)VII);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)I);

        /* syntheticum: sine sede */
        materia_tractus_lexematis(NIHIL, _lex(piscina, (s32)G_IDENT,
            "q", (s32)-I, (i32)I, (i32)I), &t);
        CREDO_AEQUALIS_S32 (t.initium, (s32)-I);

        /* derivatum: punctum in sede portata */
        materia_tractus_lexematis(&UNCUS_DERIVATUS, _lex(piscina,
            (s32)G_IDENT, "decoctum", (s32)20, (i32)III, (i32)IV), &t);
        CREDO_AEQUALIS_S32 (t.initium, (s32)20);
        CREDO_AEQUALIS_S32 (t.finis, (s32)20);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)IV);
        CREDO_FALSUM (t.est_fons);
    }

    /* --- tractus nodi --- */
    {
        MateriaNodus* nodus;
        MateriaNodus* filius;
        MateriaNodus* scopus;
        MateriaTractus t;

        imprimere("\n--- tractus nodi ---\n");
        filius = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, filius, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "yy",
                (s32)8, (i32)II, (i32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));
        nodus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "x",
                (s32)IV, (i32)I, (i32)V)),
            MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, nodus, ZEPHYRUM,
            materia_valor_nodus(filius), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_tractus_nodi(NIHIL, nodus, &t));
        CREDO_AEQUALIS_S32 (t.initium, (s32)IV);
        CREDO_AEQUALIS_S32 (t.finis, (s32)10);
        CREDO_AEQUALIS_I32 (t.linea, (i32)I);
        CREDO_AEQUALIS_I32 (t.columna, (i32)V);
        CREDO_AEQUALIS_I32 (t.linea_finis, (i32)II);
        CREDO_AEQUALIS_I32 (t.columna_finis, (i32)III);

        /* REFERENTIA non descensa */
        scopus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, scopus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "zzz",
                (s32)100, (i32)IX, (i32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));
        nodus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)II);
        CREDO_VERUM (materia_nodus_ponere(nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "a",
                ZEPHYRUM, (i32)I, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(nodus, (i32)I,
            materia_valor_referentia(scopus),
            MATERIA_LOCUS_REFERENTIA));
        CREDO_VERUM (materia_tractus_nodi(NIHIL, nodus, &t));
        CREDO_AEQUALIS_S32 (t.finis, (s32)I);

        /* nullum lexema cum sede */
        nodus = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "s",
                (s32)-I, (i32)I, (i32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_FALSUM (materia_tractus_nodi(NIHIL, nodus, &t));
        CREDO_AEQUALIS_S32 (t.initium, (s32)-I);
    }

    /* --- visio sedium --- */
    {
        MateriaArborConsilium c;
        MateriaArborScriptura s;
        MateriaArborScriptura nuda;
           MateriaArborVitium vitium;
                MateriaToken* x;
                MateriaToken* tr[3];
                MateriaNodus* n;
                          b32 admissi[1];
                   character* txt;

        imprimere("\n--- visio sedium ---\n");
        x = _lex(piscina, (s32)G_IDENT, "x", (s32)VII, (i32)I,
            (i32)VIII);
        tr[0] = _lex(piscina, (s32)G_SPATIA, "  ", ZEPHYRUM, (i32)I,
            (i32)I);
        tr[1] = _lex(piscina, (s32)G_COMMENTUM, "/*c*/", (s32)II,
            (i32)I, (i32)III);
        tr[2] = _lex(piscina, (s32)G_NOVA_LINEA, "\n", (s32)VIII,
            (i32)I, (i32)IX);
        CREDO_VERUM (materia_token_trivia_ante_ponere(x, piscina, tr,
            (i32)II));
        CREDO_VERUM (materia_token_trivia_post_ponere(x, piscina,
            tr + II, (i32)I));
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(x), MATERIA_LOCUS_LISTA_MIXTA));

        materia_arbor_consilium_nudum(&c, &REG, &ratum, "probatio");
        nuda = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (nuda.successus);
        CREDO_NIHIL (strstr((character*)nuda.textus.datum, "sedes="));
        CREDO_NIHIL (strstr((character*)nuda.textus.datum, "visio="));

        c.sedes_scribere = VERUM;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        txt = (character*)s.textus.datum;
        CREDO_NON_NIHIL (strstr(txt, "visio=\"sedes\""));
        /* nodus radix et lexema x: idem tractus (trivia exclusa) */
        CREDO_AEQUALIS_I32 (_numerare(txt, "sedes=\"1:8-1:9\""),
            (i32)II);
        CREDO_AEQUALIS_I32 (_numerare(txt, "octeti=\"7-8\""), (i32)II);
        CREDO_NON_NIHIL (strstr(txt, "octeti=\"0-2\""));
        CREDO_NON_NIHIL (strstr(txt, "sedes=\"1:3-1:8\""));
        CREDO_NON_NIHIL (strstr(txt, "octeti=\"2-7\""));
        CREDO_NON_NIHIL (strstr(txt, "sedes=\"1:9-2:1\""));

        /* lector visionem recusat */
        CREDO_NIHIL (materia_arbor_legere(piscina, NIHIL, s.textus, &c,
            &vitium));
        CREDO_NON_NIHIL (vitium.causa);
        si (vitium.causa != NIHIL)
        {
            CREDO_VERUM (strcmp(vitium.causa,
                "visio sedes: non arbor") == ZEPHYRUM);
        }

        /* cum filtro locorum */
        admissi[0] = VERUM;
        c.loci_admissi = admissi;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
            "visio=\"partialis sedes\""));

        /* lexema communicatum: transclusio sedem non iterat */
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(x), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(x), MATERIA_LOCUS_LISTA_MIXTA));
        c.loci_admissi = NIHIL;
        s = materia_arbor_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (_numerare((character*)s.textus.datum,
            "octeti=\"7-8\""), (i32)II);
    }

    praeteritus = credo_omnia_praeterierunt();
    credo_imprimere_compendium();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```

  Copy the tail (`credo_omnia_praeterierunt` / compendium / return) from
  the tail of `probatio_materia_arbor.c` exactly as that file ends it —
  the names above are the pattern, the file's own tail is the truth.

- [x] **Step 3: Run it red.** `./materia/compile_probationes.sh sedes` →
  build error (no `MateriaTractus`). Expected; the plant in Step 7 is the
  real red.

- [x] **Step 4: The range functions.** In `materia_nodus.h` after
  `materia_sedes_tokeni`:

```c
/* ==================================================
 * Tractus - extensio lexematis aut nodi in fonte
 * (materia-sedes-spec par. II)
 * ================================================== */

nomen structura {
    s32 initium;         /* octetus primus; -I = nulla sedes */
    s32 finis;           /* post octetum ultimum (EXCLUSIVUS) */
    i32 linea;           /* initii, I-basata */
    i32 columna;         /* initii, I-basata, octetis */
    i32 linea_finis;
    i32 columna_finis;   /* post octetum ultimum */
    s32 fons_index;
    b32 est_fons;        /* FALSUM = derivatum: punctum */
} MateriaTractus;

/* Tractus lexematis. Initium = sedes efficax (materia_sedes_tokeni).
 * Lexema FONTIS per octetos valoris promovetur (lex cursoris
 * lectoris: '\n' lineam novam columna I incipit, octetus alius
 * columnam addit). DERIVATUM punctum est (finis == initium): octeti
 * eius hic non iacent. Syntheticum (byte_offset -I): initium -I. */
vacuum
materia_tractus_lexematis (
    constans MateriaOrigoUncus* uncus,
         constans MateriaToken* token,
               MateriaTractus* tractus);

/* Tractum partis in summam conferre: minimum initii, maximum finis;
 * pars sine sede aut alius fontis quam summa praeteritur. inventum
 * FALSUM = summa nondum posita (pars eam ponit). */
vacuum
materia_tractus_conferre (
               MateriaTractus* summa,
                          b32* inventum,
    constans MateriaTractus* pars);

/* Tractus nodi: lexemata locorum (listae ordine; REFERENTIA non
 * descensa; trivia loci non sunt, ergo commentum ducens numquam
 * includitur), per materia_tractus_conferre. FALSUM = nullum lexema
 * cum sede (tractus->initium -I). */
b32
materia_tractus_nodi (
    constans MateriaOrigoUncus* uncus,
        constans MateriaNodus* nodus,
              MateriaTractus* tractus);
```

  In `materia_nodus.c` after `materia_sedes_tokeni`:

```c
vacuum
materia_tractus_lexematis (
    constans MateriaOrigoUncus* uncus,
         constans MateriaToken* token,
               MateriaTractus* tractus)
{
    MateriaSedes sedes;
             i32 i;

    si (tractus == NIHIL)
    {
        redde;
    }
    materia_sedes_tokeni(uncus, token, &sedes);
    tractus->initium        = sedes.byte_offset;
    tractus->finis          = sedes.byte_offset;
    tractus->linea          = sedes.linea;
    tractus->columna        = sedes.columna;
    tractus->linea_finis    = sedes.linea;
    tractus->columna_finis  = sedes.columna;
    tractus->fons_index     = sedes.fons_index;
    tractus->est_fons       = sedes.est_fons;
    si (   token == NIHIL || sedes.byte_offset < ZEPHYRUM
        || !sedes.est_fons)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < token->valor.mensura; i++)
    {
        si (token->valor.datum[i] == (i8)'\n')
        {
            tractus->linea_finis++;
            tractus->columna_finis = I;
        }
        alioquin
        {
            tractus->columna_finis++;
        }
    }
    tractus->finis = sedes.byte_offset + (s32)token->valor.mensura;
}

vacuum
materia_tractus_conferre (
               MateriaTractus* summa,
                          b32* inventum,
    constans MateriaTractus* pars)
{
    si (summa == NIHIL || inventum == NIHIL || pars == NIHIL
        || pars->initium < ZEPHYRUM)
    {
        redde;
    }
    si (!*inventum)
    {
        *summa     = *pars;
        *inventum  = VERUM;
        redde;
    }
    si (pars->fons_index != summa->fons_index)
    {
        redde;
    }
    si (pars->initium < summa->initium)
    {
        summa->initium  = pars->initium;
        summa->linea    = pars->linea;
        summa->columna  = pars->columna;
    }
    si (pars->finis > summa->finis)
    {
        summa->finis          = pars->finis;
        summa->linea_finis    = pars->linea_finis;
        summa->columna_finis  = pars->columna_finis;
    }
}

interior vacuum
_tractum_valoris_conferre (
    constans MateriaOrigoUncus* uncus,
          constans MateriaValor* valor,
               MateriaTractus* summa,
                          b32* inventum)
{
    MateriaTractus pars;
               i32 k;

    commutatio (valor->genus)
    {
    casus MATERIA_VALOR_TOKEN:
        materia_tractus_lexematis(uncus, valor->datum.token, &pars);
        materia_tractus_conferre(summa, inventum, &pars);
        frange;
    casus MATERIA_VALOR_NODUS:
        si (valor->datum.nodus != NIHIL)
        {
            per (k = ZEPHYRUM;
                 k < valor->datum.nodus->numerus_locorum; k++)
            {
                _tractum_valoris_conferre(uncus,
                    &valor->datum.nodus->loci[k], summa, inventum);
            }
        }
        frange;
    casus MATERIA_VALOR_LISTA:
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
        {
            constans MateriaValor* e =
                materia_valor_lista_obtinere(*valor, k);

            si (e != NIHIL)
            {
                _tractum_valoris_conferre(uncus, e, summa, inventum);
            }
        }
        frange;
    ordinarius:
        /* NIHIL, INDEX, REFERENTIA (scopus alibi possidetur) */
        frange;
    }
}

b32
materia_tractus_nodi (
    constans MateriaOrigoUncus* uncus,
        constans MateriaNodus* nodus,
              MateriaTractus* tractus)
{
    b32 inventum = FALSUM;
    i32 k;

    si (tractus == NIHIL)
    {
        redde FALSUM;
    }
    memset(tractus, ZEPHYRUM, magnitudo(*tractus));
    tractus->initium     = (s32)-I;
    tractus->finis       = (s32)-I;
    tractus->fons_index  = (s32)-I;
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _tractum_valoris_conferre(uncus, &nodus->loci[k], tractus,
            &inventum);
    }
    redde inventum;
}
```

  (`materia_nodus.c` needs `#include <string.h>` for `memset` if it
  lacks it.)

- [x] **Step 5: The consilium field.** In `materia_arbor.h`, after
  `MateriaTokenForma forma;` (line 320):

```c
    /* SEDES (2026-09-17, materia-sedes-spec par. III): VERUM = VISIO
     * sedium - elementum quodque nodi, lexematis et trivii attributa
     * sedes="L:C-L:C" et octeti="B-B" fert, involucrum visio="sedes";
     * lector eam recusat, canones numquam iudicant. Campus ULTIMUS. */
                                 b32  sedes_scribere;
```

- [x] **Step 6: The writer.**
  1. `structura MateriaArborScriptor` (after `ancora_initium_lineae`,
     line 66):

```c
    /* SEDES: tractus nodi currentis per ambulationem cumulatus - nodus
     * quisque parentis suum servat et suum in eum confert */
                     MateriaTractus  tractus;
                                b32  tractus_inventus;
```

  2. Before `_trivium_scribere` (line 883), a helper:

```c
/* Attributa visionis sedium (materia-sedes-spec par. III): tractus
 * sine sede nihil scribit. */
interior b32
_tractum_scribere (
       MateriaArborScriptor* st,
                  StmlNodus* elementum,
    constans MateriaTractus* tractus)
{
    ChordaAedificator* sedes;
    ChordaAedificator* octeti;

    si (tractus->initium < ZEPHYRUM)
    {
        redde VERUM;
    }
    sedes   = chorda_aedificator_creare(st->piscina,
        (memoriae_index)XLVIII);
    octeti  = chorda_aedificator_creare(st->piscina,
        (memoriae_index)XXXII);
    si (sedes == NIHIL || octeti == NIHIL)
    {
        st->causa = "sedes scribi non potuerunt";
        redde FALSUM;
    }
    chorda_aedificator_appendere_i32(sedes, tractus->linea);
    chorda_aedificator_appendere_character(sedes, ':');
    chorda_aedificator_appendere_i32(sedes, tractus->columna);
    chorda_aedificator_appendere_character(sedes, '-');
    chorda_aedificator_appendere_i32(sedes, tractus->linea_finis);
    chorda_aedificator_appendere_character(sedes, ':');
    chorda_aedificator_appendere_i32(sedes, tractus->columna_finis);
    chorda_aedificator_appendere_i32(octeti, (i32)tractus->initium);
    chorda_aedificator_appendere_character(octeti, '-');
    chorda_aedificator_appendere_i32(octeti, (i32)tractus->finis);
    si (   !stml_attributum_addere_chorda(elementum, st->piscina,
                st->intern, "sedes", chorda_aedificator_finire(sedes))
        || !stml_attributum_addere_chorda(elementum, st->piscina,
                st->intern, "octeti",
                chorda_aedificator_finire(octeti)))
    {
        st->causa = "sedes scribi non potuerunt";
        redde FALSUM;
    }
    redde VERUM;
}
```

  3. `_trivium_scribere`: replace the final `redde elementum;` with:

```c
    si (st->consilium->sedes_scribere)
    {
        MateriaTractus t;

        materia_tractus_lexematis(st->consilium->origo, trivium, &t);
        si (!_tractum_scribere(st, elementum, &t))
        {
            redde NIHIL;
        }
    }
    redde elementum;
```

  4. `_scribere_lexema`: right after `nota = _nota_lexematis(st, lexema);`
     (line 1185) — every use counts toward the owning node's range, the
     transclusion too:

```c
    si (st->consilium->sedes_scribere)
    {
        MateriaTractus t;

        materia_tractus_lexematis(st->consilium->origo, lexema, &t);
        materia_tractus_conferre(&st->tractus, &st->tractus_inventus,
            &t);
    }
```

     and after the "SEDES PORTATA" block (after line 1372, before the
     frons's `liberos_ornare`):

```c
    si (st->consilium->sedes_scribere)
    {
        MateriaTractus t;

        materia_tractus_lexematis(st->consilium->origo, lexema, &t);
        si (!_tractum_scribere(st, elementum, &t))
        {
            redde NIHIL;
        }
    }
```

  5. `_scribere_nodum_internum`: add locals
     `MateriaTractus tractus_parentis;` and `b32 inventus_parentis;`;
     after the frons `nodum_ornare` block (line 1736) and before the loci
     loop:

```c
    tractus_parentis        = st->tractus;
    inventus_parentis       = st->tractus_inventus;
    st->tractus_inventus    = FALSUM;
```

     and replace the final `redde elementum;` (line 1793) with:

```c
    si (st->consilium->sedes_scribere && st->tractus_inventus)
    {
        si (!_tractum_scribere(st, elementum, &st->tractus))
        {
            st->sedes = nodus;
            redde NIHIL;
        }
        materia_tractus_conferre(&tractus_parentis, &inventus_parentis,
            &st->tractus);
    }
    st->tractus           = tractus_parentis;
    st->tractus_inventus  = inventus_parentis;
    redde elementum;
```

  6. `_arborem_struere`: after `st.ancora_initium_lineae = FALSUM;`
     (line 1885):

```c
    memset(&st.tractus, ZEPHYRUM, magnitudo(st.tractus));
    st.tractus.initium    = (s32)-I;
    st.tractus_inventus   = FALSUM;
```

     and replace the VISIO block (lines 1923-1964) so the value names
     both kinds; the `partialis` bytes stay identical (same attribute
     order: `visio`, then `omissi`):

```c
    /* VISIO: proiectio filtrata aut sedibus ornata se ipsam profitetur
     * - lector recusat */
    si (consilium->loci_admissi != NIHIL || consilium->sedes_scribere)
    {
        ChordaAedificator* omissi = NIHIL;
        constans character* visio = consilium->loci_admissi == NIHIL
            ? "sedes"
            : (consilium->sedes_scribere ? "partialis sedes"
                                         : "partialis");

        si (consilium->loci_admissi != NIHIL)
        {
            i32 g;

            omissi = chorda_aedificator_creare(piscina,
                (memoriae_index)CCLVI);
            si (omissi == NIHIL)
            {
                fructus.causa = "omissi scribi non potuerunt";
                redde fructus;
            }
            /* ... the existing per-genus / per-locus loop, unchanged ... */
        }
        stml_attributum_addere(involucrum, piscina, intern, "visio",
            visio);
        si (omissi != NIHIL)
        {
            stml_attributum_addere_chorda(involucrum, piscina, intern,
                "omissi", chorda_aedificator_finire(omissi));
        }
    }
```

  7. Reader (lines 3641-3648): replace with

```c
    /* VISIO: proiectio filtrata aut sedibus ornata documentum canonicum
     * non est - lector eam numquam in arborem legit */
    attributum = stml_attributum_capere(involucrum, "visio");
    si (attributum != NIHIL)
    {
        constans character* causa = "visio ignota: non arbor";

        si (chorda_aequalis_literis(*attributum, "partialis"))
        {
            causa = "visio partialis: non arbor";
        }
        alioquin si (chorda_aequalis_literis(*attributum, "sedes"))
        {
            causa = "visio sedes: non arbor";
        }
        alioquin si (chorda_aequalis_literis(*attributum,
                     "partialis sedes"))
        {
            causa = "visio partialis sedes: non arbor";
        }
        materia_arbor_lector_recusare(&lector, causa, involucrum->linea);
        redde NIHIL;
    }
```

- [x] **Step 7: Run green, then plant.** `./materia/compile_probationes.sh
  sedes` → all pass. Plant:
  `silva.planta('materia/fontes/materia_nodus.c', 'tractus->finis =
  sedes.byte_offset + (s32)token->valor.mensura;', 'tractus->finis =
  sedes.byte_offset + (s32)token->valor.mensura + (s32)I;', 'materia',
  'sedes')` → red (range asserts), then green on revert.

- [x] **Step 8: Docs.** `materia/fontes/materia_arbor.worklog.md` (new,
  dated entry: the view, the bottom-up range accumulator, D1, D2).
  `materia/CLAUDE.md`, new section after "Visio":

```markdown
## Sedes (2026-09-17)

**A positioned projection is a view.** `consilium.sedes_scribere` stamps
`sedes="L:C-L:C"` (1-based lines, byte columns, end exclusive) and
`octeti="B-B"` on every node, token and trivium element; locus wrappers
never. A node's range runs over its loci tokens (trivia are not loci),
accumulated bottom-up in the writer's one walk. A DERIVED token (md,
oratio) is a zero-width point. The envelope says `visio="sedes"` (or
`"partialis sedes"`), the reader refuses it, canons never see it. Range
API: `materia_tractus_lexematis` / `_nodi` / `_conferre`
(`materia_nodus.h`). Spec `project-specs/materia-sedes-spec.md`.
```

- [x] **Step 9: Format, words, audit, commit.** `./silva/formator.sh`
  on the four C files and the probatio (`-scribere`, then `-vitia`);
  `./oratio/quaere.sh tractus conferre inventum octeti visio numerare`;
  `./silva/nexus.sh materia_tractus_nodi`; `./materia/shim_probare.sh`
  (0 fractae). Commit via `silva.commissio_umbra` with viae = the seven
  files above and portae `['materia', 'css', 'md', 'html', 'oratio',
  'crusta', 'briar']`. Message: `materia: visio sedium A1 - …` (range,
  view, envelope, reader, gate, plant, D1/D2).

---

## Task A2: The shared verifier, five client gates, `-sedes` instruments

**Executed 2026-09-17.** Verifier green at first run on every client: crusta 339,503 elements, css 9,679, html 16,439, md 2,491,843, oratio 2,643,746; zero derived points (md/oratio derived tokens are synthetic, offset −1). View ×2.47 the plain projection over 237 house `.sh`. Plant writer column `+ I` → crusta stml red at `sedes.sana`.

**Files:**
- Create: `materia/fontes/materia_sedes.h`, `materia/fontes/materia_sedes.c`
- Modify: `crusta/compile_probationes.sh:131`, `css/compile_probationes.sh:131`,
  `md/compile_probationes.sh`, `html/compile_probationes.sh`,
  `oratio/compile_probationes.sh` (add `materia_sedes` to each
  `for m in materia_… ; do` module list)
- Modify: `crusta/probationes/probatio_crusta_stml.c` (`_circuitum_probare`, 438),
  `css/probationes/probatio_css_stml.c` (92), `md/probationes/probatio_md_stml.c` (70),
  `html/probationes/probatio_html_stml.c` (320),
  `oratio/probationes/probatio_oratio_stml.c` (`_circuitum_arboris`, 66)
- Modify: `crusta/instrumenta/arbor.c`, `html/instrumenta/arbor.c`,
  `md/instrumenta/arbor.c`, `oratio/instrumenta/arbor.c` (`-sedes`)
- Modify: `materia/fontes/materia_arbor.worklog.md`

**Interfaces:**
- Consumes: A1's `MateriaTractus`, `materia_tractus_lexematis`,
  `sedes_scribere`, `materia_arbor_proicere_nodum` (arbor tradita),
  `materia_arbor_legere`.
- Produces: `MateriaSedesRelatio {sana, elementa, lexemata, derivata,
  nodi, causa, index}`;
  `b32 materia_sedes_verificare(Piscina*, constans MateriaNodus* radix,
  constans MateriaArborConsilium*, constans character* fons, i32 mensura,
  MateriaSedesRelatio*)`.

- [x] **Step 1: The verifier** (`./silva/scribe.sh`). The oracle is
  independent of the writer's arithmetic: lines and columns come from a
  LINE TABLE the verifier builds from the source bytes; token ranges are
  checked against the source slice; node ranges come from the verifier's
  own walk; elements are paired by DOCUMENT ORDER (pre-order over the
  STML tree, elements that carry `octeti`) against the tree walk in the
  writer's order (node, then for a first-use token: token, its `ante`
  trivia, its `post` trivia; repeated uses are transclusions and add no
  element but still count toward the node's range).

```c
/* materia_sedes.h - Verificatio visionis sedium (porta clientium)
 *
 * Una functio quam probatio STML cuiusque clientis super corpus suum
 * vocat (exemplar: materia_registrum_recens). ORACULUM a scriptore
 * DISIUNCTUM: lineae et columnae ex tabula linearum fontis ipsius,
 * octeti lexematum contra segmentum fontis, tractus nodorum per
 * ambulationem propriam, elementa ordine documenti paria.
 *
 * Quod iudicatur: (1) visio a proiectione nuda NIHIL differt praeter
 * attributa sedes/octeti et 'visio' involucri; (2) elementum quodque
 * sedibus ornatum tractui exspectato congruit, ordine; (3) lector
 * visionem recusat. Recusatur (causa nominata): consilium cum
 * loci_admissi, templa_activa, aut fronte liberos ornante.
 */
#ifndef MATERIA_SEDES_H
#define MATERIA_SEDES_H

#include "latina.h"
#include "piscina.h"
#include "materia_arbor.h"

nomen structura {
                    b32  sana;
                    i32  elementa;   /* elementa sedibus ornata visa */
                    i32  lexemata;   /* lexemata fontis cum segmento */
                    i32  derivata;   /* lexemata derivata (puncta) */
                    i32  nodi;       /* nodi tractu ornati */
    constans character*  causa;      /* prima; NIHIL si sana */
                    i32  index;      /* ordinalis elementi divergentis */
} MateriaSedesRelatio;

/* VERUM = sana. relatio semper impletur. */
b32
materia_sedes_verificare (
                           Piscina* piscina,
             constans MateriaNodus* radix,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
               MateriaSedesRelatio* relatio);

#endif /* MATERIA_SEDES_H */
```

```c
/* materia_sedes.c - Vide materia_sedes.h. */

#include "materia_sedes.h"
#include "materia_nodus.h"
#include "stml.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include <string.h>

nomen structura {
    constans MateriaToken* lexema;   /* NIHIL = nodus */
            MateriaTractus tractus;
} Exspectatum;

nomen structura {
                     Piscina* piscina;
    constans MateriaOrigoUncus* uncus;
                         Xar* exspectata;   /* Exspectatum */
                         Xar* lineae;       /* s32: initia linearum */
              TabulaDispersa* visa;         /* lexemata iam scripta */
                         b32  memoria_defecit;
} Verificatio;

interior b32
_recusare (
    MateriaSedesRelatio* relatio,
    constans character* causa,
                   i32  index)
{
    si (relatio->causa == NIHIL)
    {
        relatio->causa  = causa;
        relatio->index  = index;
    }
    relatio->sana = FALSUM;
    redde FALSUM;
}

interior Xar*
_lineas_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    Xar* lineae = xar_creare(piscina, (i32)magnitudo(s32));
    s32* cella;
    i32  i;

    si (lineae == NIHIL)
    {
        redde NIHIL;
    }
    cella = (s32*)xar_addere(lineae);
    si (cella == NIHIL)
    {
        redde NIHIL;
    }
    *cella = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (fons[i] == '\n')
        {
            cella = (s32*)xar_addere(lineae);
            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = (s32)(i + I);
        }
    }
    redde lineae;
}

/* linea et columna ex octeto per tabulam linearum (quaesitio bina) */
interior vacuum
_sedem_computare (
     Xar* lineae,
     s32  octetus,
    i32* linea,
    i32* columna)
{
    i32 ima    = ZEPHYRUM;
    i32 summa  = xar_numerus(lineae) - I;

    dum (ima < summa)
    {
        i32 media = (ima + summa + I) / II;

        si (*(s32*)xar_obtinere(lineae, media) <= octetus)
        {
            ima = media;
        }
        alioquin
        {
            summa = media - I;
        }
    }
    *linea    = ima + I;
    *columna  = (i32)(octetus - *(s32*)xar_obtinere(lineae, ima)) + I;
}

interior b32
_visum_notare (
            Verificatio* v,
    constans MateriaToken* lexema)
{
    chorda clavis;
       i8* octeti;

    octeti = (i8*)piscina_allocare(v->piscina,
        magnitudo(constans MateriaToken*));
    si (octeti == NIHIL)
    {
        v->memoria_defecit = VERUM;
        redde VERUM;
    }
    memcpy(octeti, &lexema, magnitudo(constans MateriaToken*));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(constans MateriaToken*);
    si (tabula_dispersa_continet(v->visa, clavis))
    {
        redde VERUM;
    }
    (vacuum)tabula_dispersa_inserere(v->visa, clavis, (vacuum*)VERUM);
    redde FALSUM;
}

/* tractus exspectatus lexematis: initium ex sede efficaci, lineae et
 * columnae ex TABULA (fontis) aut portatae (derivati) */
interior MateriaTractus
_tractus_lexematis (
            Verificatio* v,
    constans MateriaToken* lexema)
{
    MateriaTractus t;

    materia_tractus_lexematis(v->uncus, lexema, &t);
    si (t.initium >= ZEPHYRUM && t.est_fons)
    {
        t.finis = t.initium + (s32)lexema->valor.mensura;
        _sedem_computare(v->lineae, t.initium, &t.linea, &t.columna);
        _sedem_computare(v->lineae, t.finis, &t.linea_finis,
            &t.columna_finis);
    }
    redde t;
}

interior vacuum
_exspectatum_addere (
             Verificatio* v,
    constans MateriaToken* lexema,
    constans MateriaTractus* t)
{
    Exspectatum* e;

    si (t->initium < ZEPHYRUM)
    {
        redde;
    }
    e = (Exspectatum*)xar_addere(v->exspectata);
    si (e == NIHIL)
    {
        v->memoria_defecit = VERUM;
        redde;
    }
    e->lexema   = lexema;
    e->tractus  = *t;
}

/* summa nodi: minimum initii, maximum finis, fons primus solus -
 * scripta hic SEORSUM a materia_tractus_conferre (oraculum disiunctum) */
interior vacuum
_summam_augere (
      MateriaTractus* summa,
                b32* inventum,
    constans MateriaTractus* t)
{
    si (t->initium < ZEPHYRUM)
    {
        redde;
    }
    si (!*inventum)
    {
        *summa = *t;
        *inventum = VERUM;
        redde;
    }
    si (t->fons_index != summa->fons_index)
    {
        redde;
    }
    si (t->initium < summa->initium)
    {
        summa->initium  = t->initium;
        summa->linea    = t->linea;
        summa->columna  = t->columna;
    }
    si (t->finis > summa->finis)
    {
        summa->finis          = t->finis;
        summa->linea_finis    = t->linea_finis;
        summa->columna_finis  = t->columna_finis;
    }
}

interior vacuum
_valorem_ambulare (
             Verificatio* v,
    constans MateriaValor* valor,
          MateriaTractus* summa,
                     b32* inventum);

interior vacuum
_nodum_ambulare (
             Verificatio* v,
    constans MateriaNodus* nodus,
          MateriaTractus* summa_parentis,
                     b32* inventum_parentis)
{
    MateriaTractus summa;
               b32 inventum = FALSUM;
               i32 locus_exspectati;
               i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    /* locus in serie ANTE liberos (ordo documenti), postea impletur */
    locus_exspectati = xar_numerus(v->exspectata);
    si (xar_addere(v->exspectata) == NIHIL)
    {
        v->memoria_defecit = VERUM;
        redde;
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_ambulare(v, &nodus->loci[k], &summa, &inventum);
    }
    {
        Exspectatum* e = (Exspectatum*)xar_obtinere(v->exspectata,
            locus_exspectati);

        e->lexema = NIHIL;
        si (inventum)
        {
            e->tractus = summa;
            _summam_augere(summa_parentis, inventum_parentis, &summa);
        }
        alioquin
        {
            e->tractus.initium = (s32)-I;   /* nodus sine sede */
        }
    }
}

interior vacuum
_lexema_ambulare (
             Verificatio* v,
    constans MateriaToken* lexema,
          MateriaTractus* summa,
                     b32* inventum)
{
    MateriaTractus t;
               i32 k;

    si (lexema == NIHIL)
    {
        redde;
    }
    t = _tractus_lexematis(v, lexema);
    _summam_augere(summa, inventum, &t);
    si (_visum_notare(v, lexema))
    {
        redde;   /* usus iteratus: transclusio, elementum nullum */
    }
    _exspectatum_addere(v, lexema, &t);
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        MateriaTractus a = _tractus_lexematis(v, lexema->spatia_ante[k]);

        _exspectatum_addere(v, lexema->spatia_ante[k], &a);
    }
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        MateriaTractus p = _tractus_lexematis(v, lexema->spatia_post[k]);

        _exspectatum_addere(v, lexema->spatia_post[k], &p);
    }
}

interior vacuum
_valorem_ambulare (
             Verificatio* v,
    constans MateriaValor* valor,
          MateriaTractus* summa,
                     b32* inventum)
{
    i32 k;

    commutatio (valor->genus)
    {
    casus MATERIA_VALOR_TOKEN:
        _lexema_ambulare(v, valor->datum.token, summa, inventum);
        frange;
    casus MATERIA_VALOR_NODUS:
        _nodum_ambulare(v, valor->datum.nodus, summa, inventum);
        frange;
    casus MATERIA_VALOR_LISTA:
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
        {
            constans MateriaValor* e =
                materia_valor_lista_obtinere(*valor, k);

            si (e != NIHIL)
            {
                _valorem_ambulare(v, e, summa, inventum);
            }
        }
        frange;
    ordinarius:
        frange;
    }
}
```

  Then, in the same file, the document-side helpers:

```c
/* elementa sedibus ornata ordine documenti (praeordine) */
interior vacuum
_elementa_colligere (
          Xar* acta,
    StmlNodus* nodus)
{
    i32 k;

    si (nodus == NIHIL || nodus->genus != STML_NODUS_ELEMENTUM)
    {
        redde;
    }
    si (stml_attributum_capere(nodus, "octeti") != NIHIL)
    {
        StmlNodus** cella = (StmlNodus**)xar_addere(acta);

        si (cella != NIHIL)
        {
            *cella = nodus;
        }
    }
    si (nodus->liberi == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nodus->liberi); k++)
    {
        _elementa_colligere(acta,
            *(StmlNodus**)xar_obtinere(nodus->liberi, k));
    }
}

/* numeri decimales ex chorda (separator quilibet non cifra);
 * reddit numerum lectorum, ad capacitatem */
interior i32
_numeros_legere (
    constans chorda* valor,
               i32* numeri,
               i32  capacitas)
{
    i32 lecti  = ZEPHYRUM;
    i32 i      = ZEPHYRUM;

    si (valor == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum (i < valor->mensura && lecti < capacitas)
    {
        si (valor->datum[i] >= (i8)'0' && valor->datum[i] <= (i8)'9')
        {
            i32 n = ZEPHYRUM;

            dum (   i < valor->mensura
                 && valor->datum[i] >= (i8)'0'
                 && valor->datum[i] <= (i8)'9')
            {
                n = n * X + (i32)(valor->datum[i] - (i8)'0');
                i++;
            }
            numeri[lecti] = n;
            lecti++;
        }
        alioquin
        {
            i++;
        }
    }
    redde lecti;
}

interior b32
_chordae_pares (
    constans chorda* a,
    constans chorda* b)
{
    si (a == NIHIL || b == NIHIL)
    {
        redde (b32)(a == b);
    }
    redde chorda_aequalis(*a, *b);
}

/* attributum visionis: sedes/octeti ubique, 'visio' in radice */
interior b32
_attributum_visionis (
    constans chorda* titulus,
                b32  radix)
{
    redde (b32)(   chorda_aequalis_literis(*titulus, "sedes")
                || chorda_aequalis_literis(*titulus, "octeti")
                || (radix && chorda_aequalis_literis(*titulus,
                                 "visio")));
}

/* index attributi proximi quod visionis non est; numerus si nullum */
interior i32
_attributum_proximum (
    Xar* attributa,
    i32  ab,
    b32  radix)
{
    si (attributa == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum (ab < xar_numerus(attributa))
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(attributa, ab);

        si (   a != NIHIL && a->titulus != NIHIL
            && !_attributum_visionis(a->titulus, radix))
        {
            redde ab;
        }
        ab++;
    }
    redde xar_numerus(attributa);
}

/* proiectio nuda et visio aequales praeter attributa visionis */
interior b32
_arbores_aequales (
    StmlNodus* nuda,
    StmlNodus* visio,
          b32  radix)
{
    i32 ia;
    i32 ib;
    i32 na;
    i32 nb;
    i32 k;

    si (nuda == NIHIL || visio == NIHIL)
    {
        redde (b32)(nuda == visio);
    }
    si (   nuda->genus != visio->genus
        || !_chordae_pares(nuda->titulus, visio->titulus)
        || !_chordae_pares(nuda->valor, visio->valor)
        || nuda->crudus != visio->crudus
        || nuda->fragmentum != visio->fragmentum
        || !_chordae_pares(nuda->fragmentum_id, visio->fragmentum_id))
    {
        redde FALSUM;
    }
    na = nuda->attributa  != NIHIL ? xar_numerus(nuda->attributa)
                                   : ZEPHYRUM;
    nb = visio->attributa != NIHIL ? xar_numerus(visio->attributa)
                                   : ZEPHYRUM;
    ia = _attributum_proximum(nuda->attributa, ZEPHYRUM, radix);
    ib = _attributum_proximum(visio->attributa, ZEPHYRUM, radix);
    dum (ia < na && ib < nb)
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(
            nuda->attributa, ia);
        StmlAttributum* b = (StmlAttributum*)xar_obtinere(
            visio->attributa, ib);

        si (   !_chordae_pares(a->titulus, b->titulus)
            || !_chordae_pares(a->valor, b->valor))
        {
            redde FALSUM;
        }
        ia = _attributum_proximum(nuda->attributa, ia + I, radix);
        ib = _attributum_proximum(visio->attributa, ib + I, radix);
    }
    si (ia < na || ib < nb)
    {
        redde FALSUM;
    }
    na = nuda->liberi  != NIHIL ? xar_numerus(nuda->liberi)  : ZEPHYRUM;
    nb = visio->liberi != NIHIL ? xar_numerus(visio->liberi) : ZEPHYRUM;
    si (na != nb)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < na; k++)
    {
        si (!_arbores_aequales(
                *(StmlNodus**)xar_obtinere(nuda->liberi, k),
                *(StmlNodus**)xar_obtinere(visio->liberi, k), FALSUM))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
```

  and the entry:

```c
b32
materia_sedes_verificare (
                           Piscina* piscina,
             constans MateriaNodus* radix,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
               MateriaSedesRelatio* relatio)
{
     MateriaArborConsilium  nudum;
     MateriaArborConsilium  ornatum;
     MateriaArborScriptura  sn;
     MateriaArborScriptura  so;
               Verificatio  v;
            MateriaTractus  summa;
                       b32  inventum = FALSUM;
                      Xar*  acta;
        MateriaArborVitium  vitium;
                       i32  k;

    memset(relatio, ZEPHYRUM, magnitudo(*relatio));
    relatio->sana = VERUM;
    si (piscina == NIHIL || radix == NIHIL || consilium == NIHIL
        || fons == NIHIL)
    {
        redde _recusare(relatio, "argumenta nihil", ZEPHYRUM);
    }
    si (consilium->loci_admissi != NIHIL)
    {
        redde _recusare(relatio,
            "visio partialis: verificatio sedium non tractat", ZEPHYRUM);
    }
    si (consilium->templa_activa)
    {
        redde _recusare(relatio,
            "templa activa: spatia sine elemento", ZEPHYRUM);
    }
    si (consilium->frons != NIHIL
        && consilium->frons->liberos_ornare != NIHIL)
    {
        redde _recusare(relatio,
            "frons liberos ornat: ordo elementorum ignotus", ZEPHYRUM);
    }

    nudum = *consilium;
    nudum.sedes_scribere = FALSUM;
    ornatum = *consilium;
    ornatum.sedes_scribere = VERUM;
    sn = materia_arbor_proicere_nodum(piscina, radix, &nudum);
    so = materia_arbor_proicere_nodum(piscina, radix, &ornatum);
    si (!sn.successus || !so.successus)
    {
        redde _recusare(relatio, "proiectio fracta", ZEPHYRUM);
    }
    si (!_arbores_aequales(sn.arbor, so.arbor, VERUM))
    {
        redde _recusare(relatio,
            "visio a proiectione nuda differt", ZEPHYRUM);
    }

    memset(&v, ZEPHYRUM, magnitudo(v));
    v.piscina     = piscina;
    v.uncus       = consilium->origo;
    v.exspectata  = xar_creare(piscina, (i32)magnitudo(Exspectatum));
    v.lineae      = _lineas_legere(piscina, fons, mensura);
    v.visa        = tabula_dispersa_creare_chorda(piscina, 1024);
    acta          = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    si (v.exspectata == NIHIL || v.lineae == NIHIL || v.visa == NIHIL
        || acta == NIHIL)
    {
        redde _recusare(relatio, "memoria deficit", ZEPHYRUM);
    }
    _nodum_ambulare(&v, radix, &summa, &inventum);
    _elementa_colligere(acta, so.arbor);
    si (v.memoria_defecit)
    {
        redde _recusare(relatio, "memoria deficit", ZEPHYRUM);
    }

    /* nodi sine sede elementum sine octeti habent: praetereuntur */
    {
        i32 j = ZEPHYRUM;

        per (k = ZEPHYRUM; k < xar_numerus(v.exspectata); k++)
        {
            constans Exspectatum* e =
                (constans Exspectatum*)xar_obtinere(v.exspectata, k);
            StmlNodus* el;
                   i32 numeri[IV];

            si (e->tractus.initium < ZEPHYRUM)
            {
                perge;
            }
            si (j >= xar_numerus(acta))
            {
                redde _recusare(relatio, "elementa pauciora exspectatis",
                    j);
            }
            el = *(StmlNodus**)xar_obtinere(acta, j);
            relatio->elementa++;
            si (   _numeros_legere(stml_attributum_capere(el, "octeti"),
                       numeri, (i32)II) != II
                || (s32)numeri[0] != e->tractus.initium
                || (s32)numeri[1] != e->tractus.finis)
            {
                redde _recusare(relatio, "octeti dispares", j);
            }
            si (   _numeros_legere(stml_attributum_capere(el, "sedes"),
                       numeri, (i32)IV) != IV
                || numeri[0] != e->tractus.linea
                || numeri[1] != e->tractus.columna
                || numeri[2] != e->tractus.linea_finis
                || numeri[3] != e->tractus.columna_finis)
            {
                redde _recusare(relatio, "sedes dispares", j);
            }
            si (e->lexema == NIHIL)
            {
                relatio->nodi++;
            }
            alioquin si (!e->tractus.est_fons)
            {
                relatio->derivata++;
            }
            alioquin
            {
                si (   e->tractus.finis > (s32)mensura
                    || (e->lexema->valor.mensura > ZEPHYRUM
                        && memcmp(fons + e->tractus.initium,
                               e->lexema->valor.datum,
                               (size_t)e->lexema->valor.mensura)
                           != ZEPHYRUM))
                {
                    redde _recusare(relatio,
                        "segmentum fontis valori dispar", j);
                }
                relatio->lexemata++;
            }
            j++;
        }
        si (j != xar_numerus(acta))
        {
            redde _recusare(relatio, "elementa plura exspectatis", j);
        }
    }

    /* lector visionem recusat */
    {
        chorda textus = stml_scribere(so.arbor, piscina, VERUM);

        si (   materia_arbor_legere(piscina, NIHIL, textus, consilium,
                   &vitium) != NIHIL
            || vitium.causa == NIHIL
            || strncmp(vitium.causa, "visio", (size_t)V) != ZEPHYRUM)
        {
            redde _recusare(relatio, "lector visionem non recusavit",
                ZEPHYRUM);
        }
    }
    redde relatio->sana;
}
```

  (`_numeros_legere` must treat a NIHIL chorda as zero numbers. The
  `numeri` values are `i32`; offsets fit, since sources are < 2 GiB.)

- [x] **Step 2: Wire the five gates.** In each function named above,
  immediately after the FIRST successful `materia_arbor_scribere_nodum`
  (the `s1` check) insert, adapting only the tree variable (`radix` in
  crusta/css/md/html/oratio — oratio receives it as a parameter):

```c
    {
        MateriaSedesRelatio sedes;

        si (!materia_sedes_verificare(piscina, radix, consilium, fons,
                mensura, &sedes))
        {
            imprimere("    (sedes: %s, elementum %d)\n",
                sedes.causa ? sedes.causa : "-", (integer)sedes.index);
        }
        CREDO_VERUM (sedes.sana);
        SEDES_ELEMENTA  += sedes.elementa;
        SEDES_DERIVATA  += sedes.derivata;
    }
```

  with file-scope `hic_manens i32 SEDES_ELEMENTA = 0;` and
  `hic_manens i32 SEDES_DERIVATA = 0;`, `#include "materia_sedes.h"`, and
  one line printed where the gate prints its corpus summary:
  `imprimere("  sedes: %d elementa, %d derivata\n", …)` followed by
  `CREDO_VERUM (SEDES_ELEMENTA > ZEPHYRUM);` (a gate that verified
  nothing is dead). If a client fails with a named class, STOP and bring
  the class to Fran — never exempt.

- [x] **Step 3: Instruments.** In each of the four `arbor.c`, parse
  `-sedes` beside `-tacitus` into `b32 sedes = FALSUM;`, extend the usage
  line with `[-sedes]`, and after the consilium is built:
  `consilium.sedes_scribere = sedes;`. Update each `.sh` usage comment.

- [x] **Step 4: Run each client STML gate green**
  (`./crusta/compile_probationes.sh stml`, `./css/compile_probationes.sh
  stml`, `./md/… stml`, `./html/… stml`, `./oratio/… stml`) and record
  the `sedes:` lines. Measure the view's size over the house corpus:
  `for f in $(git ls-files '*.sh'); do ./crusta/arbor.sh -tacitus "$f";
  ./crusta/arbor.sh -sedes -tacitus "$f"; done` summed (record in the
  worklog; no pin).

- [x] **Step 5: Plant.** `silva.planta('materia/fontes/materia_arbor.c',
  'chorda_aedificator_appendere_i32(sedes, tractus->columna);',
  'chorda_aedificator_appendere_i32(sedes, tractus->columna + I);',
  'crusta', 'stml')` → red with `sedes dispares`; green on revert.

- [x] **Step 6: Docs, format, words, audit, commit.** Worklog entry (the
  verifier's independence, per-client element counts, size growth);
  formator; `./oratio/quaere.sh verificare exspectatum exspectata visa
  acta summa ima media`; `./materia/shim_probare.sh`; commit via
  `silva.commissio_umbra` with all touched paths and portae
  `['materia', 'css', 'md', 'html', 'oratio', 'crusta', 'briar']`.

---

## Task A3: The engine compares repeated captures under TRANSPARENTIA

**Executed 2026-09-17.** Baseline first: root exemplaria 392, macros 197; oratio oracle 873 rows 45.24 s; crusta survey 20 counts; silva exemplaria already RED on the unchanged tree (lib pin 176 vs 180, fixed first in `6e679497`). The planned fix went green, but a live probe over a crusta view exposed D11; with Fran the copy became `_nodum_comparandum` (author form dropped always, TRANSPARENTIA parts when active, children re-parented — the writer reads a text's multiline prefix from `parens`). Exemplaria 414/414, macros 197/197; oracle rows byte-identical (45.07/44.65 s, after forcing the stale oracle binary to relink, quaestio 01M2Q2BJXD); survey identical; silva latinum sites 4159 → 4175 = the 16 `NIHIL` comparisons added. Four plants, each red at its own fixture. Worklog `lib/stml_macros.worklog.md`.

**Files:**
- Modify: `lib/stml_macros.c` (`_ligamen_ponere`, 3632-3673; new helper
  before it)
- Modify: `probationes/probatio_stml_exemplaria.c` (new block after the
  "captura iterata: aequalitas" block, line ~161)
- Modify: `docs/stml-reference.md` (§19.2 table row "repeated `$x`",
  §19.4 last sentence)
- Modify: `lib/stml_macros.worklog.md` if it exists, else the phase note
  goes in `materia/fontes/materia_arbor.worklog.md`

**Interfaces:**
- Consumes: `_est_perspicuum`, `_est_perspicuum_attributum`
  (`lib/stml_macros.c:3323-3337`), `StmlMacroContextus.transparentia`,
  `.transparentia_attributa`.
- Produces: repeated node captures equal when their forests are equal
  after removing TRANSPARENTIA tags (whole subtrees) and attributes;
  without TRANSPARENTIA, byte-exact as before.

- [x] **Step 1: Baseline the row corpora on the unchanged tree.**
  - `./compile_tests.sh stml_exemplaria` and `./compile_tests.sh
    stml_macros` (record pass counts).
  - `./silva/compile_probationes.sh exemplaria` (record).
  - `./oratio/oraculum.sh -machina > $SCRATCH/oraculum_ante_a3.tsv` and
    `time` it (oratio's rules run through this engine).
  - `python3 -c "import sys; sys.path.insert(0,'pythonica'); import
    silva; e = silva.exemplaria('*.sh', open('$SCRATCH/lint/census.stml')
    .read()); print(e.summae)"` if the survey rule file still exists in
    the scratchpad; otherwise use pythonica's `REGULA_NT` from
    `pythonica/probatio_silva.py`. Save the printed summae.

- [x] **Step 2: Write the failing test.**

```c
    /* --- captura iterata SUB TRANSPARENTIA (materia-sedes A3) --- */
    {
        StmlExpansioResultus e;

        imprimere("\n--- captura iterata sub TRANSPARENTIA ---\n");
        /* attributa perspicua: valores diversi, nodi aequales */
        e = _expandere_litteras(piscina, intern,
            "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
            "<TRANSPARENTIA tags=\"nihil\" attributa=\"s\"/>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
                "<r/></radix>");
        }
        /* tagi perspicui: subarbor diversa, nodi aequales */
        e = _expandere_litteras(piscina, intern,
            "<radix><p><x><t>1</t>a</x><y><x>a</x></y></p>"
            "<TRANSPARENTIA tags=\"t\"/>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x><t>1</t>a</x><y><x>a</x></y></p>"
                "<r/></radix>");
        }
        /* sine TRANSPARENTIA: octetim, attributa diversa non congruunt */
        e = _expandere_litteras(piscina, intern,
            "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
            "<EXEMPLAR output=\"$m\"><p><x $n/><y><x $n/></y></p>"
            "</EXEMPLAR>"
            "<PER congruentia=\"$m\"><r/></PER></radix>");
        CREDO_VERUM (e.successus);
        si (e.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (
                stml_scribere(e.radix_expansa, piscina, FALSUM),
                "<radix><p><x s=\"1\">a</x><y><x s=\"2\">a</x></y></p>"
                "</radix>");
        }
    }
```

  Run `./compile_tests.sh stml_exemplaria` → the first two blocks red
  (no `<r/>`), the third green.

- [x] **Step 3: The fix.** Before `_ligamen_ponere`:

```c
/* Copia nodi SINE perspicuis (regula V sub TRANSPARENTIA, spec
 * materia-sedes par. IV; reference par. 19.4 'on both sides'):
 * attributa perspicua omissa, liberi elementa tagi perspicui cum
 * subarbore tota omissi, cetera ut in originali. Scriptura copiae
 * octetim confertur - sine TRANSPARENTIA copia non fit (comparatio
 * vetus intacta). */
interior StmlNodus*
_perspicua_exuere (
    StmlMacroContextus* ctx,
            StmlNodus* nodus)
{
    StmlNodus* copia;
          i32  i;

    copia = (StmlNodus*)piscina_allocare(ctx->piscina,
        magnitudo(StmlNodus));
    si (copia == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(copia, nodus, magnitudo(StmlNodus));
    si (nodus->attributa != NIHIL)
    {
        copia->attributa = xar_creare(ctx->piscina,
            (i32)magnitudo(StmlAttributum));
        si (copia->attributa == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i < xar_numerus(nodus->attributa); i++)
        {
            StmlAttributum* a = (StmlAttributum*)xar_obtinere(
                nodus->attributa, i);
            StmlAttributum* cella;

            si (a == NIHIL || _est_perspicuum_attributum(ctx, a->titulus))
            {
                perge;
            }
            cella = (StmlAttributum*)xar_addere(copia->attributa);
            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = *a;
        }
    }
    si (nodus->liberi != NIHIL)
    {
        copia->liberi = xar_creare(ctx->piscina,
            (i32)magnitudo(StmlNodus*));
        si (copia->liberi == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i < xar_numerus(nodus->liberi); i++)
        {
            StmlNodus*  liberum = *(StmlNodus**)xar_obtinere(
                nodus->liberi, i);
            StmlNodus*  exutum;
            StmlNodus** cella;

            si (   liberum != NIHIL
                && liberum->genus == STML_NODUS_ELEMENTUM
                && liberum->titulus != NIHIL
                && _est_perspicuum(ctx, liberum->titulus))
            {
                perge;
            }
            exutum = liberum != NIHIL ? _perspicua_exuere(ctx, liberum)
                                      : NIHIL;
            si (liberum != NIHIL && exutum == NIHIL)
            {
                redde NIHIL;
            }
            cella = (StmlNodus**)xar_addere(copia->liberi);
            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = exutum;
        }
    }
    redde copia;
}
```

  and inside `_ligamen_ponere`, replace the node branch:

```c
        si (nodus != NIHIL && lig->nodus != NIHIL)
        {
            StmlNodus* na = lig->nodus;
            StmlNodus* nb = nodus;
               chorda  sa;
               chorda  sb;

            si (   ctx->transparentia != NIHIL
                || ctx->transparentia_attributa != NIHIL)
            {
                na = _perspicua_exuere(ctx, lig->nodus);
                nb = _perspicua_exuere(ctx, nodus);
                si (na == NIHIL || nb == NIHIL)
                {
                    redde FALSUM;
                }
            }
            sa = stml_scribere(na, ctx->piscina, FALSUM);
            sb = stml_scribere(nb, ctx->piscina, FALSUM);
            redde chorda_aequalis(sa, sb);
        }
```

  (If `_est_perspicuum` / `_est_perspicuum_attributum` are defined after
  `_ligamen_ponere`, add forward declarations; they are at 3323-3337,
  before it.)

- [x] **Step 4: Run green; re-measure Step 1.** Tests green; silva
  exemplaria green unchanged; `./oratio/oraculum.sh -machina` diffed
  against the baseline — MUST be byte-identical (any row change: stop,
  name it, bring it to Fran); time recorded; crusta survey summae
  recorded before/after (a rise is the fix working: name the rows).

- [x] **Step 5: Plant.** `silva.planta('lib/stml_macros.c', 'na =
  _perspicua_exuere(ctx, lig->nodus);', 'na = lig->nodus;', 'radix',
  'stml_exemplaria')` → red; green on revert.

- [x] **Step 6: Docs and commit.** `docs/stml-reference.md` §19.2 row:
  `| repeated \`$x\` | equality constraint; forests compared byte-wise
  after removing TRANSPARENTIA tags and attributes (strict mode:
  byte-wise) |`; §19.4 last sentence: "Affects matching and repeated
  captures only, never emission or canon." Worklog entry with the
  measured deltas and timings. Commit via `silva.commissio` with portae
  `['radix', 'silva', 'oratio', 'pythonica']` (radix filter-free is the
  root suite; use `('radix', 'stml')` if the whole root suite is too
  long — check its time in `build/test_logs/radix.log` first).

---

## Task A4: pythonica — `arbor(sedes)`, rows with lines, TRANSPARENTIA refusal

**Executed 2026-09-17.** As planned, plus one gate: the A3 case end to end (`x=1⏎echo a⏎echo a` → one row, line 2 column 1, `echo a`; `echo a⏎echo b` → none). It needs `cursus="fratrum"`: greedy binds `x=1` and never retries (0 rows, measured). Gate `PYTHONICA: sana`. Plant I (row closes without its position) → 3 red (relata, exemplaria lines, the twice rule); plant II (prelude without `attributa="sedes octeti"`) → 1 red (the twice rule, 0 rows). House census over the view, 237 `.sh` / 20 rules, 4.6 s: counts identical to the plain-view baseline except `exit-2` 263 → 270 = the seven `exit 2` lines `e12c1f46` added; 1,227 of 2,002 rows positioned, the 775 without are exactly the `<situs/>` rules; every `-nt` row's source slice reads `-nt`. The scratch census rule declared `tags="ante post"` alone and met the new refusal at once.

**Files:**
- Modify: `pythonica/silva.py` (`arbor` 3237-3260; `_TRANSPARENTIA`
  3292-3296; `Congruentia`/`Exemplaria` 3305-3307; `_relata` 3337-3388;
  `exemplaria` 3402-3490)
- Modify: `pythonica/probatio_silva.py` (section "clientes materiae",
  from line 1454)
- Modify: `pythonica/README.md`, `pythonica/pythonica.worklog.md`

**Interfaces:**
- Consumes: A2's `-sedes` flags; A3's engine.
- Produces: `silva.arbor(via, nudum=False, sedes=False)`;
  `Congruentia(via, lint, textus, attributa, linea, columna,
  textus_fontis)`; `Exemplaria(summae, plagulae, congruentiae,
  fracturae, sine_sede)`; `_relata` rows are `(textus, sedes, octeti)`.

- [x] **Step 1: Failing tests.** In `probatio_silva.py`, the existing
  `rl ==` assertion becomes rows of triples, and new checks follow the
  exemplaria block:

```python
rl = silva._relata(silva._vertere(
    '<relatum lint="vacuum"/>\n<relatum lint="x" pro="y"><situs>a &amp; b'
    '</situs><situs sedes="2:1-2:4" octeti="5-8"><crusta-linea/><br/>c'
    '</situs><situs><q sedes="3:3-3:4" octeti="9-10">d</q></situs>'
    '<br/></relatum>\n'))
credo(rl == [('vacuum', {}, []),
             ('x', {'pro': 'y'}, [('a & b', None, None),
                                  ('c', '2:1-2:4', '5-8'),
                                  ('d', '3:3-3:4', '9-10'),
                                  ('', None, None)])],
      'relata: ordines cum sede propria aut prima descendentis: %r' % (rl,))

arb_sedes = silva.arbor(via_sh, sedes=True)
credo('visio="sedes"' in arb_sedes and 'octeti="0-16"' in arb_sedes
      and 'sedes="1:1-1:3"' in arb_sedes,
      'arbor .sh sedes: visio, tractus conditionis et lexematis if')
try:
    silva.arbor(via, sedes=True)
    credo(False, 'arbor .c sedes refutatur')
except silva.SilvaError as ex:
    credo('sedes' in str(ex), 'arbor .c sedes refutatur (clientium solum)')

credo({(c.via, c.linea, c.columna, c.textus_fontis)
       for c in ex_nt.congruentiae}
      == {(via_nt1, 1, 8, '-nt'), (via_nt2, 1, 10, '-nt')}
      and ex_nt.sine_sede == {via_nt1: 0, via_nt2: 0},
      'exemplaria: ordines cum linea, columna, textu fontis: %r'
      % ([(c.via, c.linea, c.columna, c.textus_fontis)
          for c in ex_nt.congruentiae],))
try:
    silva.exemplaria(via_nt1, '<TRANSPARENTIA tags="ante post"/>\n'
                     + REGULA_NT)
    credo(False, 'exemplaria: TRANSPARENTIA sine attributis sedium '
          'refutatur')
except silva.SilvaError as ex:
    credo('attributa' in str(ex),
          'exemplaria: TRANSPARENTIA sine attributa="sedes octeti" '
          'refutatur')
```

  (`via_sh` is `'if a; then b; fi # c\n\n'`: `conditio` spans bytes
  0–16, `if` 0–2. `via_nt1` has `-nt` at byte 7, `via_nt2` at byte 9.)
  Run `./pythonica/probare.sh` → red.

- [x] **Step 2: Implementation.**

```python
def arbor(via, nudum=False, sedes=False):
    """documentum STML canonicum plagulae - textus. ... (docstring as
    today, plus:) sedes: VISIO sedium clientis materiae (attributa
    sedes="L:C-L:C" octeti="B-B"; lector recusat) - silva (C) refutat."""
    cliens = _cliens_materiae(via)
    if cliens:
        if nudum:
            raise SilvaError('arbor: nudum solius silvae (C), non %s'
                             % cliens)
        args = ['./%s/arbor.sh' % cliens, _absoluta(via)]
        if sedes:
            args.append('-sedes')
        r = _curre(args)
        if r.returncode != 0:
            raise SilvaError('arbor %s fractus (rc %d): %s'
                             % (cliens, r.returncode,
                                r.stderr.strip()[-200:]))
        return r.stdout
    if sedes:
        raise SilvaError('arbor: sedes solius clientium materiae, non '
                         'silvae (C)')
    # ... silva path unchanged ...


_TRANSPARENTIA = {
    'materia': ('ante post', 'sedes octeti'),
    'silva': ('expansio pasta stringificatio api extentum ante post '
              'regio-cruda', None),
}

Congruentia = namedtuple('Congruentia', 'via lint textus attributa linea '
                         'columna textus_fontis')
Exemplaria = namedtuple('Exemplaria',
                        'summae plagulae congruentiae fracturae sine_sede')
```

```python
def _relata(html_textus):
    """[(lint, attributa, [(textus, sedes, octeti)])] ex elementis
    <relatum> summis: ordo = elementum filium quodque (PER emissio una),
    textus = textus totus subarboris (detonsus), sedes/octeti = attributa
    ordinis ipsius aut descendentis primi qui ea fert (visio sedium);
    None si nulla"""
    from html.parser import HTMLParser

    class _Lector(HTMLParser):
        def __init__(self):
            HTMLParser.__init__(self, convert_charrefs=True)
            self.relata = []
            self.gradus = 0          # profunditas intra relatum (0 = extra)
            self.ordo = None
            self.sedes = None
            self.octeti = None

        def _sedem_capere(self, a):
            if self.sedes is None and 'sedes' in a:
                self.sedes = a.get('sedes')
                self.octeti = a.get('octeti')

        def handle_starttag(self, tag, attrs):
            vacuum = tag in _VACUA_HTML
            a = dict(attrs)
            if self.gradus == 0:
                if tag == 'relatum':
                    lint = a.pop('lint', '')
                    self.relata.append((lint, a, []))
                    self.gradus = 1
                return
            if self.gradus == 1 and vacuum:     # ordo vacuus sine clausura
                self.relata[-1][2].append(('', a.get('sedes'),
                                           a.get('octeti')))
                return
            if self.gradus == 1 and self.ordo is None:
                self.ordo = []
                self.sedes = None
                self.octeti = None
            self._sedem_capere(a)
            if not vacuum:
                self.gradus += 1

        def handle_startendtag(self, tag, attrs):
            a = dict(attrs)
            if self.gradus == 1:
                self.relata[-1][2].append(('', a.get('sedes'),
                                           a.get('octeti')))
            elif self.gradus > 1:
                self._sedem_capere(a)
            elif self.gradus == 0 and tag == 'relatum':
                lint = a.pop('lint', '')
                self.relata.append((lint, a, []))

        def handle_endtag(self, tag):
            if self.gradus == 0 or tag in _VACUA_HTML:
                return
            self.gradus -= 1
            if self.gradus == 1 and self.ordo is not None:
                self.relata[-1][2].append((''.join(self.ordo).strip(),
                                           self.sedes, self.octeti))
                self.ordo = None

        def handle_data(self, data):
            if self.ordo is not None:
                self.ordo.append(data)

    lector = _Lector()
    lector.feed(html_textus)
    lector.close()
    return lector.relata
```

  In `exemplaria`, after `regula_textus` is known:

```python
    if (any(_cliens_materiae(v) for v in lista)
            and '<TRANSPARENTIA' in regula_textus):
        m = re.search(r'<TRANSPARENTIA\b[^>]*\battributa="([^"]*)"',
                      regula_textus)
        if not {'sedes', 'octeti'} <= set(m.group(1).split() if m else ()):
            raise SilvaError('exemplaria: TRANSPARENTIA regulae sine '
                             'attributa="sedes octeti" - capturae '
                             'iteratae sub visione sedium numquam '
                             'congruerent')
```

  `unum(via)`: `cliens = _cliens_materiae(via)`, `documentum = arbor(via,
  sedes=bool(cliens))`, and the prelude:

```python
        if '<TRANSPARENTIA' not in regula_textus:
            tags, attributa = _TRANSPARENTIA['materia' if cliens
                                             else 'silva']
            praefatio = ('<TRANSPARENTIA tags="%s"%s/>\n'
                         % (tags, ' attributa="%s"' % attributa
                            if attributa else ''))
```

  The collection loop:

```python
    summae = {}
    plagulae = {}
    congruentiae = []
    fracturae = {}
    sine_sede = {}
    fontes = {}
    relata_visa = False
    for via, relata, causa in eventus:
        if causa is not None:
            fracturae[via] = causa
            continue
        numeri = {}
        sine = 0
        for lint, attributa, ordines in relata:
            relata_visa = True
            numeri[lint] = numeri.get(lint, 0) + len(ordines)
            summae[lint] = summae.get(lint, 0) + len(ordines)
            for textus, sedes, octeti in ordines:
                linea = columna = textus_fontis = None
                m = re.match(r'(\d+):(\d+)-', sedes or '')
                if m:
                    linea, columna = int(m.group(1)), int(m.group(2))
                else:
                    sine += 1
                n = re.match(r'(\d+)-(\d+)$', octeti or '')
                if n:
                    if via not in fontes:
                        with open(_absoluta(via), 'rb') as f:
                            fontes[via] = f.read()
                    textus_fontis = fontes[via][
                        int(n.group(1)):int(n.group(2))].decode(
                            'utf-8', 'replace')
                congruentiae.append(Congruentia(
                    via, lint, textus, attributa, linea, columna,
                    textus_fontis))
        plagulae[via] = numeri
        sine_sede[via] = sine
```

  and the return `Exemplaria(summae, plagulae, congruentiae, fracturae,
  sine_sede)`. Docstring: drop "ordines lineam non ferunt"; name the
  view, the new fields and the TRANSPARENTIA refusal.

- [x] **Step 3: Green; plant by hand.** `./pythonica/probare.sh` →
  `PYTHONICA: sana`. Plant: in `_relata` make the row close with
  `(textus, None, None)` → red on the row and exemplaria checks; revert →
  green.

- [x] **Step 4: Docs and commit.** README (the new parameters and
  fields), worklog. `silva.commissio` with portae `['pythonica']`.

---

## Task B1: Declarations — attributes, canon, the baked table

**Executed 2026-09-17.** As planned, with measured changes: the STML reader does NOT decode `&quot;` in attribute values (the cause stayed literal), so the quote fixture uses the attribute-element form `<@absentia=>'}' "exspectata" \ a??)</>`; the emitter also escapes `??` as `?\?` (a trigraph — the first test draft's own `"quid??)"` became `]`); `_causa_sana` also refuses control bytes; the helper is `_chordam_litteralem` (a one-letter word in an identifier). Gate coctor 106/106; the five client declarations pass the widened canon; all five registrum gates green with generated files byte-identical (no client declares yet, so no seal can move). Rendered proba tables compile under the house flags and are formator-clean. `./tools/natura_struere.sh` not run: the registry canon is read at runtime, nothing bakes it. Plants: quote escaping off, `??` escaping off, table emitted with nothing declared — each red at its own assertion.

**Files:**
- Modify: `materia/fontes/materia_registrum.h` (new enums + structs, no
  includes)
- Modify: `materia/fontes/materia_coctor.c` (`Locus`/`Genus` structs
  22-35, `_locum_legere` 265-313, `_genus_legere` 316-373, `_caput_reddere`
  645-720, `_fontem_reddere` 723-844; new `_chordam_c`)
- Modify: `materia/fontes/materia_coctor.h` (`MateriaCoctio.numerus_diagnosticorum`)
- Modify: `materia/grammatica/registrum.canon` (attributes on genus and locus)
- Modify: `materia/probationes/probatio_materia_coctor.c`
- Modify: `materia/fontes/materia_coctor.worklog.md`, `materia/CLAUDE.md` ("Generata")

**Interfaces:**
- Produces (in `materia_registrum.h`):

```c
nomen enumeratio {
    MATERIA_DIAGNOSTICUM_GENUS = 0,   /* nodus generis ipse */
    MATERIA_DIAGNOSTICUM_ABSENTIA,    /* locus non scriptus */
    MATERIA_DIAGNOSTICUM_VACUA        /* lista sine elemento pleno */
} MateriaDiagnosticumSpecies;

nomen enumeratio {
    MATERIA_GRAVITAS_ERRATUM = 0,
    MATERIA_GRAVITAS_MONITUM
} MateriaGravitas;

nomen structura {
                   s32  genus;     /* index generis */
                   s32  locus;     /* intra genus; -I pro GENUS */
                   s32  species;   /* MateriaDiagnosticumSpecies */
                   s32  gravitas;  /* MateriaGravitas */
    constans character* codex;     /* "grex/tok_clausura" */
    constans character* causa;
} MateriaTabDiagnosticum;

nomen structura {
    constans MateriaTabDiagnosticum* tabula;
                                i32  numerus;
    /* genera 'inanis': nodi eorum listam 'vacua' non implent */
                       constans s32* inania;
                                i32  numerus_inanium;
} MateriaDiagnosticaCocta;
```

- Generated, only when the declaration declares any: `externus constans
  MateriaDiagnosticaCocta <P>_DIAGNOSTICA;` in the `.h`; the tables in
  the `.c`.

- [x] **Step 1: Failing coctor tests.** In `probatio_materia_coctor.c`
  add a declaration with every attribute and assert pieces of the
  rendered text; add four refusals:

```c
hic_manens constans character DECLARATIO_DIAGNOSTICA[] =
    "<registrum grammatica=\"proba\" praefixum=\"PROBA\""
    " typus=\"ProbaGenus\" sedes=\"x\">"
    "<genus titulus=\"malum\" diagnosticum=\"malum lectum\">"
    "<locus titulus=\"tokens\" species=\"lista-token\"/></genus>"
    "<genus titulus=\"separator\" inanis=\"verum\">"
    "<locus titulus=\"tok\" species=\"token\"/></genus>"
    "<genus titulus=\"grex\">"
    "<locus titulus=\"liberi\" species=\"lista-nodus\""
    " vacua=\"lista vacua\"/>"
    "<locus titulus=\"tok_clausura\" species=\"token\""
    " absentia=\"'}' &quot;exspectata&quot;\" gravitas=\"monitum\"/>"
    "</genus></registrum>";
```

  Assertions (`strstr` over the NUL-terminated renders) on
  `coctio.caput`: contains `externus constans MateriaDiagnosticaCocta
  PROBA_DIAGNOSTICA;`. On `coctio.fons`: contains
  `"    { (s32)PROBA_GENUS_MALUM, (s32)-1,\n      (s32)MATERIA_DIAGNOSTICUM_GENUS,\n"`,
  `"      \"malum\",\n      \"malum lectum\" },\n"`,
  `"      \"grex/liberi\",\n"`, `"      \"grex/tok_clausura\",\n"`,
  `"      (s32)MATERIA_GRAVITAS_MONITUM,\n"`,
  `"      \"'}' \\\"exspectata\\\"\" },\n"` (the escaped quotes),
  `"    (s32)PROBA_GENUS_SEPARATOR,\n"`; and
  `coctio.numerus_diagnosticorum == III`,
  `coctio.numerus_inanium == I`. Refusals (each FALSUM with the named `causa` prefix):
  `vacua` on a `token` locus → `"vacua in loco non listae:"`;
  `gravitas="gravis"` → `"gravitas ignota:"`; `inanis="falsum"` →
  `"inanis non 'verum':"`; `absentia` longer than LX characters →
  `"causa longior LX:"`. And a declaration with NO diagnostic attribute
  renders exactly the text the existing expected strings already pin
  (the existing assertions stay untouched and green).

- [x] **Step 2: Implementation.**
  1. `materia_coctor.h`, `MateriaCoctio` after `numerus_locorum`:
     `i32 numerus_diagnosticorum;` and `i32 numerus_inanium;`.
  2. `materia_coctor.c` structs: `Locus` gains `chorda absentia; chorda
     vacua; chorda gravitas;`, `Genus` gains `chorda diagnosticum; chorda
     gravitas; chorda inanis;`. Helpers after `_identificator_sanus`:

```c
interior b32
_gravitas_sana (
          Piscina* piscina,
    MateriaCoctio* coctio,
        StmlNodus* nodus,
            chorda  gravitas)
{
    si (   gravitas.mensura == ZEPHYRUM
        || chorda_aequalis_literis(gravitas, "erratum")
        || chorda_aequalis_literis(gravitas, "monitum"))
    {
        redde VERUM;
    }
    redde _recusare(piscina, coctio, nodus->linea, "gravitas ignota:",
        gravitas);
}

/* causa in C ut chorda brevis scribitur: lineae <= LXXII */
interior b32
_causa_sana (
          Piscina* piscina,
    MateriaCoctio* coctio,
        StmlNodus* nodus,
            chorda  causa)
{
    si (causa.mensura > (i32)LX)
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "causa longior LX:", causa);
    }
    redde VERUM;
}

/* chorda in litteram C: '\\' et '"' effugiuntur */
interior vacuum
_chordam_c (
    ChordaAedificator* a,
               chorda  s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] == (i8)'\\' || s.datum[i] == (i8)'"')
        {
            chorda_aedificator_appendere_character(a, '\\');
        }
        chorda_aedificator_appendere_character(a,
            (character)s.datum[i]);
    }
}
```

  3. `_locum_legere`, after the species is parsed and checked:

```c
    l->absentia  = _attributum(nodus, "absentia");
    l->vacua     = _attributum(nodus, "vacua");
    l->gravitas  = _attributum(nodus, "gravitas");
    si (   l->vacua.mensura > ZEPHYRUM
        && l->species != (s32)MATERIA_LOCUS_LISTA_NODUS
        && l->species != (s32)MATERIA_LOCUS_LISTA_TOKEN
        && l->species != (s32)MATERIA_LOCUS_LISTA_MIXTA)
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "vacua in loco non listae:", l->titulus);
    }
    si (   !_gravitas_sana(piscina, coctio, nodus, l->gravitas)
        || !_causa_sana(piscina, coctio, nodus, l->absentia)
        || !_causa_sana(piscina, coctio, nodus, l->vacua))
    {
        redde FALSUM;
    }
    si (l->absentia.mensura > ZEPHYRUM)
    {
        coctio->numerus_diagnosticorum++;
    }
    si (l->vacua.mensura > ZEPHYRUM)
    {
        coctio->numerus_diagnosticorum++;
    }
```

     `_genus_legere`, after the title check:

```c
    g->diagnosticum  = _attributum(nodus, "diagnosticum");
    g->gravitas      = _attributum(nodus, "gravitas");
    g->inanis        = _attributum(nodus, "inanis");
    si (   g->inanis.mensura > ZEPHYRUM
        && !chorda_aequalis_literis(g->inanis, "verum"))
    {
        redde _recusare(piscina, coctio, nodus->linea,
            "inanis non 'verum':", g->inanis);
    }
    si (   !_gravitas_sana(piscina, coctio, nodus, g->gravitas)
        || !_causa_sana(piscina, coctio, nodus, g->diagnosticum))
    {
        redde FALSUM;
    }
    si (g->diagnosticum.mensura > ZEPHYRUM)
    {
        coctio->numerus_diagnosticorum++;
    }
    si (g->inanis.mensura > ZEPHYRUM)
    {
        coctio->numerus_inanium++;
    }
```

  4. `_caput_reddere`, immediately before `_literis(a, "#endif /* ");`:

```c
    si (   coctio->numerus_diagnosticorum > ZEPHYRUM
        || coctio->numerus_inanium > ZEPHYRUM)
    {
        _literis(a, "externus constans MateriaDiagnosticaCocta ");
        _chordam(a, coctio->praefixum);
        _literis(a, "_DIAGNOSTICA;");
        _linea_nova(a);
        _linea_nova(a);
    }
```

  5. Emission of the source tables, before `_fontem_reddere`:

```c
interior vacuum
_ordinem_diagnostici (
         ChordaAedificator* a,
    constans MateriaCoctio* coctio,
           constans Genus* g,
                      s32  locus,
       constans character* species,
                   chorda  gravitas,
                   chorda  titulus_loci,
                   chorda  causa)
{
    _literis(a, "    { (s32)");
    _chordam(a, coctio->praefixum);
    _literis(a, "_GENUS_");
    _maiusculum(a, g->titulus);
    _literis(a, ", (s32)");
    si (locus < ZEPHYRUM)
    {
        _literis(a, "-1");
    }
    alioquin
    {
        chorda_aedificator_appendere_i32(a, (i32)locus);
    }
    _literis(a, ",");
    _linea_nova(a);
    _literis(a, "      (s32)MATERIA_DIAGNOSTICUM_");
    _literis(a, species);
    _literis(a, ",");
    _linea_nova(a);
    _literis(a, chorda_aequalis_literis(gravitas, "monitum")
        ? "      (s32)MATERIA_GRAVITAS_MONITUM,"
        : "      (s32)MATERIA_GRAVITAS_ERRATUM,");
    _linea_nova(a);
    _literis(a, "      \"");
    _chordam(a, g->titulus);
    si (titulus_loci.mensura > ZEPHYRUM)
    {
        _literis(a, "/");
        _chordam(a, titulus_loci);
    }
    _literis(a, "\",");
    _linea_nova(a);
    _literis(a, "      \"");
    _chordam_c(a, causa);
    _literis(a, "\" },");
    _linea_nova(a);
}

interior vacuum
_diagnostica_reddere (
         ChordaAedificator* a,
    constans MateriaCoctio* coctio,
             constans Xar* genera,
             constans Xar* loci)
{
    i32 k;
    i32 j;

    si (   coctio->numerus_diagnosticorum == ZEPHYRUM
        && coctio->numerus_inanium == ZEPHYRUM)
    {
        redde;
    }
    _linea_nova(a);
    si (coctio->numerus_diagnosticorum > ZEPHYRUM)
    {
        _literis(a, "hic_manens constans MateriaTabDiagnosticum "
            "DIAGNOSTICA_COCTA[] = {");
        _linea_nova(a);
        _literis(a, "    /* genus, locus, species, gravitas, codex, "
            "causa */");
        _linea_nova(a);
        per (k = ZEPHYRUM; k < xar_numerus(genera); k++)
        {
            constans Genus* g = (constans Genus*)xar_obtinere(genera, k);

            si (g->diagnosticum.mensura > ZEPHYRUM)
            {
                _ordinem_diagnostici(a, coctio, g, (s32)-I, "GENUS",
                    g->gravitas, _vacua(), g->diagnosticum);
            }
            per (j = g->offset; j < g->offset + g->numerus; j++)
            {
                constans Locus* l = (constans Locus*)xar_obtinere(loci, j);

                si (l->absentia.mensura > ZEPHYRUM)
                {
                    _ordinem_diagnostici(a, coctio, g,
                        (s32)(j - g->offset), "ABSENTIA", l->gravitas,
                        l->titulus, l->absentia);
                }
                si (l->vacua.mensura > ZEPHYRUM)
                {
                    _ordinem_diagnostici(a, coctio, g,
                        (s32)(j - g->offset), "VACUA", l->gravitas,
                        l->titulus, l->vacua);
                }
            }
        }
        _literis(a, "};");
        _linea_nova(a);
        _linea_nova(a);
    }
    si (coctio->numerus_inanium > ZEPHYRUM)
    {
        _literis(a, "hic_manens constans s32 INANIA_COCTA[] = {");
        _linea_nova(a);
        per (k = ZEPHYRUM; k < xar_numerus(genera); k++)
        {
            constans Genus* g = (constans Genus*)xar_obtinere(genera, k);

            si (g->inanis.mensura > ZEPHYRUM)
            {
                _literis(a, "    (s32)");
                _chordam(a, coctio->praefixum);
                _literis(a, "_GENUS_");
                _maiusculum(a, g->titulus);
                _literis(a, ",");
                _linea_nova(a);
            }
        }
        _literis(a, "};");
        _linea_nova(a);
        _linea_nova(a);
    }
    _literis(a, "constans MateriaDiagnosticaCocta ");
    _chordam(a, coctio->praefixum);
    _literis(a, "_DIAGNOSTICA = {");
    _linea_nova(a);
    si (coctio->numerus_diagnosticorum > ZEPHYRUM)
    {
        _literis(a, "    DIAGNOSTICA_COCTA,");
        _linea_nova(a);
        _literis(a, "    (i32)(magnitudo(DIAGNOSTICA_COCTA) /");
        _linea_nova(a);
        _literis(a, "        magnitudo(DIAGNOSTICA_COCTA[0])),");
        _linea_nova(a);
    }
    alioquin
    {
        _literis(a, "    NIHIL,");
        _linea_nova(a);
        _literis(a, "    (i32)0,");
        _linea_nova(a);
    }
    si (coctio->numerus_inanium > ZEPHYRUM)
    {
        _literis(a, "    INANIA_COCTA,");
        _linea_nova(a);
        _literis(a, "    (i32)(magnitudo(INANIA_COCTA) /");
        _linea_nova(a);
        _literis(a, "        magnitudo(INANIA_COCTA[0]))");
        _linea_nova(a);
    }
    alioquin
    {
        _literis(a, "    NIHIL,");
        _linea_nova(a);
        _literis(a, "    (i32)0");
        _linea_nova(a);
    }
    _literis(a, "};");
    _linea_nova(a);
}
```

     and in `_fontem_reddere`, just before `redde
     chorda_aedificator_finire(a);`: `_diagnostica_reddere(a, coctio,
     genera, loci);`. Render the proba declaration to a scratch `.c`, run
     `./silva/formator.sh <scratch> -vitia`, and adjust the emitter until
     CONFORMIS (the generated files must already be in house format).

- [x] **Step 3: Canon.** In `registrum.canon`, on `<elementum
  nomen="genus">` add

```xml
    <attributum nomen="diagnosticum" genus="textus"
      nota="causa diagnostici: omnis nodus huius generis (malum); materia_diagnostica_derivare"/>
    <attributum nomen="inanis" genus="electio"
      nota="nodi huius generis listam 'vacua' non implent (separator, heredoc)">
      <optio>verum</optio>
    </attributum>
    <attributum nomen="gravitas" genus="electio"
      nota="gravitas diagnostici generis (absens = erratum)">
      <optio>erratum</optio>
      <optio>monitum</optio>
    </attributum>
```

  and on `<elementum nomen="locus">`

```xml
    <attributum nomen="absentia" genus="textus"
      nota="causa diagnostici: locus non scriptus (clausura absens)"/>
    <attributum nomen="vacua" genus="textus"
      nota="causa diagnostici: lista non scripta aut elementis generum 'inanis' solis (coctor speciem listae poscit)"/>
    <attributum nomen="gravitas" genus="electio"
      nota="gravitas diagnosticorum loci (absens = erratum)">
      <optio>erratum</optio>
      <optio>monitum</optio>
    </attributum>
```

  Then `bin/canon_examen` on each existing `*.registrum.stml` (clean) and
  `./tools/natura_struere.sh`.

- [x] **Step 4: Green; plant.** `./materia/compile_probationes.sh coctor`
  green; every client registrum gate green and unchanged
  (`silva.porta('crusta', 'registrum')` and the same for `css`, `md`,
  `html`, `oratio`). Plant: `silva.planta(
  'materia/fontes/materia_coctor.c', '_chordam_c(a, causa);',
  '_chordam(a, causa);', 'materia', 'coctor')` → red on the
  escaped-quote assertion; green on revert.

- [x] **Step 5: Docs and commit.** `materia/CLAUDE.md` "Generata": the
  second table, its attributes, "diagnostics move no seal", D4. Worklog.
  Commit via `silva.commissio` with portae `[('materia', 'coctor'),
  ('crusta', 'registrum'), ('css', 'registrum'), ('md', 'registrum'),
  ('html', 'registrum'), ('oratio', 'registrum')]`.

---

## Task B2: Derivation, the order check, writer refusals with a place

**Executed 2026-09-17.** Gate `probatio_materia_diagnostica` 58/58 (all ten cases as planned), materia 9/9, shim 396/396. Changes to the plan's code, each measured: the parameter `registrum` is a latina macro (`register`) — the header parsed into an error node and `scribe` refused it, so it is `genera`; the seen-set stores the address of a file-scope `PRAESENS` instead of `(vacuum*)VERUM` (int-to-pointer cast); the NUL fixture builds a local array (`-Wwrite-strings` + `-Wcast-qual` reject `(i8*)"a\0b"`). Plants: `inanis` never matching, `_punctum_prius` always the file start, the order check disabled, the seen-set disabled — each red at its own case; two further plants did not compile (a helper left unused under `-Werror`) and `silva.planta` refused them without running the gate.

**Files:**
- Create: `materia/fontes/materia_diagnostica.h`, `materia/fontes/materia_diagnostica.c`
- Modify: `materia/fontes/materia_arbor.h` (`MateriaArborScriptura`, new
  last field `tractus`)
- Modify: `materia/fontes/materia_arbor.c` (`_arborem_struere` failure
  paths; `_scribere_valorem_in` token refusals record the token)
- Create: `materia/probationes/probatio_materia_diagnostica.c`
- Modify: `materia/fontes/materia_arbor.worklog.md`, `materia/CLAUDE.md`

**Interfaces:**
- Consumes: B1's `MateriaTabDiagnosticum`, `MateriaDiagnosticaCocta`;
  A1's `MateriaTractus`, `materia_tractus_lexematis`, `materia_tractus_nodi`.
- Produces:

```c
nomen structura {
                       s32  gravitas;   /* MateriaGravitas */
       constans character*  codex;      /* "grex/tok_clausura";
                                         * ':' = substrati */
       constans character*  causa;
    constans MateriaNodus*  nodus;      /* NIHIL pro ordine rupto */
    constans MateriaToken*  lexema;     /* ordo ruptus solum */
            MateriaTractus  tractus;    /* initium -I = computandum */
} MateriaDiagnosticum;

#define MATERIA_CODEX_ORDO       "materia:ordo-octetorum"
#define MATERIA_CODEX_SCRIPTURA  "materia:scriptura"

/* Xar de MateriaDiagnosticum ordine (tractus.initium, codex, ordo
 * inventionis). emissa (NIHIL licet): Xar de MateriaDiagnosticum a
 * parsatore emissa (classes sine vestigio in arbore) - tractus
 * computatur si initium -I et nodus adest. NIHIL = memoria. */
Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* registrum,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus,
                               Xar* emissa);
```

  and `MateriaArborScriptura.tractus` (`initium` −1 when the refusal
  names no place).

- [x] **Step 1: Failing gate** (`./silva/scribe.sh
  materia/probationes/probatio_materia_diagnostica.c`). Registry by hand:
  genera `radix {liberi lista-nodus}`, `par {tok_apertura token, liberi
  lista-nodus, tok_clausura token}`, `verbum {tok token}`, `separator
  {tok token}`, `malum {tokens lista-token}`; lexicon: copy
  `probatio_materia_arbor.c`'s. Table:

```c
hic_manens constans MateriaTabDiagnosticum TABULA[] = {
    { (s32)GR_PAR, (s32)II, (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM, "par/tok_clausura", "')' exspectata" },
    { (s32)GR_PAR, (s32)I, (s32)MATERIA_DIAGNOSTICUM_VACUA,
      (s32)MATERIA_GRAVITAS_ERRATUM, "par/liberi", "lista vacua" },
    { (s32)GR_MALUM, (s32)-I, (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_MONITUM, "malum", "malum" }
};
hic_manens constans s32 INANIA[] = { (s32)GR_SEPARATOR };
hic_manens constans MateriaDiagnosticaCocta DIAGNOSTICA = {
    TABULA, (i32)III, INANIA, (i32)I
};
```

  Cases, each asserting `xar_numerus`, and for each record `codex`
  (strcmp), `gravitas`, `tractus.initium/finis/linea/columna`:
  1. `"( x )"`: radix[par{"("@0, [verbum{"x"@2}], ")"@4}] → 0 records.
  2. `"( x"`: par without `tok_clausura` → 1 × `par/tok_clausura`,
     point 3–3, 1:4.
  3. `"( )"`: par liberi unwritten → 1 × `par/liberi`, range 0–3.
  4. `"( ; )"`: par liberi = [separator{";"@2}] → 1 × `par/liberi`.
  5. `")"`: radix[malum{[")"@0]}] → 1 × `malum`, MONITUM, 0–1.
  6. `"x ("`: radix[verbum{"x"@0}, par{}] (par with no tokens) →
     `par/tok_clausura` and `par/liberi`, both points at 1–1, 1:2
     (after `x`), sorted by codex.
  7. `"b a"`: radix[verbum{"b"@2}, verbum{"a"@0}] → 1 ×
     `materia:ordo-octetorum`, range 0–1, `lexema` = the `a` token.
  8. The same token appended twice → 0 records.
  9. The case-2 tree plus `emissa` with one record (`codex
     "par/probatio"`, `nodus` = that par, `tractus.initium` −1) → two
     records: `par/probatio` with tractus 0–3 computed from the node,
     then `par/tok_clausura` at 3 (sorted by `initium`).
  10. Writer refusal: a node holding an IDENT token whose value contains
      `'\0'` at @6 line 2 col 3 → `materia_arbor_scribere_nodum` fails;
      `s.tractus.initium == 6`, `linea == 2`, `columna == 3`.

- [x] **Step 2: Implementation.** `materia_diagnostica.c` walks the tree
  pre-order (loci in order, lists in order, REFERENTIA not descended);
  node rows are recorded BEFORE descending, so "the last source token
  seen" is the one before the node; tokens are visited in BYTE order
  (`ante` trivia, the token, `post` trivia), which is the emitter's
  order the order check needs.

```c
/* materia_diagnostica.c - Vide materia_diagnostica.h. */

#include "materia_diagnostica.h"
#include "tabula_dispersa.h"
#include <string.h>

nomen structura {
                             Piscina* piscina;
      constans MateriaRegistrumCoctum* registrum;
    constans MateriaDiagnosticaCocta* diagnostica;
          constans MateriaOrigoUncus* uncus;
                                 Xar* exitus;
                      TabulaDispersa* visa;
                                 b32  fons_notus;
                                 s32  fons;
                                 s32  finis_maximus;
                       MateriaTractus  ultimum;
                                 b32  ultimum_notum;
                                 b32  memoria_defecit;
} Derivatio;

interior vacuum
_addere (
                 Derivatio* d,
                       s32  gravitas,
       constans character*  codex,
       constans character*  causa,
    constans MateriaNodus*  nodus,
    constans MateriaToken*  lexema,
  constans MateriaTractus*  tractus)
{
    MateriaDiagnosticum* r = (MateriaDiagnosticum*)xar_addere(d->exitus);

    si (r == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde;
    }
    r->gravitas  = gravitas;
    r->codex     = codex;
    r->causa     = causa;
    r->nodus     = nodus;
    r->lexema    = lexema;
    r->tractus   = *tractus;
}

/* punctum in fine tractus */
interior MateriaTractus
_punctum_finis (
    constans MateriaTractus* t)
{
    MateriaTractus p = *t;

    p.initium  = t->finis;
    p.linea    = t->linea_finis;
    p.columna  = t->columna_finis;
    redde p;
}

/* punctum post lexema fontis ultimum visum; initium plagulae si nullum */
interior MateriaTractus
_punctum_prius (
    constans Derivatio* d)
{
    MateriaTractus p;

    si (d->ultimum_notum)
    {
        redde _punctum_finis(&d->ultimum);
    }
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.linea          = I;
    p.columna        = I;
    p.linea_finis    = I;
    p.columna_finis  = I;
    p.est_fons       = VERUM;
    redde p;
}

interior b32
_inane (
    constans Derivatio* d,
                   s32  genus)
{
    i32 k;

    per (k = ZEPHYRUM; k < d->diagnostica->numerus_inanium; k++)
    {
        si (d->diagnostica->inania[k] == genus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lista 'vacua': non scripta, aut elementa nodi generum inanium sola */
interior b32
_vacua (
      constans Derivatio* d,
    constans MateriaValor* valor)
{
    i32 k;

    si (valor->genus == MATERIA_VALOR_NIHIL)
    {
        redde VERUM;
    }
    si (valor->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*valor, k);

        si (e == NIHIL)
        {
            perge;
        }
        si (e->genus == MATERIA_VALOR_TOKEN)
        {
            redde FALSUM;
        }
        si (   e->genus == MATERIA_VALOR_NODUS && e->datum.nodus != NIHIL
            && !_inane(d, e->datum.nodus->genus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* VERUM si lexema iam visum (usus iteratus); aliter notatur */
interior b32
_visum (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    chorda clavis;
       i8* octeti;

    octeti = (i8*)piscina_allocare(d->piscina,
        magnitudo(constans MateriaToken*));
    si (octeti == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde VERUM;
    }
    memcpy(octeti, &lexema, magnitudo(constans MateriaToken*));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(constans MateriaToken*);
    si (tabula_dispersa_continet(d->visa, clavis))
    {
        redde VERUM;
    }
    (vacuum)tabula_dispersa_inserere(d->visa, clavis, (vacuum*)VERUM);
    redde FALSUM;
}

/* ordo octetorum: lexema fontis ante finem maximum iam visum */
interior vacuum
_lexema_notare (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    MateriaTractus t;

    si (lexema == NIHIL)
    {
        redde;
    }
    materia_tractus_lexematis(d->uncus, lexema, &t);
    si (t.initium < ZEPHYRUM || !t.est_fons)
    {
        redde;
    }
    si (   d->fons_notus && t.fons_index == d->fons
        && t.initium < d->finis_maximus)
    {
        _addere(d, (s32)MATERIA_GRAVITAS_ERRATUM, MATERIA_CODEX_ORDO,
            "lexema ante finem prioris", NIHIL, lexema, &t);
    }
    si (!d->fons_notus)
    {
        d->fons_notus  = VERUM;
        d->fons        = t.fons_index;
    }
    si (t.fons_index == d->fons && t.finis > d->finis_maximus)
    {
        d->finis_maximus = t.finis;
    }
    d->ultimum        = t;
    d->ultimum_notum  = VERUM;
}

interior vacuum
_lexema_ambulare (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    i32 k;

    si (lexema == NIHIL || _visum(d, lexema))
    {
        redde;
    }
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        _lexema_notare(d, lexema->spatia_ante[k]);
    }
    _lexema_notare(d, lexema);
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        _lexema_notare(d, lexema->spatia_post[k]);
    }
}

interior vacuum
_valorem_ambulare (
                Derivatio* d,
    constans MateriaValor* valor);

interior vacuum
_nodum_ambulare (
                Derivatio* d,
    constans MateriaNodus* nodus)
{
    MateriaTractus t;
    MateriaTractus punctum;
               b32 habet;
               i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    habet    = materia_tractus_nodi(d->uncus, nodus, &t);
    punctum  = _punctum_prius(d);
    per (k = ZEPHYRUM; k < d->diagnostica->numerus; k++)
    {
        constans MateriaTabDiagnosticum* r =
            &d->diagnostica->tabula[k];
                   constans MateriaValor* valor;
                           MateriaTractus finis;

        si (   r->genus != nodus->genus
            || (   r->species != (s32)MATERIA_DIAGNOSTICUM_GENUS
                && (   r->locus < ZEPHYRUM
                    || (i32)r->locus >= nodus->numerus_locorum)))
        {
            perge;
        }
        commutatio (r->species)
        {
        casus MATERIA_DIAGNOSTICUM_GENUS:
            _addere(d, r->gravitas, r->codex, r->causa, nodus, NIHIL,
                habet ? &t : &punctum);
            frange;
        casus MATERIA_DIAGNOSTICUM_ABSENTIA:
            valor = &nodus->loci[r->locus];
            si (   valor->genus == MATERIA_VALOR_NIHIL
                || (   valor->genus == MATERIA_VALOR_REFERENTIA
                    && valor->datum.nodus == NIHIL))
            {
                finis = habet ? _punctum_finis(&t) : punctum;
                _addere(d, r->gravitas, r->codex, r->causa, nodus, NIHIL,
                    &finis);
            }
            frange;
        casus MATERIA_DIAGNOSTICUM_VACUA:
            si (_vacua(d, &nodus->loci[r->locus]))
            {
                _addere(d, r->gravitas, r->codex, r->causa, nodus, NIHIL,
                    habet ? &t : &punctum);
            }
            frange;
        ordinarius:
            frange;
        }
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_ambulare(d, &nodus->loci[k]);
    }
}

interior vacuum
_valorem_ambulare (
                Derivatio* d,
    constans MateriaValor* valor)
{
    i32 k;

    commutatio (valor->genus)
    {
    casus MATERIA_VALOR_TOKEN:
        _lexema_ambulare(d, valor->datum.token);
        frange;
    casus MATERIA_VALOR_NODUS:
        _nodum_ambulare(d, valor->datum.nodus);
        frange;
    casus MATERIA_VALOR_LISTA:
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
        {
            constans MateriaValor* e =
                materia_valor_lista_obtinere(*valor, k);

            si (e != NIHIL)
            {
                _valorem_ambulare(d, e);
            }
        }
        frange;
    ordinarius:
        frange;
    }
}

/* ordo: initium, deinde codex; stabilis (insertio) */
interior b32
_prius (
    constans MateriaDiagnosticum* a,
    constans MateriaDiagnosticum* b)
{
    si (a->tractus.initium != b->tractus.initium)
    {
        redde (b32)(a->tractus.initium < b->tractus.initium);
    }
    redde (b32)(strcmp(a->codex, b->codex) < ZEPHYRUM);
}

Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* registrum,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus,
                               Xar* emissa)
{
               Derivatio  d;
    MateriaDiagnosticum*  series;
                    Xar*  ordinata;
                     i32  numerus;
                     i32  k;

    si (piscina == NIHIL || registrum == NIHIL || diagnostica == NIHIL)
    {
        redde NIHIL;
    }
    memset(&d, ZEPHYRUM, magnitudo(d));
    d.piscina      = piscina;
    d.registrum    = registrum;
    d.diagnostica  = diagnostica;
    d.uncus        = uncus;
    d.exitus       = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    d.visa         = tabula_dispersa_creare_chorda(piscina, 1024);
    si (d.exitus == NIHIL || d.visa == NIHIL)
    {
        redde NIHIL;
    }
    _nodum_ambulare(&d, radix);
    per (k = ZEPHYRUM; emissa != NIHIL && k < xar_numerus(emissa); k++)
    {
        constans MateriaDiagnosticum* e =
            (constans MateriaDiagnosticum*)xar_obtinere(emissa, k);
                       MateriaTractus t;

        si (e == NIHIL)
        {
            perge;
        }
        t = e->tractus;
        si (t.initium < ZEPHYRUM && e->nodus != NIHIL)
        {
            (vacuum)materia_tractus_nodi(uncus, e->nodus, &t);
        }
        _addere(&d, e->gravitas, e->codex, e->causa, e->nodus, e->lexema,
            &t);
    }
    si (d.memoria_defecit)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(d.exitus);
    series = (MateriaDiagnosticum*)piscina_allocare(piscina,
        (memoriae_index)(magnitudo(MateriaDiagnosticum)
            * (memoriae_index)(numerus + I)));
    ordinata = xar_creare(piscina, (i32)magnitudo(MateriaDiagnosticum));
    si (series == NIHIL || ordinata == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MateriaDiagnosticum clavis =
            *(MateriaDiagnosticum*)xar_obtinere(d.exitus, k);
        s32 j = (s32)k - (s32)I;

        dum (j >= ZEPHYRUM && _prius(&clavis, &series[j]))
        {
            series[j + I] = series[j];
            j--;
        }
        series[j + I] = clavis;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MateriaDiagnosticum* cella =
            (MateriaDiagnosticum*)xar_addere(ordinata);

        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = series[k];
    }
    redde ordinata;
}
```

  `registrum` is kept for the bound checks a later client may need and
  to match the spec's signature; `d.registrum` is set so the build has no
  unused parameter. The header declares `MateriaDiagnosticum`,
  `MATERIA_CODEX_ORDO`, `MATERIA_CODEX_SCRIPTURA` and the entry (see
  Interfaces) and includes `latina.h`, `piscina.h`, `xar.h`,
  `materia_nodus.h`, `materia_registrum.h`.

  Writer (`materia_arbor.c`):
  1. Scriptor struct, after `sedes`: `constans MateriaToken*
     lexema_refutatum;` and in `_arborem_struere` `st.lexema_refutatum =
     NIHIL;`.
  2. `_scribere_valorem_in`, `MATERIA_LOCUS_TOKEN` case:

```c
        liberum = _scribere_lexema(st, valor.datum.token);
        si (liberum == NIHIL)
        {
            st->sedes             = sedes;
            st->lexema_refutatum  = valor.datum.token;
            redde FALSUM;
        }
```

     and in the list case, right after
     `liberum = _scribere_lexema(st, e->datum.token);`:

```c
                si (liberum == NIHIL && st->lexema_refutatum == NIHIL)
                {
                    st->lexema_refutatum = e->datum.token;
                }
```

  3. `materia_arbor.h`, append to `MateriaArborScriptura` after `arbor`:

```c
    /* TRACTUS REFUTATIONIS (2026-09-17, materia-sedes par. VIII):
     * lexema refutatum si notum, aliter nodus 'sedes'; initium -I =
     * nulla sedes. Campus ULTIMUS. */
            MateriaTractus  tractus;
```

  4. Before `_arborem_struere`:

```c
/* tractus refutationis: lexema refutatum si notum, aliter nodus sedis */
interior vacuum
_tractum_refutationis (
            MateriaArborScriptura* fructus,
       constans MateriaArborScriptor* st,
    constans MateriaArborConsilium* consilium)
{
    si (st->lexema_refutatum != NIHIL)
    {
        materia_tractus_lexematis(consilium->origo,
            st->lexema_refutatum, &fructus->tractus);
    }
    alioquin si (fructus->sedes != NIHIL)
    {
        (vacuum)materia_tractus_nodi(consilium->origo, fructus->sedes,
            &fructus->tractus);
    }
}
```

     In `_arborem_struere`, beside the other `fructus.*` initializers:
     `memset(&fructus.tractus, ZEPHYRUM, magnitudo(fructus.tractus));
     fructus.tractus.initium = (s32)-I;`; after `fructus.sedes =
     st.sedes;` (radix NIHIL path) and after `fructus.sedes = r->nodus;`
     (reference guard) add `_tractum_refutationis(&fructus, &st,
     consilium);`.

- [x] **Step 3: Green; plant.** `./materia/compile_probationes.sh
  diagnostica` green. Plant: `silva.planta(
  'materia/fontes/materia_diagnostica.c', '&& !_inane(d,
  e->datum.nodus->genus))', '&& VERUM)', 'materia', 'diagnostica')` →
  case 4 red (a separator fills the list); revert → green.

- [x] **Step 4: Docs, audit, commit.** `materia/CLAUDE.md` new section
  "Diagnostica (2026-09-17)" (declared + derived, the three species, the
  order check, `emissa`, writer refusal tractus, printing is not
  materia's). Formator; `./oratio/quaere.sh` on new words; nexus;
  `./materia/shim_probare.sh`; `silva.commissio_umbra` with portae
  `['materia', 'css', 'md', 'html', 'oratio', 'crusta', 'briar']`.

---

## Task B3: crusta — declarations, the emitted class, agreement, bash's lines

**Executed 2026-09-17 (steps 1-5, 7; step 6 deferred to its own commit).** 39 declarations, seal unmoved (`873ce8f4` both sides). Agreement gate green at first run: house 235 (0 errata, 0 monita), oracle 121 (mala 13/13, clausurae 5/5, vacuae 3/3), adversarial 17, all 2,934 fuzz cases. TWO defects found in B2's walker by wiring it into the fuzz gate (quaestio 01M2R9MKFQ): it recursed (20k lives, 40k SIGSEGV — shallower than the writer it claimed to match) and it was QUADRATIC (`materia_tractus_nodi` per node: 40k = 31.6 s, killed by credo's 5 s limit and reported as a crash). Now iterative with ranges accumulated bottom-up like A1's writer: 40k = 0.06 s, 100k survives while the writer dies; pinned by a 50,000-deep case in materia's gate. The plan's plant (`separator` `inanis`) stayed GREEN — no corpus holds a separator-only list; 16 inline shapes were added and the plant moved to `malum`'s `inanis`, which goes red on `{ ; }` and `while; do a; done`. `separator`/`heredoc` `inanis` remain unexercised, named.

**Files:**
- Modify: `crusta/grammatica/crusta.registrum.stml` (attributes only;
  no genus or locus added — the seal must not move)
- Regenerate: `crusta/fontes/crusta_registrum_coctum.{h,c}`
- Modify: `crusta/fontes/crusta_arbor.h` (`CrustaParsura.diagnostica`,
  last field), `crusta/fontes/crusta_arbor.c` (`_vacuas_numerare` RAMUS
  branch, 708-716: emit)
- Create: `crusta/fontes/crusta_diagnostica.{h,c}`
- Create: `crusta/probationes/probatio_crusta_diagnostica.c`
- Modify: `crusta/probationes/probatio_crusta_totalitas.c` (fuzz cases),
  `crusta/probationes/probatio_crusta_differentia.c` (two new pins)
- Modify: `crusta/compile_probationes.sh:131` (`materia_diagnostica`)
- Modify: `crusta/CLAUDE.md`, `crusta/fontes/crusta_arbor.worklog.md`

**Interfaces:**
- Consumes: B1 table, B2 `materia_diagnostica_derivare`.
- Produces: `CRUSTA_DIAGNOSTICA`; `CrustaParsura.diagnostica` (`Xar*` of
  `MateriaDiagnosticum`, NIHIL when none); `Xar* crusta_diagnostica(
  Piscina*, constans MateriaNodus* radix, constans CrustaParsura*)`;
  `CrustaConcordia {mala, clausurae, vacuae, transpositae, errata,
  monita; b32 concordat; constans character* causa;}`;
  `b32 crusta_diagnostica_concordia(Piscina*, constans MateriaNodus*,
  constans CrustaParsura*, CrustaConcordia*)`.

- [x] **Step 1: Declarations.** Attribute additions (Latin causes; `"`
  as `&quot;`):

| where | attribute | gravitas |
|---|---|---|
| `malum` | `diagnosticum="lexemata quae grammatica ponere non potuit"` | erratum |
| `separator`, `heredoc`, `malum` | `inanis="verum"` | — |
| `pars-gemina/tok_clausura` | `absentia="'&quot;' exspectatum"` | erratum |
| `pars-versa/tok_clausura` | ``absentia="'`' exspectatum"`` | erratum |
| `pars-expansio/tok_clausura`, `grex/tok_clausura` | `absentia="'}' exspectata"` | erratum |
| `pars-substitutio`, `pars-processus`, `tabulatum`, `inclusa`, `crustula`, `iudicium-inclusa` `/tok_clausura` | `absentia="')' exspectata"` | erratum |
| `pars-parametrum/tok_titulus` | `absentia="titulus parametri exspectatus"` | erratum |
| `pars-arithmetica/tok_clausura`, `arithmetica/tok_clausura` | `absentia="'))' exspectatum"` | erratum |
| `conditio/tok_clausura` | `absentia="'fi' exspectatum"` | erratum |
| `iteratio/cursus`, `cyclus/cursus`, `repetitio/cursus` | `absentia="'do ... done' exspectatum"` | erratum |
| `cursus/tok_clausura` | `absentia="'done' exspectatum"` | erratum |
| `electio/tok_clausura` | `absentia="'esac' exspectatum"` | erratum |
| `optio/tok_parenthesis_clausura` | `absentia="')' exemplaris exspectata"` | erratum |
| `iudicium/tok_clausura` | `absentia="']]' exspectatum"` | erratum |
| `functio/corpus` | `absentia="corpus functionis exspectatum"` | erratum |
| `socius/imperium` | `absentia="imperium coprocessus exspectatum"` | erratum |
| `heredoc/tok_delimitator` | `absentia="heredoc sine delimitatore ad finem"` | monitum |
| `binaria/tok_operator` | `absentia="operator arithmeticus exspectatus"` | monitum |
| `binaria/sinister`, `binaria/dexter` | `absentia="operandum exspectatum"` | monitum |
| `iudicium-binaria/tok_operator` | `absentia="operator condicionis exspectatus"` | monitum |
| `grex`, `crustula`, `cursus`, `ramus` `/liberi` | `vacua="lista mandatorum vacua"` | erratum |
| `conditio/liberi` | `vacua="lista mandatorum vacua"` | erratum |
| `conditio/probatio`, `repetitio/probatio` | `vacua="probatio vacua"` | erratum |

  Causes are ASCII only (they become C string literals). Run `./materia/coquere.sh
  crusta/grammatica/crusta.registrum.stml -scribere`, `bin/canon_examen
  crusta/grammatica/crusta.registrum.stml`, `./tools/natura_struere.sh`,
  and confirm the canon gate's seal `873ce8f4` is unchanged
  (`./crusta/compile_probationes.sh canon`).

- [x] **Step 2: The emitted class.** `crusta_arbor.h`, append to
  `CrustaParsura` after `sana`:

```c
    /* diagnostica a parsatore EMISSA (classes sine vestigio
     * declarabili in arbore: probatio 'elif' vacua - condicio in
     * tok_deinde pendet): Xar de MateriaDiagnosticum, NIHIL = nulla.
     * crusta_diagnostica ea cum derivatis miscet. */
    Xar* diagnostica;
```

  In `crusta_arbor.c`, a helper after `_lista_plena`:

```c
interior vacuum
_diagnosticum_emittere (
              Aedificatio* p,
    constans MateriaNodus* nodus,
       constans character* codex,
       constans character* causa)
{
    MateriaDiagnosticum* d;

    si (p->relatio == NIHIL)
    {
        redde;
    }
    si (p->relatio->diagnostica == NIHIL)
    {
        p->relatio->diagnostica = xar_creare(p->piscina,
            (i32)magnitudo(MateriaDiagnosticum));
        si (p->relatio->diagnostica == NIHIL)
        {
            p->memoria_defecit = VERUM;
            redde;
        }
    }
    d = (MateriaDiagnosticum*)xar_addere(p->relatio->diagnostica);
    si (d == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    memset(d, ZEPHYRUM, magnitudo(*d));
    d->gravitas         = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex            = codex;
    d->causa            = causa;
    d->nodus            = nodus;
    d->tractus.initium  = (s32)-I;
}
```

  (Use the builder's actual piscina field name — read `Aedificatio`
  first.) In `_vacuas_numerare`'s RAMUS branch, next to `vacuae++` for
  the probatio: `_diagnosticum_emittere(p, nodus, "ramus/probatio",
  "probatio 'elif' vacua");`. Check how `crusta_arbor_parsare`
  initializes the caller's `CrustaParsura` (the differentia gate passes
  an uninitialized one, `probatio_crusta_differentia.c:116`): if it
  memsets, nothing more; if it assigns fields one by one, add
  `relatio->diagnostica = NIHIL;` there.

- [x] **Step 3: `crusta_diagnostica.{h,c}`** (`./silva/scribe.sh`).

```c
/* crusta_diagnostica.h - Diagnostica crustae: derivata ex
 * CRUSTA_DIAGNOSTICA (declaratio registri) et emissa a parsatore
 * (CrustaParsura.diagnostica); concordia cum numeris parsatoris
 * (porta: praesentia per classem; sana si et solum si nulla
 * diagnostica). */
#ifndef CRUSTA_DIAGNOSTICA_H
#define CRUSTA_DIAGNOSTICA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_diagnostica.h"
#include "crusta_arbor.h"

/* Xar de MateriaDiagnosticum (materia_diagnostica_derivare); NIHIL =
 * memoria. relatio NIHIL licet (emissa nulla). */
Xar*
crusta_diagnostica (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio);

nomen structura {
                    i32  mala;
                    i32  clausurae;
                    i32  vacuae;
                    i32  transpositae;
                    i32  errata;
                    i32  monita;
                    b32  concordat;
    constans character*  causa;   /* classis prima discors; NIHIL */
} CrustaConcordia;

b32
crusta_diagnostica_concordia (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio,
          CrustaConcordia* concordia);

#endif /* CRUSTA_DIAGNOSTICA_H */
```

```c
/* crusta_diagnostica.c - Vide crusta_diagnostica.h. */

#include "crusta_diagnostica.h"
#include "crusta_registrum.h"
#include <string.h>

Xar*
crusta_diagnostica (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio)
{
    redde materia_diagnostica_derivare(piscina, radix, &CRUSTA_REGISTRUM,
        &CRUSTA_DIAGNOSTICA, NIHIL,
        relatio != NIHIL ? relatio->diagnostica : NIHIL);
}

interior constans MateriaTabDiagnosticum*
_ordo_tabulae (
    constans character* codex)
{
    i32 k;

    per (k = ZEPHYRUM; k < CRUSTA_DIAGNOSTICA.numerus; k++)
    {
        si (CRUSTA_DIAGNOSTICA.tabula[k].codex == codex)
        {
            redde &CRUSTA_DIAGNOSTICA.tabula[k];
        }
    }
    redde NIHIL;
}

interior vacuum
_discors (
    CrustaConcordia* c,
                b32  numerus_parsatoris,
                b32  numerus_derivatus,
    constans character* classis)
{
    si (numerus_parsatoris != numerus_derivatus && c->causa == NIHIL)
    {
        c->causa      = classis;
        c->concordat  = FALSUM;
    }
}

b32
crusta_diagnostica_concordia (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio,
          CrustaConcordia* c)
{
    Xar* d;
    i32  k;

    memset(c, ZEPHYRUM, magnitudo(*c));
    c->concordat = VERUM;
    d = crusta_diagnostica(piscina, radix, relatio);
    si (d == NIHIL || relatio == NIHIL)
    {
        c->concordat  = FALSUM;
        c->causa      = "memoria aut relatio nihil";
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* r =
            (constans MateriaDiagnosticum*)xar_obtinere(d, k);
        constans MateriaTabDiagnosticum* t;

        si (r->gravitas == (s32)MATERIA_GRAVITAS_MONITUM)
        {
            c->monita++;
        }
        alioquin
        {
            c->errata++;
        }
        si (strcmp(r->codex, MATERIA_CODEX_ORDO) == ZEPHYRUM)
        {
            c->transpositae++;
            perge;
        }
        si (strcmp(r->codex, "ramus/probatio") == ZEPHYRUM)
        {
            c->vacuae++;
            perge;
        }
        t = _ordo_tabulae(r->codex);
        si (t == NIHIL)
        {
            c->concordat  = FALSUM;
            c->causa      = "codex ignotus";
            perge;
        }
        si (t->species == (s32)MATERIA_DIAGNOSTICUM_GENUS)
        {
            c->mala++;
        }
        alioquin si (t->species == (s32)MATERIA_DIAGNOSTICUM_VACUA)
        {
            c->vacuae++;
        }
        alioquin si (   t->genus == (s32)CRUSTA_GENUS_BINARIA
                     || t->genus == (s32)CRUSTA_GENUS_IUDICIUM_BINARIA)
        {
            c->mala++;
        }
        alioquin
        {
            c->clausurae++;
        }
    }
    _discors(c, (b32)(relatio->mala > ZEPHYRUM),
        (b32)(c->mala > ZEPHYRUM), "mala");
    _discors(c, (b32)(relatio->clausurae_absentes > ZEPHYRUM),
        (b32)(c->clausurae > ZEPHYRUM), "clausurae");
    _discors(c, (b32)(relatio->listae_vacuae > ZEPHYRUM),
        (b32)(c->vacuae > ZEPHYRUM), "vacuae");
    _discors(c, (b32)(relatio->heredoca_transposita > ZEPHYRUM),
        (b32)(c->transpositae > ZEPHYRUM), "transpositae");
    _discors(c, relatio->sana, (b32)(c->errata + c->monita == ZEPHYRUM),
        "sana");
    redde c->concordat;
}
```

  (The codex pointer comparison is sound because every derived GENUS /
  ABSENTIA / VACUA record carries the table row's own `codex` pointer;
  the two substrate and parser codices are matched by `strcmp`.)

- [x] **Step 4: The agreement gate** `probatio_crusta_diagnostica.c`
  (`./silva/scribe.sh`). Sources: (a) the house corpus
  (`build/crusta_corpus.lst` minus the two pathology fixtures, the
  differentia gate's rule), (b) every oracle case (`crusta_oraculum_casus`:
  the pathology cases and the 89 FreeBSD files), (c)
  `probationes/fixa/crusta/adversarius.sh` split by
  `crusta_exempla_legere`.

```c
/* probatio_crusta_diagnostica.c - Concordia diagnosticorum (plan
 * materia-sedes B3; spec par. IX)
 *
 * Diagnostica DERIVATA (CRUSTA_DIAGNOSTICA et emissa) contra numeros
 * parsatoris: praesentia per classem (mala, clausurae, vacuae,
 * transpositae) et sana si et solum si nulla diagnostica, super
 * corpus domus,
 * casus oraculi et fixa adversarii. Domus sana: errata et monita
 * ZEPHYRUM pinnata. Numeri utriusque lateris summantur et
 * imprimuntur (machina arithmetica pro vestigio uno plus semel
 * numerare potest - praesentia, non numerus, concordat).
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_exempla.h"
#include "crusta_oraculum.h"
#include "crusta_registrum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOMUS_ERRATA     0
#define DOMUS_MONITA     0
#define DOMUS_MINIMUM    200
#define ORACULI_MINIMUM  100

nomen structura {
    i32 documenta;
    i32 discordes;
    i32 mala_parsatoris;
    i32 mala_derivata;
    i32 clausurae_parsatoris;
    i32 clausurae_derivatae;
    i32 vacuae_parsatoris;
    i32 vacuae_derivatae;
    i32 transpositae_parsatoris;
    i32 transpositae_derivatae;
    i32 errata;
    i32 monita;
} Summa;

interior vacuum
_documentum (
                Piscina* piscina,
    constans character* titulus,
    constans character* fons,
                    i32  mensura,
                 Summa* summa)
{
      CrustaParsura  relatio;
    CrustaConcordia  c;
      MateriaNodus* radix;

    memset(&relatio, ZEPHYRUM, magnitudo(relatio));
    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        &relatio);
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde;
    }
    (vacuum)crusta_diagnostica_concordia(piscina, radix, &relatio, &c);
    CREDO_VERUM (c.concordat);
    si (!c.concordat)
    {
        summa->discordes++;
        imprimere("  DISCORS %s (%s): parsator m%d c%d v%d t%d sana %d"
            " | derivata m%d c%d v%d t%d e%d mon%d\n", titulus,
            c.causa != NIHIL ? c.causa : "-", (integer)relatio.mala,
            (integer)relatio.clausurae_absentes,
            (integer)relatio.listae_vacuae,
            (integer)relatio.heredoca_transposita,
            (integer)relatio.sana, (integer)c.mala, (integer)c.clausurae,
            (integer)c.vacuae, (integer)c.transpositae,
            (integer)c.errata, (integer)c.monita);
    }
    summa->documenta++;
    summa->mala_parsatoris          += relatio.mala;
    summa->mala_derivata            += c.mala;
    summa->clausurae_parsatoris     += relatio.clausurae_absentes;
    summa->clausurae_derivatae      += c.clausurae;
    summa->vacuae_parsatoris        += relatio.listae_vacuae;
    summa->vacuae_derivatae         += c.vacuae;
    summa->transpositae_parsatoris  += relatio.heredoca_transposita;
    summa->transpositae_derivatae   += c.transpositae;
    summa->errata                   += c.errata;
    summa->monita                   += c.monita;
}

interior vacuum
_summam_imprimere (
    constans character* titulus,
       constans Summa* s)
{
    imprimere("  %s: %d documenta, discordes %d | mala %d/%d, clausurae "
        "%d/%d, vacuae %d/%d, transpositae %d/%d (parsator/derivata) | "
        "errata %d, monita %d\n", titulus, (integer)s->documenta,
        (integer)s->discordes, (integer)s->mala_parsatoris,
        (integer)s->mala_derivata, (integer)s->clausurae_parsatoris,
        (integer)s->clausurae_derivatae, (integer)s->vacuae_parsatoris,
        (integer)s->vacuae_derivatae, (integer)s->transpositae_parsatoris,
        (integer)s->transpositae_derivatae, (integer)s->errata,
        (integer)s->monita);
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix = getenv("RHUBARB_RADIX");
    constans character* causa;
                 Summa  domus;
                 Summa  oraculi;
                 Summa  adversarii;
             character* lista;
                   Xar* collectio;
                   i32  mensura = ZEPHYRUM;
                   i32  k;

    piscina = piscina_generare_dynamicum("probatio_crusta_diagnostica",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix == NIHIL)
    {
        radix = ".";
    }
    memset(&domus, ZEPHYRUM, magnitudo(domus));
    memset(&oraculi, ZEPHYRUM, magnitudo(oraculi));
    memset(&adversarii, ZEPHYRUM, magnitudo(adversarii));

    imprimere("\n--- Concordia: corpus domus ---\n");
    lista = crusta_oraculum_legere(piscina, radix,
        "build/crusta_corpus.lst", &mensura);
    si (lista == NIHIL)
    {
        CREDO_CULPA ("build/crusta_corpus.lst absens - cursor eam scribit");
    }
    alioquin
    {
        i32 ab = ZEPHYRUM;

        dum (ab < mensura)
        {
                  i32  ad = ab;
            character  via[1024];
              Piscina* propria;
            character* textus;
                  i32  longitudo = ZEPHYRUM;

            dum (ad < mensura && lista[ad] != '\n')
            {
                ad++;
            }
            si (ad == ab || ad - ab >= (i32)magnitudo(via))
            {
                ab = ad + I;
                perge;
            }
            memcpy(via, lista + ab, (size_t)(ad - ab));
            via[ad - ab]  = '\0';
            ab            = ad + I;
            si (   strcmp(via, "probationes/fixa/crusta/pathologiae.sh")
                    == ZEPHYRUM
                || strcmp(via, "probationes/fixa/crusta/adversarius.sh")
                    == ZEPHYRUM)
            {
                perge;
            }
            propria = piscina_generare_dynamicum("crusta_diagnostica",
                4194304);
            si (propria == NIHIL)
            {
                CREDO_CULPA ("piscina plagulae deficit");
                perge;
            }
            textus = crusta_oraculum_legere(propria, radix, via,
                &longitudo);
            si (textus != NIHIL)
            {
                _documentum(propria, via, textus, longitudo, &domus);
            }
            piscina_destruere(propria);
        }
    }
    _summam_imprimere("domus", &domus);
    CREDO_VERUM (domus.documenta >= (i32)DOMUS_MINIMUM);
    CREDO_AEQUALIS_I32 (domus.errata, (i32)DOMUS_ERRATA);
    CREDO_AEQUALIS_I32 (domus.monita, (i32)DOMUS_MONITA);

    imprimere("\n--- Concordia: casus oraculi ---\n");
    collectio = crusta_oraculum_casus(piscina, radix, &causa);
    CREDO_NON_NIHIL (collectio);
    per (k = ZEPHYRUM; collectio != NIHIL && k < xar_numerus(collectio);
         k++)
    {
        constans CrustaCasusOraculi* c =
            (constans CrustaCasusOraculi*)xar_obtinere(collectio, k);
        character titulus[256];

        sprintf(titulus, "%.*s", (integer)(c->clavis.mensura < CCL
            ? c->clavis.mensura : CCL),
            (constans character*)c->clavis.datum);
        _documentum(piscina, titulus, (constans character*)c->datum.datum,
            c->datum.mensura, &oraculi);
    }
    _summam_imprimere("oraculi", &oraculi);
    CREDO_VERUM (oraculi.documenta >= (i32)ORACULI_MINIMUM);

    imprimere("\n--- Concordia: adversarius per casus ---\n");
    {
        character* fons = crusta_oraculum_legere(piscina, radix,
            "probationes/fixa/crusta/adversarius.sh", &mensura);
              Xar* exempla = fons != NIHIL
            ? crusta_exempla_legere(piscina, fons, mensura) : NIHIL;

        CREDO_NON_NIHIL (exempla);
        per (k = ZEPHYRUM; exempla != NIHIL && k < xar_numerus(exempla);
             k++)
        {
            constans CrustaExemplum* e =
                (constans CrustaExemplum*)xar_obtinere(exempla, k);
            character titulus[256];

            sprintf(titulus, "adversarius %d", (integer)e->numerus);
            _documentum(piscina, titulus,
                (constans character*)e->datum.datum, e->datum.mensura,
                &adversarii);
        }
    }
    _summam_imprimere("adversarii", &adversarii);

    praeteritus = credo_omnia_praeterierunt();
    credo_imprimere_compendium();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```

  (Check the credo tail names against `probatio_crusta_differentia.c`'s
  own ending before compiling.) In `probatio_crusta_totalitas.c`, find
  every `crusta_arbor_parsare(` call over a generated case, and after
  it add:

```c
        {
            CrustaConcordia concordia;

            (vacuum)crusta_diagnostica_concordia(piscina, arbor, &relatio,
                &concordia);
            CREDO_VERUM (concordia.concordat);
            si (!concordia.concordat)
            {
                imprimere("    DISCORS diagnosticorum (%s)\n",
                    concordia.causa != NIHIL ? concordia.causa : "-");
            }
        }
```

  using that call site's own tree, relatio and piscina variable names
  (read them first). Add `materia_diagnostica` to
  `crusta/compile_probationes.sh`'s module list.

- [x] **Step 5: Measure until concordant.** Run
  `./crusta/compile_probationes.sh diagnostica` and `totalitas`. Every
  printed disagreement is resolved in this order: (1) a missing
  declaration (add the attribute; regenerate); (2) a class with no tree
  trace (emit it from the parser like `ramus/probatio`, named in the
  worklog and in the header comment of `CrustaParsura.diagnostica`); (3)
  anything else — STOP and bring it to Fran. No exemptions.

- [x] **Step 6: bash's lines and verdicts** in
  `probatio_crusta_differentia.c`. Two new RISING pins, both born at the
  measured value: `PINNA_GRAVITATIS` (cases where "no erratum" agrees
  with bash's exit 0 — the expectation, not a promise, is that the three
  known discordes of quaestio 01M2NT4CDD resolve once monita follow
  bash, D9) and `PINNA_LINEARUM` (among cases with bash status ≠ 0 whose
  ERRATUM text holds `line N`, those whose first erratum's `linea` is
  N). Add `#include "crusta_diagnostica.h"` and
  `#include "materia_diagnostica.h"`, the counters `gravitas_concordes`,
  `lineae_comparatae`, `lineae_concordes` beside `concordes`, a helper:

```c
/* numerus post 'line ' primum in textu erroris bash; ZEPHYRUM = nullus */
interior i32
_linea_bash (
    chorda textus)
{
    i32 k;

    per (k = ZEPHYRUM; k + V < textus.mensura; k++)
    {
        si (memcmp(textus.datum + k, "line ", (size_t)V) == ZEPHYRUM)
        {
            i32 v = ZEPHYRUM;
            i32 j = k + V;

            dum (   j < textus.mensura && textus.datum[j] >= '0'
                 && textus.datum[j] <= '9')
            {
                v = v * (i32)X + (i32)(textus.datum[j] - '0');
                j++;
            }
            redde v;
        }
    }
    redde ZEPHYRUM;
}
```

  and inside the per-case loop, after `sana` is computed (line 132):

```c
        si (arbor != NIHIL)
        {
            Xar* diagnostica = crusta_diagnostica(piscina, arbor,
                &relatio);
            constans MateriaDiagnosticum* primum = NIHIL;
            i32 j;

            per (j = ZEPHYRUM; diagnostica != NIHIL
                 && j < xar_numerus(diagnostica); j++)
            {
                constans MateriaDiagnosticum* d =
                    (constans MateriaDiagnosticum*)xar_obtinere(
                        diagnostica, j);

                si (d->gravitas == (s32)MATERIA_GRAVITAS_ERRATUM)
                {
                    primum = d;
                    frange;
                }
            }
            si ((primum == NIHIL) == accepta)
            {
                gravitas_concordes++;
            }
            alioquin
            {
                imprimere("    gravitas discors %.*s: bash status %.*s, "
                    "erratum primum %s\n",
                    (integer)c->clavis.mensura,
                    (constans character*)c->clavis.datum,
                    (integer)status.mensura,
                    (constans character*)status.datum,
                    primum != NIHIL ? primum->codex : "-");
            }
            si (   !accepta && erratum.datum != NIHIL
                && _linea_bash(erratum) > ZEPHYRUM && primum != NIHIL)
            {
                lineae_comparatae++;
                si (primum->tractus.linea == _linea_bash(erratum))
                {
                    lineae_concordes++;
                }
                alioquin
                {
                    imprimere("    linea discors %.*s: bash %d, crusta "
                        "%d (%s)\n", (integer)c->clavis.mensura,
                        (constans character*)c->clavis.datum,
                        (integer)_linea_bash(erratum),
                        (integer)primum->tractus.linea, primum->codex);
                }
            }
        }
```

  After the loop: print `gravitas concordes %d (pinna %d), lineae
  %d/%d (pinna %d)` with the NOTA "pinnam attolle" pattern of lines
  158-163, and `CREDO_VERUM (gravitas_concordes >=
  (i32)PINNA_GRAVITATIS);` `CREDO_VERUM (lineae_concordes >=
  (i32)PINNA_LINEARUM);`. Birth: set both pins to 0, run, read the
  printed values, set the pins to them with the cause in the commit.

**Executed 2026-09-17.** As planned for the line pin, which was born at
7/9 and is a complete law, not a residue: all six cases where bash names
a TOKEN agree; two of the three where bash says `unexpected end of file`
differ because bash reports the EOF line and we report the end of the
unfinished construct (`crlf` bash 5 / us 2, `si-apertum` bash 2 / us 1)
— kept deliberately, the construct's place serves a reader better.
Plant: ABSENTIA's point moved from the node's end to its start → red on
`lineae_concordes`.

The gravitas pin was **born dead and had to be earned.** It first read
118, identical to `concordes` case for case, and NO plant could move it
— not demoting the heredoc-at-EOF warning (that case was already
discordant for another reason), not removing every `gravitas="monitum"`
in the declaration. Cause: no oracle case carried a monitum without also
carrying an erratum. D9's four shapes (`[[ a b ]]`, `(( 1 2 ))`,
`(( 3 ! ))`, `$(( 1 + ))` — bash exit 0, the first with a message)
existed only as inline shapes in the `diagnostica` gate, with no bash
golden, so `differentia` could not see them. With Fran: appended to
`pathologiae.sh`, goldens regenerated → gravitas 122 while `concordes`
stays 118, the gap of four being exactly the monita, and the identical
plant now fires. The three remaining discordes are all desideratum
01M2PN1VYH, so the pin reaches 125 when that is fulfilled.

Widening the corpus moved pins in six gates plus one computus row, each
with its cause (exempla 32→36 and its tail assertion, stml 49→53 and
47→51, canon 47→51, totalitas 33→37, oraculum 121→125 / 112→115 /
`declare -f` 103→106, differentia 121→125; `pathologiae.sh` 1,735→1,883
bytes with the other four computus rows byte-identical). Trap: the Edit
tool NORMALIZES line endings — editing the fixture through it stripped
all four CR bytes from the `crlf` case and the regenerated golden
recorded bash accepting it (status 2 → 0); caught only by a golden diff
line I had not intended. Restored and re-appended with `printf >>`.
Gates: crusta 15/15.
- [x] **Step 7: Plant, docs, commit.** Plant by hand in the declaration:
  remove `inanis="verum"` from `separator`, regenerate → the agreement
  gate goes red on `{ ; }`-shaped cases (vacuae); restore, regenerate →
  green. `silva.planta` for the C side: in `crusta_diagnostica.c`, map
  VACUA to clausurae → red; revert → green. `crusta/CLAUDE.md` (a
  "Diagnostica" paragraph and the new gate in the table), worklog (D5–D9
  as measured, class counts, pins). Formator; words; commit via
  `silva.commissio_umbra` with portae `['crusta', 'materia']`.

---

## Task B4: css — declarations and gate

**Files:**
- Modify: `css/grammatica/css.registrum.stml` (`diagnosticum` on
  `regula-mala`, `declaratio-mala`, `selector-malus`)
- Regenerate: `css/fontes/css_registrum_coctum.{h,c}`
- Modify: `css/compile_probationes.sh:131` (`materia_diagnostica`)
- Create: `css/probationes/probatio_css_diagnostica.c`
- Modify: `css/CLAUDE.md` if present, else the css worklog

**Interfaces:**
- Produces: `CSS_DIAGNOSTICA`.

- [x] **Step 1: Declarations** (erratum): `regula-mala`
  `diagnosticum="regula legi non potuit"`; `declaratio-mala`
  `diagnosticum="declaratio legi non potuit"`; `selector-malus`
  `diagnosticum="selector legi non potuit"`. Regenerate; canon check;
  natura; css canon seal unchanged.

- [x] **Step 2: Gate** `probatio_css_diagnostica.c` (`./silva/scribe.sh`).

```c
/* probatio_css_diagnostica.c - Diagnostica css ex declaratione (plan
 * materia-sedes B4)
 *
 * Numerus diagnosticorum derivatorum == numerus nodorum generum
 * malorum (ambulatione HIC scripta) super corpus; fixa parva cum
 * sedibus pinnatis (post inspectionem oculi).
 */

#include "latina.h"
#include "credo.h"
#include "css_arbor.h"
#include "css_registrum.h"
#include "materia_diagnostica.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* CORPUS[] = {
    "probationes/fixa/css/componentia_2026-08-18.css",
    "probationes/fixa/css/thema_2026-08-18.css",
    "probationes/fixa/css/adversarius.css",
    "probationes/fixa/css/adversarius_2.css",
    "lib/speculum_assets/speculum.css",
    "knotapel/atlas/web/style.css"
};

interior i32
_mala_numerare (
    constans MateriaValor* valor)
{
    i32 numerus = ZEPHYRUM;
    i32 k;

    si (valor->genus == MATERIA_VALOR_NODUS && valor->datum.nodus != NIHIL)
    {
        constans MateriaNodus* n = valor->datum.nodus;

        si (   n->genus == (s32)CSS_GENUS_REGULA_MALA
            || n->genus == (s32)CSS_GENUS_DECLARATIO_MALA
            || n->genus == (s32)CSS_GENUS_SELECTOR_MALUS)
        {
            numerus++;
        }
        per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
        {
            numerus += _mala_numerare(&n->loci[k]);
        }
    }
    alioquin si (valor->genus == MATERIA_VALOR_LISTA)
    {
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*valor,
                k);

            si (e != NIHIL)
            {
                numerus += _mala_numerare(e);
            }
        }
    }
    redde numerus;
}
```

  `principale`: for each `CORPUS` path (read with the
  `_plagulam_legere` of `probatio_css_stml.c`, lines 48-90, copied),
  parse, derive with `materia_diagnostica_derivare(piscina, radix,
  &CSS_REGISTRUM, &CSS_DIAGNOSTICA, NIHIL, NIHIL)`, and
  `CREDO_AEQUALIS_I32 (xar_numerus(d), _mala_numerare(&radix_valor))`
  where `radix_valor = materia_valor_nodus(radix)`; print per file both
  numbers. Then the three inline fixtures `"a { color: red; } }"`,
  `"a { : x; }"`, `"{ }"`: derive, PRINT every record (`codex`, `linea`,
  `columna`) on the first run, `CREDO_VERUM (xar_numerus(d) > ZEPHYRUM)`
  only; show the printout to Fran with the sources; then replace the
  printout with pinned `strcmp`/`CREDO_AEQUALIS_I32` on those values.
  Use the generated enum names as they appear in
  `css_registrum_coctum.h` (the three above follow the coctor's
  `_maiusculum` rule). Add `materia_diagnostica` to
  `css/compile_probationes.sh`'s module list.

- [x] **Step 3: Plant, commit.** Plant: delete one `diagnosticum`
  attribute by hand, regenerate → red; restore → green. Commit via
  `silva.commissio` with portae `['css']`.

**Executed 2026-09-17.** Declarations and the corpus half as planned;
seal unmoved, all nine existing css gates green through the
regeneration. TWO plan errors, both found by running it:

1. **`selector-malus` is unreachable from the stylesheet tree.** It is
   built only by `css_selector.c`, in the ANALYSIS tree that shares
   tokens but never nodes (B9, deliberate). A walk over
   `css_arbor_parsare`'s result can never find one, so as planned a
   THIRD of the declarations would have been untested and its plant
   would have stayed green — the dead-gate shape B3 step 6 hit the same
   day. Added a selector section using the `_selectorem_parsare` helper
   from `probatio_css_selector.c`; the plant now fires.
2. **`{ }` produces no diagnostic**, so the planned third fixture's
   assertion was false. An empty prelude is valid to "consume a
   qualified rule"; the SELECTOR grammar is what objects. Replaced by
   the selector section.

Pinned after Fran's inspection (byte ends exclusive): `a { color: red;
} }` → regula-mala 1:19, 18–19; `a { : x; }` → declaratio-mala 1:5,
4–8; `1` → selector-malus 1:1, 0–1; `a, 1` → selector-malus 1:4, 3–4
(the `1`, not the `a`); `..` → TWO selector-malus, 1:1 and 1:2. Gate
70/70. Named honestly: five of six corpus files yield zero diagnostics,
so that half asserts `0 == 0` — the fixtures carry the gate. Three
plants (one per declaration), each red, all green on revert.
---

## Task B5: The printer, the instrument, pythonica

**Files:**
- Create: `include/excerptum.h`, `lib/excerptum.c`, `probationes/probatio_excerptum.c`
- Run: `./tools/compile_tests_fontes_generare.sh`
- Create: `tools/diagnostica.c`, `tools/diagnostica.sh`, `tools/diagnostica_fumus.sh`
- Modify: `pythonica/silva.py` (`PORTAE`, `diagnostica`), `pythonica/probatio_silva.py`,
  `pythonica/README.md`
- Modify: MEMORY bench line (closure)

**Interfaces:**
- Produces: `b32 excerptum_scribere(ChordaAedificator* exitus, constans
  character* fons, i32 mensura, s32 initium, s32 finis, i32 linea)`;
  instrument contract `./tools/diagnostica.sh <viae…> [-machina]
  [-sine-excerpto]` (exit 0 no erratum, 1 erratum, 2 nothing judged;
  TSV `via linea columna linea_finis columna_finis initium finis
  gravitas codex causa`); `silva.diagnostica(viae)` →
  `[Diagnosticum(via, linea, columna, linea_finis, columna_finis,
  gravitas, codex, causa, textus)]`; `PORTAE['diagnostica']`.

- [ ] **Step 1: Failing printer test** (root probatio; `probatio_piscina.c`
  pattern). Expected strings (gutter = digits of the line, at least 4;
  then `" | "`):

```c
    _probare(piscina, "int x = 1;\n", 4, 5, 1,
        "   1 | int x = 1;\n     |     ^\n");
    _probare(piscina, "\tab\n", 1, 3, 1,
        "   1 | \tab\n     | \t^~\n");
    /* UTF-8: e acutum duo octeti, unus character */
    _probare(piscina, "\xC3\xA9 = x\n", 3, 4, 1,
        "   1 | \xC3\xA9 = x\n     |   ^\n");
    _probare(piscina, "abc", 3, 3, 1,
        "   1 | abc\n     |    ^\n");
    _probare(piscina, "abc\nd", 1, 5, 1,
        "   1 | abc\n     |  ^~\n");
    _probare(piscina, "ab\r\n", 0, 2, 1,
        "   1 | ab\n     | ^~\n");
    _probare(piscina, "q\n", 0, 1, 12345,
        "12345 | q\n      | ^\n");
    CREDO_FALSUM (excerptum_scribere(a, "ab", 2, (s32)3, (s32)3, 1));
```

  where `_probare` builds a `ChordaAedificator`, calls
  `excerptum_scribere`, and compares the result with
  `CREDO_CHORDA_AEQUALIS_LITERIS`.

- [ ] **Step 2: The printer.**

```c
/* excerptum.h - Excerptum fontis cum signo sub tractu (forma
 * diagnosticorum compilatorum): margo numeri lineae, linea fontis
 * (CR sublato, moderatores praeter tabulam ut spatium), deinde
 * '^' ad initium et '~' per characterem UTF-8 usque ad finem
 * tractus aut lineae. Tabula in margine sub linea servatur, ergo
 * signum columnam veram tenet. */
#ifndef EXCERPTUM_H
#define EXCERPTUM_H

#include "latina.h"
#include "chorda_aedificator.h"

/* FALSUM si initium extra [0, mensura] aut linea ZEPHYRUM. */
b32
excerptum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32  finis,
                   i32  linea);

#endif /* EXCERPTUM_H */
```

```c
/* excerptum.c - Vide excerptum.h. */

#include "excerptum.h"

interior b32
_continuatio (
    character c)
{
    redde (b32)((((i32)(insignatus character)c) & 0xC0) == 0x80);
}

interior vacuum
_marginem (
    ChordaAedificator* exitus,
                  i32  linea,
                  b32  numerus)
{
    character cifrae[XVI];
          i32 n = ZEPHYRUM;
          i32 latitudo;
          i32 k;
          i32 valor = linea;

    fac
    {
        cifrae[n] = (character)('0' + (valor % X));
        valor = valor / X;
        n++;
    } dum (valor > ZEPHYRUM && n < XV);
    latitudo = n < IV ? (i32)IV : n;
    per (k = ZEPHYRUM; k < latitudo - (numerus ? n : ZEPHYRUM); k++)
    {
        chorda_aedificator_appendere_character(exitus, ' ');
    }
    si (numerus)
    {
        per (k = n; k > ZEPHYRUM; k--)
        {
            chorda_aedificator_appendere_character(exitus, cifrae[k - I]);
        }
    }
    chorda_aedificator_appendere_literis(exitus, " | ");
}

b32
excerptum_scribere (
     ChordaAedificator* exitus,
    constans character* fons,
                   i32  mensura,
                   s32  initium,
                   s32  finis,
                   i32  linea)
{
    s32 principium;
    s32 terminus;
    s32 finis_lineae;
    s32 i;

    si (   exitus == NIHIL || fons == NIHIL || linea == ZEPHYRUM
        || initium < ZEPHYRUM || initium > (s32)mensura)
    {
        redde FALSUM;
    }
    si (finis < initium)
    {
        finis = initium;
    }
    principium = initium;
    dum (principium > ZEPHYRUM && fons[principium - I] != '\n')
    {
        principium--;
    }
    terminus = initium;
    dum (terminus < (s32)mensura && fons[terminus] != '\n')
    {
        terminus++;
    }
    si (terminus > principium && fons[terminus - I] == '\r')
    {
        terminus--;
    }
    _marginem(exitus, linea, VERUM);
    per (i = principium; i < terminus; i++)
    {
        character c = fons[i];

        si (c != '\t' && (i32)(insignatus character)c < 0x20)
        {
            c = ' ';
        }
        chorda_aedificator_appendere_character(exitus, c);
    }
    chorda_aedificator_appendere_character(exitus, '\n');
    _marginem(exitus, linea, FALSUM);
    per (i = principium; i < initium && i < terminus; i++)
    {
        si (fons[i] == '\t')
        {
            chorda_aedificator_appendere_character(exitus, '\t');
        }
        alioquin si (!_continuatio(fons[i]))
        {
            chorda_aedificator_appendere_character(exitus, ' ');
        }
    }
    chorda_aedificator_appendere_character(exitus, '^');
    finis_lineae = finis < terminus ? finis : terminus;
    i = initium;
    si (i < finis_lineae)
    {
        i++;
        dum (i < finis_lineae && _continuatio(fons[i]))
        {
            i++;
        }
    }
    per (; i < finis_lineae; i++)
    {
        si (!_continuatio(fons[i]))
        {
            chorda_aedificator_appendere_character(exitus, '~');
        }
    }
    chorda_aedificator_appendere_character(exitus, '\n');
    redde VERUM;
}
```

  (Check `_marginem`'s gutter: the second line prints `latitudo` spaces
  then `" | "`; the test strings above are the contract. Fix the code to
  the strings, never the strings to the code. Then
  `./tools/compile_tests_fontes_generare.sh` and `./compile_tests.sh
  excerptum` green; plant `'^'` → `'*'` via `silva.planta` → red.)

- [ ] **Step 3: The instrument** `tools/diagnostica.c`
  (`./silva/scribe.sh`). The machine TSV carries the RAW codex; the human
  line prefixes `<grammatica>:` to a codex without `:`.

```c
/* diagnostica.c - Diagnostica plagularum per clientes materiae
 * (tools/diagnostica.sh; materia-sedes-spec par. XI)
 *
 * Usus: diagnostica <plagula...> [-machina] [-sine-excerpto]
 * Suffixum clientem eligit (.sh crusta, .css css). Quaeque plagula:
 * parsura, diagnostica derivata (et emissa), deinde proiectio una -
 * refutatio scriptoris ut materia:scriptura. Exitus: 0 nullum
 * erratum, 1 erratum, 2 nihil iudicatum.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "excerptum.h"
#include "materia_arbor.h"
#include "materia_diagnostica.h"
#include "materia_lexicon.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "css_arbor.h"
#include "css_lexicon.h"
#include "css_registrum.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    i32 errata;
    i32 monita;
    i32 plagulae;
} Summa;

/* ... _plagulam_legere copied from crusta/instrumenta/arbor.c:21-59 ... */

interior b32
_suffixum (
    constans character* via,
    constans character* suffixum)
{
    size_t lv = strlen(via);
    size_t ls = strlen(suffixum);

    redde (b32)(lv >= ls && strcmp(via + lv - ls, suffixum) == ZEPHYRUM);
}

interior vacuum
_diagnosticum_imprimere (
                  Piscina* piscina,
       constans character* via,
       constans character* grammatica,
       constans character* fons,
                      i32  mensura,
    constans MateriaDiagnosticum* d,
                      b32  machina,
                      b32  excerptum,
                   Summa* summa)
{
    b32 monitum = (b32)(d->gravitas == (s32)MATERIA_GRAVITAS_MONITUM);
    b32 substrati = (b32)(strchr(d->codex, ':') != NIHIL);

    si (monitum)
    {
        summa->monita++;
    }
    alioquin
    {
        summa->errata++;
    }
    si (machina)
    {
        imprimere("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\n", via,
            (integer)d->tractus.linea, (integer)d->tractus.columna,
            (integer)d->tractus.linea_finis,
            (integer)d->tractus.columna_finis,
            (integer)d->tractus.initium, (integer)d->tractus.finis,
            monitum ? "monitum" : "erratum", d->codex, d->causa);
        redde;
    }
    imprimere("%s:%d:%d: [%s] %s%s%s\n  %s\n", via,
        (integer)d->tractus.linea, (integer)d->tractus.columna,
        monitum ? "monitum" : "erratum",
        substrati ? "" : grammatica, substrati ? "" : ":", d->codex,
        d->causa);
    si (excerptum && d->tractus.initium >= ZEPHYRUM)
    {
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            (memoriae_index)CCLVI);

        si (   a != NIHIL
            && excerptum_scribere(a, fons, mensura, d->tractus.initium,
                   d->tractus.finis, d->tractus.linea))
        {
            chorda textus = chorda_aedificator_finire(a);

            fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
        }
    }
}

interior b32
_plagulam_iudicare (
    constans character* via,
                   b32  machina,
                   b32  excerptum,
                Summa* summa)
{
                 Piscina* piscina;
              character* fons;
                     i32  mensura = ZEPHYRUM;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
    MateriaArborScriptura  s;
            CrustaParsura  relatio;
            MateriaNodus* radix       = NIHIL;
                     Xar* diagnostica = NIHIL;
      constans character* grammatica  = NIHIL;
                     i32  k;

    piscina = piscina_generare_dynamicum("diagnostica", 4194304);
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "diagnostica: plagula illegibilis: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    si (_suffixum(via, ".sh"))
    {
        grammatica = "crusta";
        si (materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
                &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM,
                &ratum, grammatica);
            memset(&relatio, ZEPHYRUM, magnitudo(relatio));
            radix = crusta_arbor_parsare(piscina, fons, mensura,
                &CRUSTA_BASH, &relatio);
            diagnostica = radix != NIHIL
                ? crusta_diagnostica(piscina, radix, &relatio) : NIHIL;
        }
    }
    alioquin si (_suffixum(via, ".css"))
    {
        grammatica = "css";
        si (materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON,
                &iudicium))
        {
            materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM,
                &ratum, grammatica);
            radix = css_arbor_parsare(piscina, fons, mensura);
            diagnostica = radix != NIHIL
                ? materia_diagnostica_derivare(piscina, radix,
                      &CSS_REGISTRUM, &CSS_DIAGNOSTICA, NIHIL, NIHIL)
                : NIHIL;
        }
    }
    alioquin
    {
        fprintf(stderr, "diagnostica: suffixum sine cliente: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    si (radix == NIHIL || diagnostica == NIHIL)
    {
        fprintf(stderr, "diagnostica: parsura aut derivatio fracta: %s\n",
            via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(diagnostica); k++)
    {
        _diagnosticum_imprimere(piscina, via, grammatica, fons, mensura,
            (constans MateriaDiagnosticum*)xar_obtinere(diagnostica, k),
            machina, excerptum, summa);
    }
    s = materia_arbor_proicere_nodum(piscina, radix, &consilium);
    si (!s.successus)
    {
        MateriaDiagnosticum r;

        memset(&r, ZEPHYRUM, magnitudo(r));
        r.gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
        r.codex     = MATERIA_CODEX_SCRIPTURA;
        r.causa     = s.causa != NIHIL ? s.causa : "scriptura fracta";
        r.tractus   = s.tractus;
        _diagnosticum_imprimere(piscina, via, grammatica, fons, mensura,
            &r, machina, excerptum, summa);
    }
    summa->plagulae++;
    piscina_destruere(piscina);
    redde VERUM;
}

integer
principale (
      integer   argc,
    character** argv)
{
      Summa summa;
        b32 machina    = FALSUM;
        b32 excerptum  = VERUM;
    integer i;

    memset(&summa, ZEPHYRUM, magnitudo(summa));
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sine-excerpto") == ZEPHYRUM)
        {
            excerptum = FALSUM;
        }
    }
    per (i = I; i < argc; i++)
    {
        si (argv[i][0] != '-')
        {
            (vacuum)_plagulam_iudicare(argv[i], machina, excerptum,
                &summa);
        }
    }
    fprintf(stderr, "%d diagnostica (erratum %d, monitum %d) in %d "
        "plagulis\n", (integer)(summa.errata + summa.monita),
        (integer)summa.errata, (integer)summa.monita,
        (integer)summa.plagulae);
    si (summa.plagulae == ZEPHYRUM)
    {
        fprintf(stderr, "usus: diagnostica <plagula...> [-machina] "
            "[-sine-excerpto] (nihil iudicatum)\n");
        redde II;
    }
    redde summa.errata > ZEPHYRUM ? I : ZEPHYRUM;
}
```

  `tools/diagnostica.sh`:

```bash
#!/bin/bash
# tools/diagnostica.sh - diagnostica plagularum per clientes materiae
#
# Usage:  ./tools/diagnostica.sh <plagula...> [-machina] [-sine-excerpto]
# Exit:   0 nullum erratum | 1 erratum | 2 nihil iudicatum
# Obiecta SUA in build/diagnostica/: aedificia crustae et css obiecta
# materiae et bibliothecae sua quaeque ferunt - coniuncta colliderent.
set -u
RADIX_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")/.." && pwd)"
BUILD_DIR="$RADIX_DIR/build/diagnostica"
BIN="$BUILD_DIR/diagnostica"
source "$RADIX_DIR/tools/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")
declare -a INCLUDE_FLAGS=("-I$RADIX_DIR/include" "-I$RADIX_DIR/materia/fontes" "-I$RADIX_DIR/crusta/fontes" "-I$RADIX_DIR/css/fontes")
mkdir -p "$BUILD_DIR" || exit 2
CAPITA_NOVA=""
if [ -f "$BIN" ]; then
    CAPITA_NOVA="$(find "$RADIX_DIR/include" "$RADIX_DIR/materia/fontes" "$RADIX_DIR/crusta/fontes" "$RADIX_DIR/css/fontes" -name '*.h' -newer "$BIN" 2>/dev/null | head -1)"
fi
OBJ=""
compilare () {
    # in aequalitate temporum RECOMPILA ('-nt' secundis mensuratur)
    if [ ! -f "$2" ] || ! [ "$2" -nt "$1" ] || [ -n "$CAPITA_NOVA" ]; then
        rm -f "$2"
        clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$1" -o "$2" >&2 || {
            echo "diagnostica: compilatio fracta: $1" >&2; exit 2; }
    fi
    OBJ="$OBJ $2"
}
for f in piscina chorda chorda_aedificator xar friatio tabula_dispersa \
         internamentum stml stml_macros selectio similitudo canon credo \
         processus_posix css_lexema excerptum; do
    compilare "$RADIX_DIR/lib/$f.c" "$BUILD_DIR/$f.o"
done
for m in materia_lexicon materia_token materia_nodus materia_scribere \
         materia_arbor materia_arbor_aequalitas materia_coctor \
         materia_diagnostica; do
    compilare "$RADIX_DIR/materia/fontes/$m.c" "$BUILD_DIR/$m.o"
done
for src in "$RADIX_DIR"/crusta/fontes/*.c "$RADIX_DIR"/css/fontes/*.c; do
    compilare "$src" "$BUILD_DIR/$(basename "$src" .c).o"
done
if [ ! -f "$BIN" ] || ! [ "$BIN" -nt "$RADIX_DIR/tools/diagnostica.c" ] \
   || [ -n "$(find "$BUILD_DIR" -name '*.o' -newer "$BIN" 2>/dev/null | head -1)" ]; then
    rm -f "$BIN"
    clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "$RADIX_DIR/tools/diagnostica.c" $OBJ -o "$BIN" >&2 || {
        echo "diagnostica: ligatio fracta" >&2; exit 2; }
fi
exec "$BIN" "$@"
```

  A link error naming a missing library object gets that object added
  to the `lib` list (name it in the commit message); a DUPLICATE symbol
  means a list names something a client file already defines — remove
  it. Run it on `crusta/arbor.sh` and on a temp `{ echo a` by eye.

- [ ] **Step 4: The fumus** `tools/diagnostica_fumus.sh`:

```bash
#!/bin/bash
# tools/diagnostica_fumus.sh - porta natalis tools/diagnostica.sh
#
# I.   domus: plagulae .sh tractae (fixa crustae pathologica exclusa)
#      sine erratis, exitus 0;
# II.  plagula plantata '{ echo a': exitus 1, linea prima exacta;
# III. suffixum sine cliente solum: exitus 2 nominatus;
# IV.  css plantata (B4): exitus 1, codex css.
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT

git ls-files '*.sh' | grep -v '^probationes/fixa/crusta/' > "$T/domus.lst"
./tools/diagnostica.sh -machina $(cat "$T/domus.lst") > "$T/domus.tsv" 2> "$T/domus.err"
rc=$?
[ "$rc" -eq 0 ]; credo $? "I. domus exitus 0 ($(wc -l < "$T/domus.lst") plagulae, rc $rc)"
! grep -q $'\terratum\t' "$T/domus.tsv"; credo $? "I. domus sine erratis"

printf '{ echo a' > "$T/plantata.sh"
./tools/diagnostica.sh "$T/plantata.sh" > "$T/plantata.out" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ]; credo $? "II. plantata exitus 1 (rc $rc)"
[ "$(head -1 "$T/plantata.out")" = "$T/plantata.sh:1:9: [erratum] crusta:grex/tok_clausura" ]
credo $? "II. linea prima: $(head -1 "$T/plantata.out")"

printf 'x\n' > "$T/x.txt"
./tools/diagnostica.sh "$T/x.txt" > /dev/null 2> "$T/x.err"
rc=$?
[ "$rc" -eq 2 ] && grep -q 'suffixum sine cliente' "$T/x.err"; credo $? "III. suffixum sine cliente: exitus 2 nominatus (rc $rc)"

printf 'a { color: red; } }' > "$T/plantata.css"
./tools/diagnostica.sh -machina "$T/plantata.css" > "$T/css.tsv" 2>/dev/null
rc=$?
[ "$rc" -eq 1 ] && grep -q $'\terratum\t' "$T/css.tsv"; credo $? "IV. css plantata exitus 1 (rc $rc)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus diagnostica: sanum"; exit 0; fi
echo "fumus diagnostica: FRACTUM ($fracta)"; exit 1
```

  IV asserts the exit and one erratum; if B4 pinned a codex for this
  fixture, add `grep -q '<codex>'` with that exact codex. Add
  `'diagnostica': (['./tools/diagnostica_fumus.sh'], r'fumus
  diagnostica: (sanum|FRACTUM)')` to `PORTAE`. Plant by hand: change II's
  expected column to `1:8` → FRACTUM; revert → sanum.

- [ ] **Step 5: pythonica.**

```python
Diagnosticum = namedtuple('Diagnosticum', 'via linea columna linea_finis '
                          'columna_finis gravitas codex causa textus')


def diagnostica(viae):
    """diagnostica plagularum (.sh crusta, .css css) per
    ./tools/diagnostica.sh -machina: declarata in registro clientis et
    per materiam derivata, plus refutatio scriptoris
    (materia:scriptura). viae: via, lista, aut forma git. Reddit
    [Diagnosticum] ordine plagularum et octetorum; textus = segmentum
    fontis. Refutat si nihil iudicatum (exit 2)."""
    lista = _viae_expandere(viae)
    if not lista:
        raise SilvaError('diagnostica: nullae plagulae (%r)' % (viae,))
    absolutae = {_absoluta(v): v for v in lista}
    r = _curre(['./tools/diagnostica.sh', '-machina']
               + list(absolutae))
    if r.returncode == 2:
        raise SilvaError('diagnostica: nihil iudicatum: %s'
                         % (r.stderr or '').strip()[-200:])
    fontes = {}
    exitus = []
    for linea in r.stdout.splitlines():
        campi = linea.split('\t')
        if len(campi) != 10:
            raise SilvaError('diagnostica: linea TSV prava: %r' % linea)
        via = campi[0]
        initium, finis = int(campi[5]), int(campi[6])
        textus = ''
        if initium >= 0:
            if via not in fontes:
                with open(via, 'rb') as f:
                    fontes[via] = f.read()
            textus = fontes[via][initium:finis].decode('utf-8', 'replace')
        exitus.append(Diagnosticum(absolutae.get(via, via), int(campi[1]),
                                   int(campi[2]), int(campi[3]),
                                   int(campi[4]), campi[7], campi[8],
                                   campi[9], textus))
    return exitus
```

  Tests in `probatio_silva.py`: a temp `{ echo a` → one record
  `(linea 1, columna 9, gravitas 'erratum', codex 'grex/tok_clausura',
  textus '')`; a temp `"a"` clean `.sh` → `[]`; `x.txt` alone →
  SilvaError. `./pythonica/probare.sh` green; plant by hand (swap
  `campi[1]`/`campi[2]`) → red; revert.

- [ ] **Step 6: Docs and commit.** `pythonica/README.md`; the root
  `CLAUDE.md` is Fran's — do not edit; the bench line goes into memory
  at closure. Commit via `silva.commissio` with portae `[('radix',
  'excerptum'), 'diagnostica', 'pythonica']`.

---

## Task C: Closure

**Files:**
- Modify: `project-specs/materia-sedes-spec.md` (new §16 "As built":
  deltas D1–D10 as executed, numbers, what the pins say)
- Modify: `project-specs/materia-sedes-plan.md` (STATUS CLOSED, each
  task's "Executed" note)
- Modify: `materia/phase-log.md` (RELATIO + debrief)
- Modify: `materia/CLAUDE.md`, `crusta/CLAUDE.md` (final state)

- [ ] **Step 1: Records.** RELATIO in `materia/phase-log.md` (commits,
  per-client `sedes` element counts, size growth, A3 row deltas and
  timing, crusta class totals and the two new pins, the instrument's
  house run), with the house instrument debrief (ADHIBITA / FRUCTUS /
  ASPERITATES / DESIDERATA).
- [ ] **Step 2: Ledger.** Desideratum `01M2PSWHM0` → `impletum` with a
  note naming the commits. New desiderata (only those still true):
  html parse-error emission (html5lib positions pinned rising); legatus
  LSP publishing diagnostics for `.sh`/`.css`; character and UTF-16
  columns at the edges; `-sedes` on `briar/arbor.sh`; diagnostics as
  elements of the view. Quaestio `01M2NT4CDD` gets a note with the
  gravity pin's value.
- [ ] **Step 3: Memory.** `materia-fork.md` (Sedes + Diagnostica
  paragraph); `bash-parser-project.md` NEXT pointer → the crusta lint
  over `silva.exemplaria` with rows that now carry lines; `MEMORY.md`
  crusta and materia lines; the bench: "where is the problem in this
  `.sh`/`.css`?" → `./tools/diagnostica.sh`.
- [ ] **Step 4: Commit** docs via `silva.commissio` with no portae (all
  gates already green on the unchanged tree).
