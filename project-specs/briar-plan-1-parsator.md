# briar plan 1 — the parser (P0–P2 of briar-spec.md)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** A materia client that parses `.thistle` files into a byte-exact
tree, projects it to canon-judged STML, and hangs markdown and STML inner
trees off its regions — every gate green, `./briar/arbor.sh x.thistle`
working. No fabrica, no binary, no silva yet (plan 2).

**Architecture:** `briar/fontes/` beside css and md. Line table → per-line
classification (column-0 tags, markdown fences) → regions coalesced into a
five-genus tree (documentum, interpres, prosa, regio, elementum) whose
tokens cover every byte. Projection through materia's arbor writer with
md's origin-hook pattern; a hand-written `briar.canon`; a nexus module
that parses prose with md and STML regions/open tags with stml, keyed by
node identity, carrying `.thistle` line offsets.

**Tech Stack:** C89 under the house flags, materia (`materia/fontes`),
md (`md/fontes`), stml, canon, credo; bash runner; pythonica gate tables.

**Spec:** `project-specs/briar-spec.md` v1.1 (this plan implements §2,
§3, §6 rows registrum/lexema/arbor/stml/canon/totalitas/nexus/computus,
§8 P0–P2). Two amendments to the spec are made by this plan and recorded
in Task 10: (a) an STML element's extent is its column-0 `</name>` line
or its single self-closing line, not a lexeme walk; (b) regio loci are
named `apertum/titulus/contentum/clausum/vitium` (no underscores, canon
element names).

## Global Constraints

- **Work in the worktree** `/Users/francisarant/Documents/projects/rhubarb-briar`
  (branch `rhubarb-briar`). Every command below runs from that directory.
- **C89, house flags** from `tools/vexilla.sh` (`-std=c89 -pedantic -Wall
  -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual
  -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings …`).
  Declarations at the top of every block. No `//` comments.
- **Latin identifiers, comments, messages.** `latina.h` macros are
  keywords: never use `nomen`, `casus`, `registrum`, `magnitudo`, or the
  single capitals `I V X L C D M` as identifiers; `I II III IV … ZEPHYRUM`
  are the house numerals for small constants.
- **`chorda` is `{i32 mensura; i8* datum}`, NOT NUL-terminated.** `i32`
  and `i64` are unsigned; `s32`/`s64` signed.
- **New C files are written with** `./silva/scribe.sh <via> <<'EOF' … EOF`
  (from the worktree; NOT `~/.bin/scribe`, which refuses worktree paths).
  Exit 0 ACCIPE, 1 REICE (file kept, fix it), 3 RECUSO.
- **Never format mid-edit**; the pre-commit formats changed functions.
- **Tests use credo** (`credo_aperire`, `CREDO_*`, `credo_imprimere_compendium`,
  `credo_omnia_praeterierunt`); bail-outs are `CREDO_CULPA("…")`. Every
  gate must be seen RED once (the "plant" step) before it counts.
- **Commits:** explicit paths only, through pythonica from the worktree:

  ```sh
  cd /Users/francisarant/Documents/projects/rhubarb-briar && python3 - <<'EOF'
  import sys; sys.path.insert(0, 'pythonica'); import silva
  h, r = silva.commissio_umbra("<nuntius Latinus>", ["via/una", "via/altera"], ['briar'])
  print(h, r)
  EOF
  ```
  Add `'pythonica'` to the gate list whenever `pythonica/silva.py` is among
  the paths. `commissio_umbra` runs the gate in a shadow clone; a red gate
  raises and commits nothing.
- **Never** call `silva.umbrae_purgare()` or run `tabularii`/`fori`/
  `./gesta/frigida.sh` from the worktree (see MEMORY: worktree-briar-shared-memory).
- Runner contract: exit 0 sane, 1 failures, **2 = NOTHING RAN**.

---

## File structure

```
briar/
  compile_probationes.sh          runner (Task 1)             — clones md's
  arbor.sh                        projection CLI (Task 5)
  computus.sh                     bench CLI (Task 8)
  CLAUDE.md                       orientation (Task 10)
  fontes/
    briar_registrum.h/.c          genera + loci tables (Task 1)
    briar_lexicon.h/.c            token genera (Task 1)
    briar_lexema.h/.c             line table, classification, fences, factory (Task 2)
    briar_arbor.h/.c              regions → tree, accessors (Tasks 3–4)
    briar_stml.h/.c               projection consilium + origin hook (Task 5)
    briar_computus.h/.c           bench twin (Task 8)
    briar_nexus.h/.c              inner trees by identity (Tasks 9–10)
    briar.worklog.md              findings (Task 10, appended throughout)
  grammatica/briar.canon          hand-written canon (Task 6)
  instrumenta/arbor.c             projection tool (Task 5)
  instrumenta/computus.c          bench tool (Task 8)
  probationes/
    probatio_briar_registrum.c    Task 1
    probatio_briar_lexema.c       Task 2
    probatio_briar_arbor.c        Tasks 3–4
    probatio_briar_stml.c         Task 5
    probatio_briar_canon.c        Task 6
    probatio_briar_totalitas.c    Task 7
    probatio_briar_computus.c     Task 8
    probatio_briar_nexus.c        Tasks 9–10
    fixa/thistle/*.thistle        fixtures (Task 2; adversa Tasks 3–4)
    fixa/computus/basis.tsv       golden (Task 8)
    fixa/FONTES.md                fixture inventory (Task 2)
Modified: pythonica/silva.py (four table rows, Task 1)
```

Vocabulary fixed for all tasks (names unsealed by Fran, but the plan
uses them consistently):

| thing | name |
|---|---|
| genera | `documentum` `interpres` `prosa` `regio` `elementum` |
| documentum loci | `interpres` NODUS · `partes` LISTA_NODUS · `finis` TOKEN · `vitium` INDEX |
| interpres / prosa loci | `tok` TOKEN |
| regio loci | `apertum` TOKEN · `titulus` TOKEN (derived) · `contentum` TOKEN (absent = empty) · `clausum` TOKEN (absent = unterminated) · `vitium` INDEX |
| elementum loci | `tok` TOKEN · `titulus` TOKEN (derived) · `vitium` INDEX |
| vitia (INDEX values) | 0 nullum · 1 regio-non-clausa · 2 elementum-non-clausum · 3 saeptum-non-clausum |
| token genera | `FINIS` `INTERPRES` `TAG_APERTUM` `TAG_CLAUSUM` `CRUDUM` `PROSA` `STML` `DERIVATUM`, tag prefix `briar-` |
| fons_index | `BRIAR_FONS_PLAGULAE` 0 · `BRIAR_FONS_DERIVATUS` 1 |

---

### Task 1: Registry, lexicon, runner, first gate

**Files:**
- Create: `briar/fontes/briar_registrum.h`, `briar/fontes/briar_registrum.c`
- Create: `briar/fontes/briar_lexicon.h`, `briar/fontes/briar_lexicon.c`
- Create: `briar/compile_probationes.sh`
- Create: `briar/probationes/probatio_briar_registrum.c`
- Modify: `pythonica/silva.py` (PORTAE ~1449, FORMAE ~1489, praef map ~2197, SUITAE ~2286)

**Interfaces:**
- Produces: `BriarGenus`, `BriarLocus*` enums, `BriarVitium`, `BRIAR_REGISTRUM`;
  `BriarLexGenus`, `BRIAR_LEXICON`, `BRIAR_FONS_PLAGULAE`, `BRIAR_FONS_DERIVATUS`.
- Consumed by every later task.

- [ ] **Step 1: Write the registry header**

```sh
mkdir -p briar/fontes briar/probationes/fixa/thistle briar/grammatica briar/instrumenta
./silva/scribe.sh briar/fontes/briar_registrum.h <<'EOF'
/* briar_registrum.h - Vocabularium nodorum thistle (cliens materiae)
 *
 * Quinque genera, XIV loci, series plana ut css/md. Regio = tag
 * crudus in columna 0 usque ad clausuram in columna 0; elementum =
 * tag STML in columna 0 usque ad clausuram in columna 0 aut linea
 * una se-claudens; prosa = cetera (markdown); interpres = linea '#!'
 * prima. Loci INDEX 'vitium' valores BriarVitium ferunt.
 */

#ifndef BRIAR_REGISTRUM_H
#define BRIAR_REGISTRUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    BRIAR_GENUS_DOCUMENTUM = 0,
    BRIAR_GENUS_INTERPRES,
    BRIAR_GENUS_PROSA,
    BRIAR_GENUS_REGIO,
    BRIAR_GENUS_ELEMENTUM,

    BRIAR_GENUS_NUMERUS
} BriarGenus;

nomen enumeratio {
    BRIAR_DOCUMENTUM_INTERPRES = 0,   /* NODUS? interpres */
    BRIAR_DOCUMENTUM_PARTES,          /* LISTA_NODUS prosa|regio|elementum */
    BRIAR_DOCUMENTUM_FINIS,           /* TOKEN FINIS */
    BRIAR_DOCUMENTUM_VITIUM           /* INDEX BriarVitium */
} BriarLocusDocumenti;

nomen enumeratio {
    BRIAR_INTERPRES_TOK = 0           /* TOKEN INTERPRES (linea tota) */
} BriarLocusInterpretis;

nomen enumeratio {
    BRIAR_PROSA_TOK = 0               /* TOKEN PROSA (cursus totus) */
} BriarLocusProsae;

nomen enumeratio {
    BRIAR_REGIO_APERTUM = 0,          /* TOKEN TAG_APERTUM (linea tota) */
    BRIAR_REGIO_TITULUS,              /* TOKEN DERIVATUM (nomen) */
    BRIAR_REGIO_CONTENTUM,            /* TOKEN? CRUDUM (absens = vacuum) */
    BRIAR_REGIO_CLAUSUM,              /* TOKEN? TAG_CLAUSUM (absens = non clausa) */
    BRIAR_REGIO_VITIUM                /* INDEX BriarVitium */
} BriarLocusRegionis;

nomen enumeratio {
    BRIAR_ELEMENTUM_TOK = 0,          /* TOKEN STML (extensio tota) */
    BRIAR_ELEMENTUM_TITULUS,          /* TOKEN DERIVATUM (nomen) */
    BRIAR_ELEMENTUM_VITIUM            /* INDEX BriarVitium */
} BriarLocusElementi;

nomen enumeratio {
    BRIAR_VITIUM_NULLUM = 0,
    BRIAR_VITIUM_REGIO_NON_CLAUSA,
    BRIAR_VITIUM_ELEMENTUM_NON_CLAUSUM,
    BRIAR_VITIUM_SAEPTUM_NON_CLAUSUM
} BriarVitium;

externus constans MateriaRegistrumCoctum BRIAR_REGISTRUM;

#endif /* BRIAR_REGISTRUM_H */
EOF
```

- [ ] **Step 2: Write the registry table**

```sh
./silva/scribe.sh briar/fontes/briar_registrum.c <<'EOF'
/* briar_registrum.c - Vide briar_registrum.h. Series LOCORUM plana;
 * genus quodque fenestram suam per loci_offset + loci_numerus nominat.
 * Probatio congruentiam enumerationum cum hac tabula per TITULOS
 * asserit. */

#include "briar_registrum.h"
#include "materia_nodus.h"

hic_manens constans MateriaTabLocus LOCI_BRIAR[] = {
    /* documentum (0..3) */
    { "interpres",  (s32)MATERIA_LOCUS_NODUS },
    { "partes",     (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "finis",      (s32)MATERIA_LOCUS_TOKEN },
    { "vitium",     (s32)MATERIA_LOCUS_INDEX },

    /* interpres (4) */
    { "tok",        (s32)MATERIA_LOCUS_TOKEN },

    /* prosa (5) */
    { "tok",        (s32)MATERIA_LOCUS_TOKEN },

    /* regio (6..10) */
    { "apertum",    (s32)MATERIA_LOCUS_TOKEN },
    { "titulus",    (s32)MATERIA_LOCUS_TOKEN },
    { "contentum",  (s32)MATERIA_LOCUS_TOKEN },
    { "clausum",    (s32)MATERIA_LOCUS_TOKEN },
    { "vitium",     (s32)MATERIA_LOCUS_INDEX },

    /* elementum (11..13) */
    { "tok",        (s32)MATERIA_LOCUS_TOKEN },
    { "titulus",    (s32)MATERIA_LOCUS_TOKEN },
    { "vitium",     (s32)MATERIA_LOCUS_INDEX }
};

hic_manens constans MateriaTabGenus GENERA_BRIAR_NODI[] = {
    /* titulus        offset   numerus */
    { "documentum",   (i32)0,  (i32)4 },
    { "interpres",    (i32)4,  (i32)1 },
    { "prosa",        (i32)5,  (i32)1 },
    { "regio",        (i32)6,  (i32)5 },
    { "elementum",    (i32)11, (i32)3 }
};

constans MateriaRegistrumCoctum BRIAR_REGISTRUM = {
    GENERA_BRIAR_NODI,
    (i32)(magnitudo(GENERA_BRIAR_NODI) / magnitudo(GENERA_BRIAR_NODI[0])),
    LOCI_BRIAR,
    (i32)(magnitudo(LOCI_BRIAR) / magnitudo(LOCI_BRIAR[0]))
};
EOF
```

- [ ] **Step 3: Write the lexicon**

```sh
./silva/scribe.sh briar/fontes/briar_lexicon.h <<'EOF'
/* briar_lexicon.h - Descriptor lexicalis thistle
 *
 * Lexemata OMNIA verbatim praeter FINIS; lineae novae INTRA valores
 * vivunt (cursor materiae eas numerat - _cursorem_promovere), ergo
 * nullum genus LINEA: trivia non exsistunt, quisque octetus uni
 * lexemati pertinet. DERIVATUM (fons I) nomen tagi fert - octetos
 * nullos, emissor eum omittit.
 */

#ifndef BRIAR_LEXICON_H
#define BRIAR_LEXICON_H

#include "latina.h"
#include "materia_lexicon.h"

nomen enumeratio {
    BRIAR_LEX_FINIS = 0,
    BRIAR_LEX_INTERPRES,     /* linea '#!' prima, cum terminatore */
    BRIAR_LEX_TAG_APERTUM,   /* linea tagi aperti, cum terminatore */
    BRIAR_LEX_TAG_CLAUSUM,   /* linea '</nomen>', cum terminatore */
    BRIAR_LEX_CRUDUM,        /* contentum regionis crudae */
    BRIAR_LEX_PROSA,         /* cursus prosae (markdown) */
    BRIAR_LEX_STML,          /* extensio elementi STML tota */
    BRIAR_LEX_DERIVATUM,     /* fons I: nomen tagi */

    BRIAR_LEX_NUMERUS_GENERUM
} BriarLexGenus;

#define BRIAR_FONS_PLAGULAE   ((s32)0)
#define BRIAR_FONS_DERIVATUS  ((s32)1)

externus constans MateriaLexiconCoctum BRIAR_LEXICON;

#endif /* BRIAR_LEXICON_H */
EOF
./silva/scribe.sh briar/fontes/briar_lexicon.c <<'EOF'
/* briar_lexicon.c - Vide briar_lexicon.h. Ordo ordinem BriarLexGenus
 * sequitur; probatio_briar_registrum id per titulos asserit. */

#include "briar_lexicon.h"

hic_manens constans MateriaLexGenus GENERA_BRIAR[] = {
    /* titulus        orthographia  species                munus */
    { "FINIS",        "",     MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "INTERPRES",    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_APERTUM",  NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_CLAUSUM",  NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "CRUDUM",       NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "PROSA",        NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "STML",         NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "DERIVATUM",    NIHIL,  MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM }
};

constans MateriaLexiconCoctum BRIAR_LEXICON = {
    GENERA_BRIAR,
    (i32)(magnitudo(GENERA_BRIAR) / magnitudo(GENERA_BRIAR[0])),
    "briar-",
    (s32)-I   /* nullum spatium canonicum - trivia non exsistunt */
};
EOF
```

- [ ] **Step 4: Write the runner (clone of md's, with md objects consumed)**

Copy `md/compile_probationes.sh` to `briar/compile_probationes.sh`, then apply
exactly these substitutions and edits:

```sh
cp md/compile_probationes.sh briar/compile_probationes.sh
python3 - <<'EOF'
p = 'briar/compile_probationes.sh'
s = open(p).read()
s = s.replace('MD_PROBATIONES_EFFUSIO', 'BRIAR_PROBATIONES_EFFUSIO')
s = s.replace('build/test_logs/md.log', 'build/test_logs/briar.log')
s = s.replace('# md/compile_probationes.sh - Probationes markdown (cliens tertius materiae)',
              '# briar/compile_probationes.sh - Probationes thistle (cliens materiae; md consumit)')
s = s.replace('MD_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"',
              'BRIAR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"\nMD_DIR="$(cd "$BRIAR_DIR/../md" && pwd)"')
s = s.replace('MATERIA_DIR="$(cd "$MD_DIR/../materia" && pwd)"', 'MATERIA_DIR="$(cd "$BRIAR_DIR/../materia" && pwd)"')
s = s.replace('RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"', 'RADIX_DIR="$(cd "$BRIAR_DIR/.." && pwd)"')
s = s.replace('BUILD_DIR="$MD_DIR/build"', 'BUILD_DIR="$BRIAR_DIR/build"')
s = s.replace('    "-I$MD_DIR/fontes"\n    "-I$MD_DIR/probationes"',
              '    "-I$MD_DIR/fontes"\n    "-I$BRIAR_DIR/fontes"\n    "-I$BRIAR_DIR/probationes"')
s = s.replace('find "$RADIX_DIR/include" "$MD_DIR/fontes" "$MD_DIR/probationes"',
              'find "$RADIX_DIR/include" "$MATERIA_DIR/fontes" "$MD_DIR/fontes" "$BRIAR_DIR/fontes" "$BRIAR_DIR/probationes"')
# md fontes: briar ea CONSUMIT (nexus) - lista explicita, non globus
vetus = 'for src in "$MD_DIR"/fontes/*.c; do\n    base="$(basename "$src" .c)"'
novus = ('for base in md_registrum md_lexicon md_lexema md_arbor md_inlinea md_decoctum md_stml; do\n'
         '    src="$MD_DIR/fontes/$base.c"')
assert s.count(vetus) == 1; s = s.replace(vetus, novus)
s = s.replace('        echo "  [md] $base.c"', '        echo "  [md] $base.c"')
# briar fontes ipsa
vetus2 = '# adiumenta probationum (probationes/*.c non probatio_*), e.g.'
novus2 = ('for src in "$BRIAR_DIR"/fontes/*.c; do\n'
          '    base="$(basename "$src" .c)"\n'
          '    obj="$BUILD_DIR/$base.o"\n'
          '    if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ] || [ -n "$(newest_header "$obj")" ]; then\n'
          '        echo "  [briar] $base.c"\n'
          '        if ! clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" -c "$src" -o "$obj"; then\n'
          '            echo "FRACTA: $base.c" ; exit 1\n'
          '        fi\n'
          '    fi\n'
          '    obj_files="$obj_files $obj"\n'
          'done\n\n' + vetus2)
assert s.count(vetus2) == 1; s = s.replace(vetus2, novus2)
s = s.replace('for src in "$MD_DIR"/probationes/*.c; do', 'for src in "$BRIAR_DIR"/probationes/*.c; do')
# corpus_md.txt gradus ablatus (briar corpus nullum habet - fixa sola)
a = s.index('# corpus markdown = plagulae a git TRACTATAE')
b = s.index('# metra suitae in volumen mensoris')
s = s[:a] + s[b:]
s = s.replace('mensor_suitae_incipere "md."', 'mensor_suitae_incipere "briar."')
s = s.replace('for test_file in "$MD_DIR"/probationes/probatio_*.c; do', 'for test_file in "$BRIAR_DIR"/probationes/probatio_*.c; do')
s = s.replace('echo "MD PROBATIONES: $passed/$total praeteritae"', 'echo "BRIAR PROBATIONES: $passed/$total praeteritae"')
assert 'MD_DIR"/probationes' not in s
open(p, 'w').write(s)
print('runner scriptus')
EOF
chmod +x briar/compile_probationes.sh
grep -n 'BRIAR PROBATIONES\|\[briar\]\|\[md\]\|mensor_suitae_incipere' briar/compile_probationes.sh
```

Expected: four matching lines; the `[md]` loop compiles the seven md
objects into `briar/build/`.

- [ ] **Step 5: Register the gate in pythonica's four tables**

```sh
python3 - <<'EOF'
p = 'pythonica/silva.py'
s = open(p).read()
a = "    'md': (['./md/compile_probationes.sh'], r'MD PROBATIONES: \\d+/\\d+'),\n"
assert s.count(a) == 1
s = s.replace(a, a + "    'briar': (['./briar/compile_probationes.sh'],\n              r'BRIAR PROBATIONES: \\d+/\\d+'),\n")
b = "          'materia': 'suita', 'md': 'suita', 'oratio': 'suita',\n"
assert s.count(b) == 1
s = s.replace(b, "          'materia': 'suita', 'md': 'suita', 'oratio': 'suita',\n          'briar': 'suita',\n")
c = "                     'materia': 'materia.', 'md': 'md.',\n"
assert s.count(c) == 1
s = s.replace(c, "                     'materia': 'materia.', 'md': 'md.',\n                     'briar': 'briar.',\n")
d = "    'md': ('md/probationes', 'md/build/%s'),\n"
assert s.count(d) == 1
s = s.replace(d, d + "    'briar': ('briar/probationes', 'briar/build/%s'),\n")
open(p, 'w').write(s)
print('quattuor tabulae')
EOF
python3 -c "import sys; sys.path.insert(0,'pythonica'); import silva; print(silva.PORTAE['briar'], silva.FORMAE['briar'], silva.SUITAE['briar'])"
```

Expected: the three tuples print; no exception.

- [ ] **Step 6: Write the failing registry test**

```sh
./silva/scribe.sh briar/probationes/probatio_briar_registrum.c <<'EOF'
/* probatio_briar_registrum.c - Registrum et lexicon thistle
 *
 * Tres custodiae tabularum manu scriptarum (exemplar
 * probatio_md_registrum): ordo lexici per TITULOS, offsets locorum
 * CONTIGUI, loci nominati contra titulos. Et circuitus minimus per
 * materiam: documentum vacuum (FINIS solum) scriptum -> lectum ->
 * scriptum, octetim idem.
 */

#include "latina.h"
#include "credo.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS", "INTERPRES", "TAG_APERTUM", "TAG_CLAUSUM", "CRUDUM",
    "PROSA", "STML", "DERIVATUM"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum", "interpres", "prosa", "regio", "elementum"
};

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_briar_registrum",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans lexicon thistle ---\n");
    {
        i32 i;

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);
        CREDO_AEQUALIS_I32 (BRIAR_LEXICON.numerus_generum,
            (i32)BRIAR_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)BRIAR_LEX_NUMERUS_GENERUM);
        per (i = ZEPHYRUM; i < (i32)BRIAR_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)BRIAR_LEX_FINIS), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)BRIAR_LEX_PROSA));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)BRIAR_LEX_CRUDUM));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)BRIAR_LEX_FINIS));
        CREDO_VERUM (strcmp(BRIAR_LEXICON.praefixum_tagi, "briar-")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (BRIAR_LEXICON.genus_spatii, (s32)-I);
        /* nullum trivium, nullum genus LINEA: lineae novae intra
         * valores vivunt */
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
    }

    imprimere("\n--- Probans registrum nodorum ---\n");
    {
        i32 i;
        i32 offset = ZEPHYRUM;

        CREDO_AEQUALIS_I32 (BRIAR_REGISTRUM.numerus_generum,
            (i32)BRIAR_GENUS_NUMERUS);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])), (i32)BRIAR_GENUS_NUMERUS);
        per (i = ZEPHYRUM; i < (i32)BRIAR_GENUS_NUMERUS; i++)
        {
            CREDO_VERUM (strcmp(BRIAR_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &BRIAR_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
            CREDO_AEQUALIS_I32 (BRIAR_REGISTRUM.genera[i].loci_offset,
                offset);
            CREDO_MAIOR_I32 (BRIAR_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            offset += BRIAR_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (offset, BRIAR_REGISTRUM.numerus_locorum);
        CREDO_AEQUALIS_I32 (BRIAR_REGISTRUM.numerus_locorum, (i32)14);

        /* loci nominati contra titulos tabulae */
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_DOCUMENTUM].loci_offset
            + (i32)BRIAR_DOCUMENTUM_PARTES].titulus, "partes") == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_DOCUMENTUM].loci_offset
            + (i32)BRIAR_DOCUMENTUM_VITIUM].titulus, "vitium") == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_REGIO].loci_offset
            + (i32)BRIAR_REGIO_CONTENTUM].titulus, "contentum") == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_REGIO].loci_offset
            + (i32)BRIAR_REGIO_CLAUSUM].titulus, "clausum") == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_ELEMENTUM].loci_offset
            + (i32)BRIAR_ELEMENTUM_TITULUS].titulus, "titulus") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_REGIO].loci_offset
            + (i32)BRIAR_REGIO_VITIUM].species, (s32)MATERIA_LOCUS_INDEX);
    }

    imprimere("\n--- Probans circuitum minimum per materiam ---\n");
    {
        MateriaArborConsilium consilium;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
           MateriaArborVitium vitium;
                MateriaNodus* doc;
                MateriaNodus* lecta;
                MateriaToken* finis;
                       chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        materia_arbor_consilium_nudum(&consilium, &BRIAR_REGISTRUM,
            &ratum, "briar");
        doc = materia_nodus_creare(piscina, (s32)BRIAR_GENUS_DOCUMENTUM,
            (i32)4);
        finis = materia_token_creare(piscina, &FORMA, (s32)BRIAR_LEX_FINIS,
            vacua, ZEPHYRUM, I, I, BRIAR_FONS_PLAGULAE);
        CREDO_NON_NIHIL (doc);
        CREDO_NON_NIHIL (finis);
        CREDO_VERUM (materia_nodus_ponere(doc, (i32)BRIAR_DOCUMENTUM_FINIS,
            materia_valor_token(finis), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(doc, (i32)BRIAR_DOCUMENTUM_VITIUM,
            materia_valor_index((s32)BRIAR_VITIUM_NULLUM),
            MATERIA_LOCUS_INDEX));
        s1 = materia_arbor_scribere_nodum(piscina, doc, &consilium);
        CREDO_VERUM (s1.successus);
        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &consilium,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        si (lecta != NIHIL)
        {
            s2 = materia_arbor_scribere_nodum(piscina, lecta, &consilium);
            CREDO_VERUM (s2.successus);
            CREDO_AEQUALIS_I32 (s1.textus.mensura, s2.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
EOF
```

- [ ] **Step 7: Run the gate, expect green, then plant and revert**

```sh
./briar/compile_probationes.sh registrum
```
Expected tail: `BRIAR PROBATIONES: 1/1 praeteritae`, exit 0.

Plant: swap the order of `"PROSA"` and `"CRUDUM"` in `GENERA_BRIAR[]` of
`briar/fontes/briar_lexicon.c`, run again — expected `FRACTAE:
probatio_briar_registrum` with the title assertion named. Revert the swap,
run again, green.

- [ ] **Step 8: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P0.1 - registrum (genera V: documentum interpres prosa regio elementum; loci XIV, INDEX vitium) + lexicon (VIII genera briar-, verbatim omnia, nullum LINEA: lineae novae intra valores) + cursor briar/compile_probationes.sh (exemplar md; md_* VII obiecta consumpta) + tabulae pythonicae IV. Porta probatio_briar_registrum: ordo per titulos, offsets contigui, loci nominati, circuitus minimus materiae octetim idem; culpa plantata (PROSA/CRUDUM permutata) RUBRA.",
 ["briar/fontes/briar_registrum.h", "briar/fontes/briar_registrum.c",
  "briar/fontes/briar_lexicon.h", "briar/fontes/briar_lexicon.c",
  "briar/compile_probationes.sh", "briar/probationes/probatio_briar_registrum.c",
  "pythonica/silva.py"], ['briar', 'pythonica'])
print(h, r)
EOF
```

---

### Task 2: Line table, classification, fences, token factory (lexema)

**Files:**
- Create: `briar/fontes/briar_lexema.h`, `briar/fontes/briar_lexema.c`
- Create: `briar/probationes/fixa/thistle/salve.thistle`,
  `briar/probationes/fixa/thistle/salve_vitreum.thistle`,
  `briar/probationes/fixa/FONTES.md`
- Create: `briar/probationes/probatio_briar_lexema.c`

**Interfaces:**
- Consumes: `BRIAR_LEXICON`, `BriarLexGenus`, `BRIAR_FONS_*` (Task 1);
  `materia_token_creare` (materia_token.h).
- Produces:
  `b32 briar_lineae_scindere(Piscina*, constans character* fons, i32 mensura, BriarLineae* exitus)`;
  `i32 briar_lineae_numerus(constans BriarLineae*)`;
  `constans BriarLinea* briar_linea(constans BriarLineae*, i32 index)`;
  `chorda briar_chorda_fontis(constans character* fons, s32 ab, s32 ad)`;
  `BriarClassis briar_lineam_classificare(constans BriarLineae*, i32 index, BriarSaeptum* saeptum)`;
  `Xar* briar_lexare_crude(Piscina*, constans character* fons, i32 mensura)`;
  `vacuum briar_fabrica_incipere(BriarFabrica*, Piscina*, constans BriarLineae*)`;
  `MateriaToken* briar_lexema_fabricare(BriarFabrica*, s32 genus, s32 ab, s32 ad)`;
  `MateriaToken* briar_lexema_finis(BriarFabrica*)`;
  `MateriaToken* briar_lexema_derivatum(BriarFabrica*, s32 genus, chorda valor, constans MateriaToken* origo)`.

- [ ] **Step 1: Write the two main fixtures and their inventory**

`briar/probationes/fixa/thistle/salve.thistle` (plain-program shape; the
file ends with a newline after `</c>`):

```
#!/usr/bin/env briar
# Salve

A plain program: one region defines `principale`, one is its probatio.

<c!>
#include "chorda.h"

s32
principale (vacuum)
{
    imprimere("salve, munde\n");
    redde ZEPHYRUM;
}
</c>

<c! munus="probatio">
#include "credo.h"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);

    credo_aperire(piscina);
    CREDO_VERUM(I + I == II);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
</c>
```

`briar/probationes/fixa/thistle/salve_vitreum.thistle` — copy the block
from briar-spec.md §2 "The first file" verbatim (from `#!/usr/bin/env
briar` through the closing `</c>` of the probatio region, one trailing
newline). Its part sequence, which Tasks 3–4 and 9 assert, is:
interpres; prosa; elementum `fenestra`; prosa; regio `html`; prosa; regio
`c` (methodus="salve"); prosa; regio `c` (munus="probatio") — eight parts.

`briar/probationes/fixa/FONTES.md`:

```
# Fixa thistle — inventarium

| plagula | forma | partes | nota |
|---|---|---|---|
| `thistle/salve.thistle` | programma planum | prosa, regio c, prosa, regio c(probatio) | briar-spec §2 gemellus planus |
| `thistle/salve_vitreum.thistle` | app atrii | prosa, elementum fenestra, prosa, regio html, prosa, regio c(methodus), prosa, regio c(probatio) | briar-spec §2 verbatim; saeptum ```html in prosa = lex saepti |
| `thistle/adversa/*.thistle` | casus limitis | vide probatio_briar_arbor.c | Tasks 3–4 |

Snapshots manu scripti 2026-09-04 (plan 1). Mutatio = mutatio pinnarum
in probationibus arbor/stml/nexus/computus - causa nominanda.
```

- [ ] **Step 2: Write the lexema header**

```sh
./silva/scribe.sh briar/fontes/briar_lexema.h <<'EOF'
/* briar_lexema.h - Tabula linearum, classificatio linearum, fabrica
 * lexematum thistle.
 *
 * COLUMNA 0 SOLA LEGITUR (spec par. II): '<' + initium nominis aperit
 * tagum, '</nomen>' claudit, cetera prosa. SAEPTA markdown (``` aut
 * ~~~, indentatio <= III) commutant statum: intra saeptum omnis linea
 * prosa est. Linea prima '#!' = interpres. Lexema super [ab, ad)
 * quodlibet, etiam trans lineas - positio ex tabula linearum.
 */

#ifndef BRIAR_LEXEMA_H
#define BRIAR_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_token.h"

nomen structura {
    s32 offset;       /* initium contenti in fonte */
    i32 mensura;      /* octeti contenti, sine terminatore */
    i32 terminator;   /* ZEPHYRUM (finis), I ("\n"|"\r"), II ("\r\n") */
    i32 numerus;      /* I-basatus */
} BriarLinea;

nomen structura {
    constans character* fons;
                   i32  mensura;
                   Xar* lineae;    /* Xar de BriarLinea (per valorem) */
} BriarLineae;

b32
briar_lineae_scindere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
           BriarLineae* exitus);

i32
briar_lineae_numerus (
    constans BriarLineae* lineae);

/* NIHIL si index extra */
constans BriarLinea*
briar_linea (
    constans BriarLineae* lineae,
                     i32  index);

/* Visus in fontem, nulla copia */
chorda
briar_chorda_fontis (
    constans character* fons,
                   s32  ab,
                   s32  ad);

/* ---- classificatio ---- */

nomen enumeratio {
    BRIAR_LINEA_PROSA = 0,
    BRIAR_LINEA_INTERPRES,
    BRIAR_LINEA_TAG_APERTUM,
    BRIAR_LINEA_TAG_CLAUSUM
} BriarLineaGenus;

/* status saepti markdown, per lineas portatus */
nomen structura {
    b32       aperta;
    character signum;      /* '`' aut '~' */
    i32       longitudo;   /* cursus aperiens */
} BriarSaeptum;

nomen structura {
    BriarLineaGenus genus;
             chorda titulus;       /* nomen tagi (visus in fontem) */
                b32 crudus;        /* '!' post nomen */
                b32 dedens;        /* '\' post nomen aut '!' */
                b32 auto_clausum;  /* linea in "/>" desinit */
} BriarClassis;

/* Lineam classificare; saeptum RENOVATUR (apertura/clausura). */
BriarClassis
briar_lineam_classificare (
    constans BriarLineae* lineae,
                     i32  index,
            BriarSaeptum* saeptum);

/* Lexatio cruda: lexema unum per lineam (genus ex classificatione,
 * INTERPRES/TAG_APERTUM/TAG_CLAUSUM/PROSA), FINIS ultimum. Xar de
 * MateriaToken*. Valores concatenati == fons. Status regionum NON
 * fertur - porta classificationis, non arbor. */
Xar*
briar_lexare_crude (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* ---- fabrica lexematum ---- */

nomen structura {
              Piscina* piscina;
    constans BriarLineae* lineae;
    MateriaTokenForma  forma;
                  i32  linea_ultima;   /* cursor quaesitionis lineae */
} BriarFabrica;

vacuum
briar_fabrica_incipere (
             BriarFabrica* fabrica,
                 Piscina* piscina,
    constans BriarLineae* lineae);

/* Lexema super [ab, ad) - positio (linea, columna) octeti 'ab' ex
 * tabula; ad > ab aut ad == ab (vacuum) licet. NIHIL = memoria. */
MateriaToken*
briar_lexema_fabricare (
    BriarFabrica* fabrica,
             s32  genus,
             s32  ab,
             s32  ad);

/* FINIS post octetum ultimum */
MateriaToken*
briar_lexema_finis (
    BriarFabrica* fabrica);

/* Lexema DERIVATUM (fons I): byte_offset -I, positio originis */
MateriaToken*
briar_lexema_derivatum (
             BriarFabrica* fabrica,
                      s32  genus,
                   chorda  valor,
    constans MateriaToken* origo);

#endif /* BRIAR_LEXEMA_H */
EOF
```

- [ ] **Step 3: Write the failing lexema test**

```sh
./silva/scribe.sh briar/probationes/probatio_briar_lexema.c <<'EOF'
/* probatio_briar_lexema.c - Tabula linearum, classificatio, saepta,
 * fabrica, corpus fixturarum octetim exactum.
 *
 * LEX COLUMNAE 0 (spec par. II): tag aperit solum si '<' + initium
 * nominis in columna 0; '< 3', '<-', '</ x', ' <c!>' prosa sunt. LEX
 * SAEPTI: intra ``` aut ~~~ omnis linea prosa; clausura idem signum,
 * cursus >= aperiens, indentatio <= III.
 */

#include "latina.h"
#include "credo.h"
#include "briar_lexema.h"
#include "briar_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle"
};

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* valores concatenati == fons, byte_offset currens */
interior b32
_circuitus (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   i32* lexemata)
{
    Xar* xs = briar_lexare_crude(piscina, fons, mensura);
    s32  cursor = ZEPHYRUM;
    i32  i;
    i32  n;

    si (xs == NIHIL)
    {
        redde FALSUM;
    }
    n = xar_numerus(xs);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaToken* t = *(MateriaToken**)xar_obtinere(xs, i);

        si (t->byte_offset != cursor)
        {
            imprimere("    (lexema %d: offset %d, cursor %d)\n",
                (integer)i, (integer)t->byte_offset, (integer)cursor);
            redde FALSUM;
        }
        si (   t->valor.mensura > ZEPHYRUM
            && memcmp(t->valor.datum, fons + cursor,
                   (size_t)t->valor.mensura) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        cursor = cursor + (s32)t->valor.mensura;
    }
    *lexemata = n;
    redde (b32)(cursor == (s32)mensura);
}

/* classificatio lineae 'index' documenti dati, statu saepti ab initio
 * ambulato usque ad eam */
interior BriarClassis
_classis (
               Piscina* piscina,
    constans character* fons,
                   i32  index)
{
    BriarLineae  lineae;
    BriarSaeptum saeptum;
    BriarClassis c;
             i32 i;

    saeptum.aperta     = FALSUM;
    saeptum.signum     = ' ';
    saeptum.longitudo  = ZEPHYRUM;
    (vacuum)briar_lineae_scindere(piscina, fons, (i32)strlen(fons),
        &lineae);
    c.genus = BRIAR_LINEA_PROSA;
    per (i = ZEPHYRUM; i <= index; i++)
    {
        c = briar_lineam_classificare(&lineae, i, &saeptum);
    }
    redde c;
}

interior b32
_titulus_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM);
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
    BriarClassis c;

    piscina = piscina_generare_dynamicum("probatio_briar_lexema",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans tabulam linearum ---\n");
    {
        BriarLineae lineae;

        CREDO_VERUM (briar_lineae_scindere(piscina, "a\nbb\r\n\nc", 8,
            &lineae));
        CREDO_AEQUALIS_I32 (briar_lineae_numerus(&lineae), (i32)4);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, ZEPHYRUM)->mensura, I);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, I)->terminator, II);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, II)->mensura, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, III)->terminator, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, III)->numerus, (i32)4);
        CREDO_NIHIL (briar_linea(&lineae, (i32)4));
        CREDO_VERUM (briar_lineae_scindere(piscina, "", ZEPHYRUM, &lineae));
        CREDO_AEQUALIS_I32 (briar_lineae_numerus(&lineae), ZEPHYRUM);
    }

    imprimere("\n--- Probans classificationem (lex columnae 0) ---\n");
    c = _classis(piscina, "#!/usr/bin/env briar\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_INTERPRES);
    c = _classis(piscina, "x\n#!/usr/bin/env briar\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "<c!>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    CREDO_VERUM (c.crudus);
    CREDO_FALSUM (c.dedens);
    CREDO_FALSUM (c.auto_clausum);
    CREDO_VERUM (_titulus_est(c.titulus, "c"));
    c = _classis(piscina, "<c! methodus=\"salve\">\n", ZEPHYRUM);
    CREDO_VERUM (c.crudus && _titulus_est(c.titulus, "c"));
    c = _classis(piscina, "<c!\\>\n", ZEPHYRUM);
    CREDO_VERUM (c.crudus && c.dedens);
    c = _classis(piscina, "<fenestra titulus=\"x\"/>  \n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    CREDO_FALSUM (c.crudus);
    CREDO_VERUM (c.auto_clausum);
    CREDO_VERUM (_titulus_est(c.titulus, "fenestra"));
    c = _classis(piscina, "<md-html>\n", ZEPHYRUM);
    CREDO_VERUM (_titulus_est(c.titulus, "md-html"));
    c = _classis(piscina, "</c>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_CLAUSUM);
    CREDO_VERUM (_titulus_est(c.titulus, "c"));
    c = _classis(piscina, "< 3\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "<-\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "</ x>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, " <c!>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "<c>", ZEPHYRUM);   /* sine terminatore */
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);

    imprimere("\n--- Probans lex saepti ---\n");
    c = _classis(piscina, "```html\n<html>\n```\n<html!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "```html\n<html>\n```\n<html!>\n", III);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    c = _classis(piscina, "~~~\n<c!>\n~~~\n<c!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "~~~\n<c!>\n~~~\n<c!>\n", III);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    /* clausura brevior non claudit; signum alienum non claudit */
    c = _classis(piscina, "````\n```\n<c!>\n", II);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "```\n~~~\n<c!>\n", II);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    /* indentatio III aperit, IV non */
    c = _classis(piscina, "   ```\n<c!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "    ```\n<c!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    /* saeptum apertum in fine */
    {
        BriarLineae  lineae;
        BriarSaeptum saeptum;

        saeptum.aperta = FALSUM;
        saeptum.signum = ' ';
        saeptum.longitudo = ZEPHYRUM;
        (vacuum)briar_lineae_scindere(piscina, "```\nx\n", (i32)6, &lineae);
        (vacuum)briar_lineam_classificare(&lineae, ZEPHYRUM, &saeptum);
        (vacuum)briar_lineam_classificare(&lineae, I, &saeptum);
        CREDO_VERUM (saeptum.aperta);
        CREDO_AEQUALIS_I32 (saeptum.longitudo, III);
    }

    imprimere("\n--- Probans fabricam ---\n");
    {
        BriarLineae  lineae;
        BriarFabrica fabrica;
        MateriaToken* t;
        constans character* fons = "ab\ncd\nef";

        (vacuum)briar_lineae_scindere(piscina, fons, (i32)8, &lineae);
        briar_fabrica_incipere(&fabrica, piscina, &lineae);
        t = briar_lexema_fabricare(&fabrica, (s32)BRIAR_LEX_PROSA, (s32)3,
            (s32)8);
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_I32 (t->linea, II);
        CREDO_AEQUALIS_I32 (t->columna, I);
        CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)5);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)3);
        t = briar_lexema_fabricare(&fabrica, (s32)BRIAR_LEX_PROSA, (s32)7,
            (s32)7);
        CREDO_AEQUALIS_I32 (t->valor.mensura, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (t->columna, II);
        t = briar_lexema_finis(&fabrica);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)8);
        CREDO_AEQUALIS_I32 (t->linea, III);
        CREDO_AEQUALIS_I32 (t->columna, III);
        t = briar_lexema_derivatum(&fabrica, (s32)BRIAR_LEX_DERIVATUM,
            briar_chorda_fontis(fons, ZEPHYRUM, I), t);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)-I);
        CREDO_AEQUALIS_S32 (t->fons_index, BRIAR_FONS_DERIVATUS);
    }

    imprimere("\n--- Probans corpus fixturarum (octetim) ---\n");
    {
        i32 i;
        i32 lexemata_summa = ZEPHYRUM;

        per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXA) / magnitudo(FIXA[0]));
            i++)
        {
            character* textus;
                  i32  mensura = ZEPHYRUM;
                  i32  lexemata = ZEPHYRUM;

            textus = _plagulam_legere(piscina, FIXA[i], &mensura);
            CREDO_NON_NIHIL (textus);
            si (textus == NIHIL)
            {
                perge;
            }
            CREDO_VERUM (_circuitus(piscina, textus, mensura, &lexemata));
            lexemata_summa = lexemata_summa + lexemata;
        }
        /* tegumentum suum metitur */
        CREDO_MAIOR_I32 (lexemata_summa, (i32)40);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
EOF
./briar/compile_probationes.sh lexema
```
Expected: `FRACTA (compilatio): probatio_briar_lexema` (no implementation).

- [ ] **Step 4: Write the lexema implementation**

```sh
./silva/scribe.sh briar/fontes/briar_lexema.c <<'EOF'
/* briar_lexema.c - Vide briar_lexema.h. Tabula linearum ex
 * md_lexema.c transcripta (una regula: terminator "\n", "\r", "\r\n";
 * terminator ultimus lineam vacuam ulteriorem non parit). */

#include "briar_lexema.h"
#include "briar_lexicon.h"
#include <string.h>

#define SAEPTUM_CURSUS_MINIMUS  III
#define SAEPTUM_INDENTATIO_MAX  III

b32
briar_lineae_scindere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
           BriarLineae* exitus)
{
    s32 i;
    s32 initium;
    i32 numerus;

    exitus->fons     = fons;
    exitus->mensura  = mensura;
    exitus->lineae   = xar_creare(piscina, (i32)magnitudo(BriarLinea));
    si (exitus->lineae == NIHIL)
    {
        redde FALSUM;
    }
    i        = ZEPHYRUM;
    initium  = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    dum (i < (s32)mensura)
    {
        character c = fons[i];

        si (c == '\n' || c == '\r')
        {
            BriarLinea* l;
                   i32  term = I;

            si (   c == '\r' && i + I < (s32)mensura
                && fons[i + I] == '\n')
            {
                term = II;
            }
            l = (BriarLinea*)xar_addere(exitus->lineae);
            si (l == NIHIL)
            {
                redde FALSUM;
            }
            numerus        = numerus + I;
            l->offset      = initium;
            l->mensura     = (i32)(i - initium);
            l->terminator  = term;
            l->numerus     = numerus;
            i              = i + (s32)term;
            initium        = i;
            perge;
        }
        i = i + I;
    }
    si (initium < (s32)mensura)
    {
        BriarLinea* l = (BriarLinea*)xar_addere(exitus->lineae);

        si (l == NIHIL)
        {
            redde FALSUM;
        }
        numerus        = numerus + I;
        l->offset      = initium;
        l->mensura     = (i32)((s32)mensura - initium);
        l->terminator  = ZEPHYRUM;
        l->numerus     = numerus;
    }
    redde VERUM;
}

i32
briar_lineae_numerus (
    constans BriarLineae* lineae)
{
    redde (lineae == NIHIL || lineae->lineae == NIHIL)
        ? ZEPHYRUM : xar_numerus(lineae->lineae);
}

constans BriarLinea*
briar_linea (
    constans BriarLineae* lineae,
                     i32  index)
{
    si (index < ZEPHYRUM || index >= briar_lineae_numerus(lineae))
    {
        redde NIHIL;
    }
    redde (constans BriarLinea*)xar_obtinere(lineae->lineae, index);
}

chorda
briar_chorda_fontis (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    chorda c;
    unio { constans character* c; i8* i; } u;

    u.c        = fons + ab;
    c.datum    = u.i;
    c.mensura  = (i32)(ad - ab);
    redde c;
}

interior b32
_initium_nominis (
    character c)
{
    redde (b32)((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || c == '_');
}

interior b32
_character_nominis (
    character c)
{
    redde (b32)(_initium_nominis(c) || (c >= '0' && c <= '9')
        || c == '-');
}

BriarClassis
briar_lineam_classificare (
    constans BriarLineae* lineae,
                     i32  index,
            BriarSaeptum* saeptum)
{
           BriarClassis  c;
    constans BriarLinea* l = briar_linea(lineae, index);
     constans character* t;
                    i32  n;
                    i32  i;

    c.genus            = BRIAR_LINEA_PROSA;
    c.titulus.datum    = NIHIL;
    c.titulus.mensura  = ZEPHYRUM;
    c.crudus           = FALSUM;
    c.dedens           = FALSUM;
    c.auto_clausum     = FALSUM;
    si (l == NIHIL)
    {
        redde c;
    }
    t = lineae->fons + l->offset;
    n = l->mensura;

    /* interpres: linea prima sola */
    si (index == ZEPHYRUM && n >= II && t[0] == '#' && t[1] == '!')
    {
        c.genus = BRIAR_LINEA_INTERPRES;
        redde c;
    }

    /* saeptum markdown: <= III spatia, deinde ``` aut ~~~ */
    {
        i32       spatia = ZEPHYRUM;
        i32       cursus = ZEPHYRUM;
        character signum;

        dum (   spatia < n && spatia < SAEPTUM_INDENTATIO_MAX
             && t[spatia] == ' ')
        {
            spatia = spatia + I;
        }
        si (spatia < n && (t[spatia] == '`' || t[spatia] == '~'))
        {
            signum = t[spatia];
            dum (spatia + cursus < n && t[spatia + cursus] == signum)
            {
                cursus = cursus + I;
            }
            si (cursus >= SAEPTUM_CURSUS_MINIMUS)
            {
                si (saeptum->aperta)
                {
                    /* clausura: idem signum, cursus >= aperiens,
                     * deinde spatia sola */
                    si (   signum == saeptum->signum
                        && cursus >= saeptum->longitudo)
                    {
                        i32 k    = spatia + cursus;
                        b32 sola = VERUM;

                        dum (k < n)
                        {
                            si (t[k] != ' ' && t[k] != '\t')
                            {
                                sola = FALSUM;
                                frange;
                            }
                            k = k + I;
                        }
                        si (sola)
                        {
                            saeptum->aperta = FALSUM;
                        }
                    }
                    redde c;
                }
                saeptum->aperta     = VERUM;
                saeptum->signum     = signum;
                saeptum->longitudo  = cursus;
                redde c;
            }
        }
    }
    si (saeptum->aperta)
    {
        redde c;   /* intra saeptum omnis linea prosa */
    }

    /* tagi in columna 0 */
    si (n >= II && t[0] == '<')
    {
        si (_initium_nominis(t[1]))
        {
            i32 f;

            i = I;
            dum (i < n && _character_nominis(t[i]))
            {
                i = i + I;
            }
            c.genus    = BRIAR_LINEA_TAG_APERTUM;
            c.titulus  = briar_chorda_fontis(lineae->fons, l->offset + I,
                l->offset + (s32)i);
            si (i < n && t[i] == '!')
            {
                c.crudus = VERUM;
                i = i + I;
            }
            si (i < n && t[i] == '\\')
            {
                c.dedens = VERUM;
            }
            f = n;
            dum (f > ZEPHYRUM && (t[f - I] == ' ' || t[f - I] == '\t'))
            {
                f = f - I;
            }
            si (f >= II && t[f - II] == '/' && t[f - I] == '>')
            {
                c.auto_clausum = VERUM;
            }
            redde c;
        }
        si (t[1] == '/' && n >= (i32)4 && _initium_nominis(t[2]))
        {
            i = II;
            dum (i < n && _character_nominis(t[i]))
            {
                i = i + I;
            }
            si (i < n && t[i] == '>')
            {
                c.genus    = BRIAR_LINEA_TAG_CLAUSUM;
                c.titulus  = briar_chorda_fontis(lineae->fons,
                    l->offset + II, l->offset + (s32)i);
            }
            redde c;
        }
    }
    redde c;
}

vacuum
briar_fabrica_incipere (
             BriarFabrica* fabrica,
                 Piscina* piscina,
    constans BriarLineae* lineae)
{
    fabrica->piscina              = piscina;
    fabrica->lineae               = lineae;
    fabrica->forma.mensura_caudae = ZEPHYRUM;
    fabrica->linea_ultima         = ZEPHYRUM;
}

/* linea quae octetum 'ab' continet (cursor resumit; retro licet) */
interior constans BriarLinea*
_lineam_invenire (
    BriarFabrica* fabrica,
             s32  ab)
{
    i32 n = briar_lineae_numerus(fabrica->lineae);
    i32 i = fabrica->linea_ultima;

    si (i >= n)
    {
        i = (n > ZEPHYRUM) ? n - I : ZEPHYRUM;
    }
    dum (i > ZEPHYRUM && briar_linea(fabrica->lineae, i)->offset > ab)
    {
        i = i - I;
    }
    dum (i + I < n && briar_linea(fabrica->lineae, i + I)->offset <= ab)
    {
        i = i + I;
    }
    fabrica->linea_ultima = i;
    redde briar_linea(fabrica->lineae, i);
}

MateriaToken*
briar_lexema_fabricare (
    BriarFabrica* fabrica,
             s32  genus,
             s32  ab,
             s32  ad)
{
    constans BriarLinea* l = _lineam_invenire(fabrica, ab);
                    i32  linea   = I;
                    i32  columna = I;

    si (l != NIHIL)
    {
        linea    = l->numerus;
        columna  = (i32)(ab - l->offset) + I;
    }
    redde materia_token_creare(fabrica->piscina, &fabrica->forma, genus,
        briar_chorda_fontis(fabrica->lineae->fons, ab, ad), ab, linea,
        columna, BRIAR_FONS_PLAGULAE);
}

MateriaToken*
briar_lexema_finis (
    BriarFabrica* fabrica)
{
    i32 n        = briar_lineae_numerus(fabrica->lineae);
    i32 linea    = I;
    i32 columna  = I;

    si (n > ZEPHYRUM)
    {
        constans BriarLinea* l = briar_linea(fabrica->lineae, n - I);

        si (l->terminator > ZEPHYRUM)
        {
            linea = l->numerus + I;
        }
        alioquin
        {
            linea    = l->numerus;
            columna  = l->mensura + I;
        }
    }
    redde materia_token_creare(fabrica->piscina, &fabrica->forma,
        (s32)BRIAR_LEX_FINIS,
        briar_chorda_fontis(fabrica->lineae->fons,
            (s32)fabrica->lineae->mensura, (s32)fabrica->lineae->mensura),
        (s32)fabrica->lineae->mensura, linea, columna,
        BRIAR_FONS_PLAGULAE);
}

MateriaToken*
briar_lexema_derivatum (
             BriarFabrica* fabrica,
                      s32  genus,
                   chorda  valor,
    constans MateriaToken* origo)
{
    redde materia_token_creare(fabrica->piscina, &fabrica->forma, genus,
        valor, (s32)-I, origo->linea, origo->columna,
        BRIAR_FONS_DERIVATUS);
}

Xar*
briar_lexare_crude (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
     BriarLineae  lineae;
    BriarFabrica  fabrica;
    BriarSaeptum  saeptum;
             Xar* xs;
             i32  n;
             i32  i;

    si (piscina == NIHIL || (fons == NIHIL && mensura > ZEPHYRUM))
    {
        redde NIHIL;
    }
    si (!briar_lineae_scindere(piscina, fons, mensura, &lineae))
    {
        redde NIHIL;
    }
    briar_fabrica_incipere(&fabrica, piscina, &lineae);
    xs = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    si (xs == NIHIL)
    {
        redde NIHIL;
    }
    saeptum.aperta     = FALSUM;
    saeptum.signum     = ' ';
    saeptum.longitudo  = ZEPHYRUM;
    n = briar_lineae_numerus(&lineae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans BriarLinea* l = briar_linea(&lineae, i);
              BriarClassis  c = briar_lineam_classificare(&lineae, i,
                  &saeptum);
                       s32  genus;
             MateriaToken* t;
             MateriaToken** sedes;

        commutatio (c.genus)
        {
            casus BRIAR_LINEA_INTERPRES:   genus = (s32)BRIAR_LEX_INTERPRES;   frange;
            casus BRIAR_LINEA_TAG_APERTUM: genus = (s32)BRIAR_LEX_TAG_APERTUM; frange;
            casus BRIAR_LINEA_TAG_CLAUSUM: genus = (s32)BRIAR_LEX_TAG_CLAUSUM; frange;
            ordinarius:                    genus = (s32)BRIAR_LEX_PROSA;       frange;
        }
        t = briar_lexema_fabricare(&fabrica, genus, l->offset,
            l->offset + (s32)l->mensura + (s32)l->terminator);
        sedes = (MateriaToken**)xar_addere(xs);
        si (t == NIHIL || sedes == NIHIL)
        {
            redde NIHIL;
        }
        *sedes = t;
    }
    {
        MateriaToken*  t     = briar_lexema_finis(&fabrica);
        MateriaToken** sedes = (MateriaToken**)xar_addere(xs);

        si (t == NIHIL || sedes == NIHIL)
        {
            redde NIHIL;
        }
        *sedes = t;
    }
    redde xs;
}
EOF
```

Note the `unio` cast in `briar_chorda_fontis`: `chorda.datum` is `i8*` and
the source is `constans character*`; the house forbids a cast that drops
`constans` (`-Wcast-qual`), so the union is the sanctioned bridge (md's
`md_chorda_fontis` does the same).

- [ ] **Step 5: Run to green, plant, revert**

```sh
./briar/compile_probationes.sh lexema
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`.

Plant: in `briar_lineam_classificare` change `si (saeptum->aperta)` to
`si (FALSUM)` — the fence law dies; expected red at the ````html`
assertion (`<html>` classified TAG_APERTUM). Revert; green.

- [ ] **Step 6: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P0.2 - lexema: tabula linearum (\\n \\r \\r\\n), classificatio columnae 0 (tag apertus: '<' + initium nominis, '!' crudus, '\\\\' dedens, '/>' auto-clausum; tag clausus '</nomen>'; '< 3' '<-' '</ x' ' <c!>' prosa), LEX SAEPTI (``` ~~~ indentatio <= III, clausura idem signum cursus >= aperiens: intra saeptum omnis linea prosa - exemplum documentationis in columna 0 regionem non aperit), fabrica lexematum trans lineas (positio ex tabula, cursor quaesitionis), FINIS, derivatum fons I, lexatio cruda (lexema per lineam). Fixa salve.thistle + salve_vitreum.thistle (spec par. II) + FONTES.md. Porta probatio_briar_lexema: tabula, classificatio XX casus, saepta VIII, fabrica, corpus octetim; culpa plantata (saeptum ignoratum) RUBRA.",
 ["briar/fontes/briar_lexema.h", "briar/fontes/briar_lexema.c",
  "briar/probationes/probatio_briar_lexema.c",
  "briar/probationes/fixa/thistle/salve.thistle",
  "briar/probationes/fixa/thistle/salve_vitreum.thistle",
  "briar/probationes/fixa/FONTES.md"], ['briar'])
print(h, r)
EOF
```

---

### Task 3: The arbor — regions, prose, interpres, elements, byte round trip

**Files:**
- Create: `briar/fontes/briar_arbor.h`, `briar/fontes/briar_arbor.c`
- Create: `briar/probationes/probatio_briar_arbor.c`

**Interfaces:**
- Consumes: Task 2's lexema API; `materia_nodus_*`, `materia_valor_*`,
  `materia_arbor_patres_figere`, `materia_scribere_nodum` +
  `materia_scriptura_consilium_nudum`.
- Produces:
  `MateriaNodus* briar_arbor_parsare(Piscina*, constans character* fons, i32 mensura)`;
  `i32 briar_partes_numerus(constans MateriaNodus* documentum)`;
  `MateriaNodus* briar_pars(constans MateriaNodus* documentum, i32 index)`;
  `MateriaToken* briar_lexema(constans MateriaNodus* nodus, i32 locus)`;
  `chorda briar_titulus(constans MateriaNodus* pars)`;
  `s32 briar_vitium(constans MateriaNodus* nodus)`;
  `MateriaScriptura briar_emittere(Piscina*, constans MateriaNodus* radix)`.

- [ ] **Step 1: Write the header**

```sh
./silva/scribe.sh briar/fontes/briar_arbor.h <<'EOF'
/* briar_arbor.h - Documentum thistle parsare: sequentia partium
 * (prosa | regio | elementum) cum interprete optionali et FINIS.
 *
 * SEMPER ARBOR: fons quilibet parsatur (regio non clausa usque ad
 * finem cum vitio; clausura vaga prosa est; saeptum apertum vitium
 * documenti). Emissio arboris == fons octetim (briar_emittere).
 * Extensio elementi STML: linea aperiens usque ad '</nomen>' in
 * columna 0, aut linea una se-claudens (spec par. II, emendatio
 * plani I).
 */

#ifndef BRIAR_ARBOR_H
#define BRIAR_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"
#include "materia_scribere.h"

/* NIHIL solum si memoria deficit */
MateriaNodus*
briar_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

i32
briar_partes_numerus (
    constans MateriaNodus* documentum);

/* NIHIL si index extra */
MateriaNodus*
briar_pars (
    constans MateriaNodus* documentum,
                      i32  index);

/* lexema loci dati aut NIHIL (locus absens aut non TOKEN) */
MateriaToken*
briar_lexema (
    constans MateriaNodus* nodus,
                      i32  locus);

/* regio/elementum: nomen tagi (lexema derivatum); alii: vacua */
chorda
briar_titulus (
    constans MateriaNodus* pars);

/* documentum/regio/elementum: BriarVitium; alii: NULLUM */
s32
briar_vitium (
    constans MateriaNodus* nodus);

/* octeti fontis ex arbore (fons ZEPHYRUM: derivata omissa) */
MateriaScriptura
briar_emittere (
               Piscina* piscina,
    constans MateriaNodus* radix);

#endif /* BRIAR_ARBOR_H */
EOF
```

- [ ] **Step 2: Write the failing arbor test**

```sh
./silva/scribe.sh briar/probationes/probatio_briar_arbor.c <<'EOF'
/* probatio_briar_arbor.c - Arbor thistle: partes, regiones, elementa,
 * interpres, vitia; emissio == fons octetim super fixa omnia.
 *
 * Casus adversi in fixa/thistle/adversa (Task 4): regio non clausa
 * (contentum usque ad finem, vitium I), clausura vaga (prosa),
 * '<' sine nomine (prosa), regiones vacuae, sine interprete, saeptum
 * cum tag (prosa), saeptum apertum (vitium III documenti), plagula
 * vacua, sine linea finali, CRLF, elementum non clausum (vitium II),
 * elementum multilineare.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle",
    "briar/probationes/fixa/thistle/adversa/non_clausa.thistle",
    "briar/probationes/fixa/thistle/adversa/clausum_vagum.thistle",
    "briar/probationes/fixa/thistle/adversa/minor_in_prosa.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuae.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_interprete.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_apertum.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuum.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_linea_finali.thistle",
    "briar/probationes/fixa/thistle/adversa/crlf.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_non_clausum.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_multilineare.thistle"
};

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior b32
_emissio_idem (
               Piscina* piscina,
    constans MateriaNodus* radix,
    constans character* fons,
                   i32  mensura)
{
    MateriaScriptura s = briar_emittere(piscina, radix);

    si (!s.successus)
    {
        imprimere("    (emissio fracta: %s)\n", s.causa ? s.causa : "-");
        redde FALSUM;
    }
    si (s.textus.mensura != mensura)
    {
        imprimere("    (emissio %d octeti contra %d)\n",
            (integer)s.textus.mensura, (integer)mensura);
        redde FALSUM;
    }
    redde (b32)(mensura == ZEPHYRUM
        || memcmp(s.textus.datum, fons, (size_t)mensura) == ZEPHYRUM);
}

interior b32
_titulus_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM);
}

interior b32
_valor_est (
    constans MateriaToken* t,
       constans character* literae)
{
    redde (b32)(t != NIHIL && _titulus_est(t->valor, literae));
}

/* genera partium ut litterae: p=prosa r=regio e=elementum */
interior vacuum
_forma (
    constans MateriaNodus* doc,
                character* exitus)
{
    i32 n = briar_partes_numerus(doc);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* p = briar_pars(doc, i);

        exitus[i] = (p->genus == (s32)BRIAR_GENUS_PROSA) ? 'p'
                  : (p->genus == (s32)BRIAR_GENUS_REGIO) ? 'r' : 'e';
    }
    exitus[n] = '\0';
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
    character forma[64];

    piscina = piscina_generare_dynamicum("probatio_briar_arbor", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans salve.thistle ---\n");
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
        MateriaNodus* doc;
        MateriaNodus* r;

        textus = _plagulam_legere(piscina, FIXA[0], &mensura);
        CREDO_NON_NIHIL (textus);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_NON_NIHIL (doc);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "prpr") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)doc->loci[BRIAR_DOCUMENTUM_INTERPRES].genus,
            (s32)MATERIA_VALOR_NODUS);
        CREDO_VERUM (_valor_est(briar_lexema(
            doc->loci[BRIAR_DOCUMENTUM_INTERPRES].datum.nodus,
            (i32)BRIAR_INTERPRES_TOK), "#!/usr/bin/env briar\n"));
        r = briar_pars(doc, I);
        CREDO_VERUM (_titulus_est(briar_titulus(r), "c"));
        CREDO_VERUM (_valor_est(briar_lexema(r, (i32)BRIAR_REGIO_APERTUM),
            "<c!>\n"));
        CREDO_VERUM (_valor_est(briar_lexema(r, (i32)BRIAR_REGIO_CLAUSUM),
            "</c>\n"));
        CREDO_CHORDA_INCIPIT (briar_lexema(r, (i32)BRIAR_REGIO_CONTENTUM)->valor,
            chorda_ex_literis("#include \"chorda.h\"\n", piscina));
        CREDO_AEQUALIS_S32 (briar_vitium(r), (s32)BRIAR_VITIUM_NULLUM);
        CREDO_AEQUALIS_S32 (briar_vitium(doc), (s32)BRIAR_VITIUM_NULLUM);
        r = briar_pars(doc, III);
        CREDO_VERUM (_valor_est(briar_lexema(r, (i32)BRIAR_REGIO_APERTUM),
            "<c! munus=\"probatio\">\n"));
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, II),
            (i32)BRIAR_PROSA_TOK), "\n"));
        CREDO_NON_NIHIL (briar_lexema(doc, (i32)BRIAR_DOCUMENTUM_FINIS));
        CREDO_VERUM (_emissio_idem(piscina, doc, textus, mensura));
    }

    imprimere("\n--- Probans salve_vitreum.thistle ---\n");
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
        MateriaNodus* doc;
        MateriaNodus* e;

        textus = _plagulam_legere(piscina, FIXA[1], &mensura);
        CREDO_NON_NIHIL (textus);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_NON_NIHIL (doc);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "peprprpr") == ZEPHYRUM);
        e = briar_pars(doc, I);
        CREDO_VERUM (_titulus_est(briar_titulus(e), "fenestra"));
        CREDO_VERUM (_valor_est(briar_lexema(e, (i32)BRIAR_ELEMENTUM_TOK),
            "<fenestra titulus=\"salve\" latitudo=\"640\" altitudo=\"400\"/>\n"));
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, III)), "html"));
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, (i32)5)), "c"));
        /* prosa prima saeptum ```html cum <button> continet */
        CREDO_CHORDA_CONTINET (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_PROSA_TOK)->valor,
            chorda_ex_literis("```html\n<button", piscina));
        CREDO_VERUM (_emissio_idem(piscina, doc, textus, mensura));
    }

    imprimere("\n--- Probans casus inlineares ---\n");
    {
        MateriaNodus* doc;

        doc = briar_arbor_parsare(piscina, "", ZEPHYRUM);
        CREDO_NON_NIHIL (doc);
        CREDO_AEQUALIS_I32 (briar_partes_numerus(doc), ZEPHYRUM);
        CREDO_NON_NIHIL (briar_lexema(doc, (i32)BRIAR_DOCUMENTUM_FINIS));
        CREDO_VERUM (_emissio_idem(piscina, doc, "", ZEPHYRUM));

        doc = briar_arbor_parsare(piscina, "<c!/>\nx\n", (i32)8);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "rp") == ZEPHYRUM);
        CREDO_NIHIL (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM));
        CREDO_VERUM (_emissio_idem(piscina, doc, "<c!/>\nx\n", (i32)8));
    }

    imprimere("\n--- Probans corpus fixturarum (emissio octetim) ---\n");
    {
        i32 i;
        i32 n = (i32)(magnitudo(FIXA) / magnitudo(FIXA[0]));

        per (i = ZEPHYRUM; i < n; i++)
        {
            character* textus;
                  i32  mensura = ZEPHYRUM;
            MateriaNodus* doc;

            textus = _plagulam_legere(piscina, FIXA[i], &mensura);
            si (textus == NIHIL)
            {
                imprimere("  absens: %s\n", FIXA[i]);
                CREDO_NON_NIHIL (textus);
                perge;
            }
            doc = briar_arbor_parsare(piscina, textus, mensura);
            CREDO_NON_NIHIL (doc);
            CREDO_VERUM (_emissio_idem(piscina, doc, textus, mensura));
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
EOF
```

The adversa fixtures are written in Task 4; until then the corpus loop
reports them absent (red), which is the intended order: Task 3 goes green
on the two main fixtures and inline cases by temporarily running with the
adversa entries commented out, Task 4 restores them.

- [ ] **Step 3: Run, expect compile failure**

```sh
./briar/compile_probationes.sh arbor
```
Expected: `FRACTA (compilatio): probatio_briar_arbor`.

- [ ] **Step 4: Write the arbor**

```sh
./silva/scribe.sh briar/fontes/briar_arbor.c <<'EOF'
/* briar_arbor.c - Vide briar_arbor.h. */

#include "briar_arbor.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "briar_lexema.h"
#include "materia_arbor.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>

nomen structura {
         Piscina* piscina;
     BriarLineae  lineae;
    BriarFabrica  fabrica;
    MateriaNodus* documentum;
             s32  prosa_initium;   /* offset prosae pendentis; -I = nulla */
} BriarParsura;

interior MateriaNodus*
_nodus (
    BriarParsura* p,
      BriarGenus  genus)
{
    redde materia_nodus_creare(p->piscina, (s32)genus,
        BRIAR_REGISTRUM.genera[genus].loci_numerus);
}

interior b32
_ponere_lexema (
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* lexema)
{
    si (lexema == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_ponere(nodus, locus, materia_valor_token(lexema),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_ponere_index (
    MateriaNodus* nodus,
             i32  locus,
             s32  valor)
{
    redde materia_nodus_ponere(nodus, locus, materia_valor_index(valor),
        MATERIA_LOCUS_INDEX);
}

interior b32
_partem_addere (
    BriarParsura* p,
    MateriaNodus* pars)
{
    redde materia_nodus_appendere(p->piscina, p->documentum,
        (i32)BRIAR_DOCUMENTUM_PARTES, materia_valor_nodus(pars),
        MATERIA_LOCUS_LISTA_NODUS);
}

interior s32
_finis_lineae (
    constans BriarLinea* l)
{
    redde l->offset + (s32)l->mensura + (s32)l->terminator;
}

/* prosam pendentem usque ad 'ad' deponere (nihil si nulla/vacua) */
interior b32
_prosam_deponere (
    BriarParsura* p,
             s32  ad)
{
    MateriaNodus* n;
    MateriaToken* t;

    si (p->prosa_initium < ZEPHYRUM || ad <= p->prosa_initium)
    {
        p->prosa_initium = (s32)-I;
        redde VERUM;
    }
    n = _nodus(p, BRIAR_GENUS_PROSA);
    t = briar_lexema_fabricare(&p->fabrica, (s32)BRIAR_LEX_PROSA,
        p->prosa_initium, ad);
    p->prosa_initium = (s32)-I;
    si (n == NIHIL || t == NIHIL)
    {
        redde FALSUM;
    }
    redde _ponere_lexema(n, (i32)BRIAR_PROSA_TOK, t) && _partem_addere(p, n);
}

/* linea 'index' est '</titulus>' in columna 0 (spatia finalia licent)? */
interior b32
_clausura_est (
    constans BriarLineae* lineae,
                     i32  index,
                  chorda  titulus)
{
    constans BriarLinea* l = briar_linea(lineae, index);
     constans character* t;
                    i32  n;
                    i32  k;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    t = lineae->fons + l->offset;
    n = l->mensura;
    si (n < titulus.mensura + III || t[0] != '<' || t[1] != '/')
    {
        redde FALSUM;
    }
    si (memcmp(t + II, titulus.datum, (size_t)titulus.mensura) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    k = II + titulus.mensura;
    si (t[k] != '>')
    {
        redde FALSUM;
    }
    per (k = k + I; k < n; k++)
    {
        si (t[k] != ' ' && t[k] != '\t')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* clausuram quaerere ab linea 'ab'; -I si nulla */
interior i32
_clausuram_quaerere (
    constans BriarLineae* lineae,
                     i32  ab,
                  chorda  titulus)
{
    i32 n = briar_lineae_numerus(lineae);
    i32 j;

    per (j = ab; j < n; j++)
    {
        si (_clausura_est(lineae, j, titulus))
        {
            redde j;
        }
    }
    redde (i32)-I;
}

/* regio cruda ab linea 'index' (tag apertus crudus). Reddit indicem
 * lineae proximae; *bene FALSUM = memoria. */
interior i32
_regionem_legere (
          BriarParsura* p,
                   i32  index,
    constans BriarClassis* c,
                   b32* bene)
{
    constans BriarLinea* l = briar_linea(&p->lineae, index);
                    i32  n = briar_lineae_numerus(&p->lineae);
                    i32  j;
           MateriaNodus* nodus;
           MateriaToken* apertum;
           MateriaToken* titulus;
                    s32  contentum_ab = _finis_lineae(l);
                    s32  contentum_ad;
                    s32  vitium = (s32)BRIAR_VITIUM_NULLUM;

    *bene = FALSUM;
    nodus = _nodus(p, BRIAR_GENUS_REGIO);
    apertum = briar_lexema_fabricare(&p->fabrica, (s32)BRIAR_LEX_TAG_APERTUM,
        l->offset, contentum_ab);
    si (nodus == NIHIL || apertum == NIHIL)
    {
        redde n;
    }
    titulus = briar_lexema_derivatum(&p->fabrica, (s32)BRIAR_LEX_DERIVATUM,
        c->titulus, apertum);
    si (   !_ponere_lexema(nodus, (i32)BRIAR_REGIO_APERTUM, apertum)
        || !_ponere_lexema(nodus, (i32)BRIAR_REGIO_TITULUS, titulus))
    {
        redde n;
    }
    j = index + I;
    si (!c->auto_clausum)
    {
        i32 clausura = _clausuram_quaerere(&p->lineae, j, c->titulus);

        contentum_ad = (clausura >= ZEPHYRUM)
            ? briar_linea(&p->lineae, clausura)->offset
            : (s32)p->lineae.mensura;
        si (contentum_ad > contentum_ab)
        {
            MateriaToken* contentum = briar_lexema_fabricare(&p->fabrica,
                (s32)BRIAR_LEX_CRUDUM, contentum_ab, contentum_ad);

            si (!_ponere_lexema(nodus, (i32)BRIAR_REGIO_CONTENTUM, contentum))
            {
                redde n;
            }
        }
        si (clausura >= ZEPHYRUM)
        {
            constans BriarLinea* lc = briar_linea(&p->lineae, clausura);
                  MateriaToken* clausum = briar_lexema_fabricare(&p->fabrica,
                      (s32)BRIAR_LEX_TAG_CLAUSUM, lc->offset,
                      _finis_lineae(lc));

            si (!_ponere_lexema(nodus, (i32)BRIAR_REGIO_CLAUSUM, clausum))
            {
                redde n;
            }
            j = clausura + I;
        }
        alioquin
        {
            vitium = (s32)BRIAR_VITIUM_REGIO_NON_CLAUSA;
            j = n;
        }
    }
    si (   !_ponere_index(nodus, (i32)BRIAR_REGIO_VITIUM, vitium)
        || !_partem_addere(p, nodus))
    {
        redde n;
    }
    *bene = VERUM;
    redde j;
}

/* elementum STML ab linea 'index' (tag apertus non crudus): usque ad
 * '</titulus>' in columna 0 aut linea una se-claudens */
interior i32
_elementum_legere (
          BriarParsura* p,
                   i32  index,
    constans BriarClassis* c,
                   b32* bene)
{
    constans BriarLinea* l = briar_linea(&p->lineae, index);
                    i32  n = briar_lineae_numerus(&p->lineae);
                    i32  j = index + I;
           MateriaNodus* nodus;
           MateriaToken* tok;
           MateriaToken* titulus;
                    s32  ad = _finis_lineae(l);
                    s32  vitium = (s32)BRIAR_VITIUM_NULLUM;

    *bene = FALSUM;
    si (!c->auto_clausum)
    {
        i32 clausura = _clausuram_quaerere(&p->lineae, j, c->titulus);

        si (clausura >= ZEPHYRUM)
        {
            ad = _finis_lineae(briar_linea(&p->lineae, clausura));
            j  = clausura + I;
        }
        alioquin
        {
            ad     = (s32)p->lineae.mensura;
            j      = n;
            vitium = (s32)BRIAR_VITIUM_ELEMENTUM_NON_CLAUSUM;
        }
    }
    nodus = _nodus(p, BRIAR_GENUS_ELEMENTUM);
    tok = briar_lexema_fabricare(&p->fabrica, (s32)BRIAR_LEX_STML,
        l->offset, ad);
    si (nodus == NIHIL || tok == NIHIL)
    {
        redde n;
    }
    titulus = briar_lexema_derivatum(&p->fabrica, (s32)BRIAR_LEX_DERIVATUM,
        c->titulus, tok);
    si (   !_ponere_lexema(nodus, (i32)BRIAR_ELEMENTUM_TOK, tok)
        || !_ponere_lexema(nodus, (i32)BRIAR_ELEMENTUM_TITULUS, titulus)
        || !_ponere_index(nodus, (i32)BRIAR_ELEMENTUM_VITIUM, vitium)
        || !_partem_addere(p, nodus))
    {
        redde n;
    }
    *bene = VERUM;
    redde j;
}

MateriaNodus*
briar_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    BriarParsura  p;
    BriarSaeptum  saeptum;
    MateriaToken* finis;
             i32  n;
             i32  i;

    si (piscina == NIHIL || (fons == NIHIL && mensura > ZEPHYRUM))
    {
        redde NIHIL;
    }
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.piscina        = piscina;
    p.prosa_initium  = (s32)-I;
    si (!briar_lineae_scindere(piscina, fons, mensura, &p.lineae))
    {
        redde NIHIL;
    }
    briar_fabrica_incipere(&p.fabrica, piscina, &p.lineae);
    p.documentum = _nodus(&p, BRIAR_GENUS_DOCUMENTUM);
    si (p.documentum == NIHIL)
    {
        redde NIHIL;
    }
    saeptum.aperta     = FALSUM;
    saeptum.signum     = ' ';
    saeptum.longitudo  = ZEPHYRUM;
    n = briar_lineae_numerus(&p.lineae);
    i = ZEPHYRUM;
    dum (i < n)
    {
        constans BriarLinea* l = briar_linea(&p.lineae, i);
              BriarClassis  c = briar_lineam_classificare(&p.lineae, i,
                  &saeptum);
                       b32  bene;

        si (c.genus == BRIAR_LINEA_INTERPRES)
        {
            MateriaNodus* in;
            MateriaToken* t;

            si (!_prosam_deponere(&p, l->offset))
            {
                redde NIHIL;
            }
            in = _nodus(&p, BRIAR_GENUS_INTERPRES);
            t = briar_lexema_fabricare(&p.fabrica, (s32)BRIAR_LEX_INTERPRES,
                l->offset, _finis_lineae(l));
            si (   in == NIHIL || !_ponere_lexema(in, (i32)BRIAR_INTERPRES_TOK, t)
                || !materia_nodus_ponere(p.documentum,
                       (i32)BRIAR_DOCUMENTUM_INTERPRES, materia_valor_nodus(in),
                       MATERIA_LOCUS_NODUS))
            {
                redde NIHIL;
            }
            i = i + I;
        }
        alioquin si (c.genus == BRIAR_LINEA_TAG_APERTUM)
        {
            si (!_prosam_deponere(&p, l->offset))
            {
                redde NIHIL;
            }
            i = c.crudus ? _regionem_legere(&p, i, &c, &bene)
                         : _elementum_legere(&p, i, &c, &bene);
            si (!bene)
            {
                redde NIHIL;
            }
        }
        alioquin
        {
            /* prosa, et clausura vaga */
            si (p.prosa_initium < ZEPHYRUM)
            {
                p.prosa_initium = l->offset;
            }
            i = i + I;
        }
    }
    si (!_prosam_deponere(&p, (s32)mensura))
    {
        redde NIHIL;
    }
    finis = briar_lexema_finis(&p.fabrica);
    si (   !_ponere_lexema(p.documentum, (i32)BRIAR_DOCUMENTUM_FINIS, finis)
        || !_ponere_index(p.documentum, (i32)BRIAR_DOCUMENTUM_VITIUM,
               saeptum.aperta ? (s32)BRIAR_VITIUM_SAEPTUM_NON_CLAUSUM
                              : (s32)BRIAR_VITIUM_NULLUM))
    {
        redde NIHIL;
    }
    materia_arbor_patres_figere(piscina, p.documentum);
    redde p.documentum;
}

i32
briar_partes_numerus (
    constans MateriaNodus* documentum)
{
    si (documentum == NIHIL || documentum->genus != (s32)BRIAR_GENUS_DOCUMENTUM)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(
        documentum->loci[BRIAR_DOCUMENTUM_PARTES]);
}

MateriaNodus*
briar_pars (
    constans MateriaNodus* documentum,
                      i32  index)
{
    MateriaValor* v;

    si (index < ZEPHYRUM || index >= briar_partes_numerus(documentum))
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(documentum->loci[BRIAR_DOCUMENTUM_PARTES],
        index);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS) ? v->datum.nodus
                                                          : NIHIL;
}

MateriaToken*
briar_lexema (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (nodus == NIHIL || locus < ZEPHYRUM || locus >= nodus->numerus_locorum)
    {
        redde NIHIL;
    }
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

chorda
briar_titulus (
    constans MateriaNodus* pars)
{
    MateriaToken* t = NIHIL;
        chorda   vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (pars == NIHIL)
    {
        redde vacua;
    }
    si (pars->genus == (s32)BRIAR_GENUS_REGIO)
    {
        t = briar_lexema(pars, (i32)BRIAR_REGIO_TITULUS);
    }
    alioquin si (pars->genus == (s32)BRIAR_GENUS_ELEMENTUM)
    {
        t = briar_lexema(pars, (i32)BRIAR_ELEMENTUM_TITULUS);
    }
    redde (t != NIHIL) ? t->valor : vacua;
}

s32
briar_vitium (
    constans MateriaNodus* nodus)
{
    i32 locus;

    si (nodus == NIHIL)
    {
        redde (s32)BRIAR_VITIUM_NULLUM;
    }
    si (nodus->genus == (s32)BRIAR_GENUS_DOCUMENTUM)
    {
        locus = (i32)BRIAR_DOCUMENTUM_VITIUM;
    }
    alioquin si (nodus->genus == (s32)BRIAR_GENUS_REGIO)
    {
        locus = (i32)BRIAR_REGIO_VITIUM;
    }
    alioquin si (nodus->genus == (s32)BRIAR_GENUS_ELEMENTUM)
    {
        locus = (i32)BRIAR_ELEMENTUM_VITIUM;
    }
    alioquin
    {
        redde (s32)BRIAR_VITIUM_NULLUM;
    }
    redde (nodus->loci[locus].genus == MATERIA_VALOR_INDEX)
        ? nodus->loci[locus].datum.index : (s32)BRIAR_VITIUM_NULLUM;
}

MateriaScriptura
briar_emittere (
               Piscina* piscina,
    constans MateriaNodus* radix)
{
    MateriaScripturaConsilium consilium;

    materia_scriptura_consilium_nudum(&consilium, &BRIAR_REGISTRUM);
    consilium.fons_index = BRIAR_FONS_PLAGULAE;
    redde materia_scribere_nodum(piscina, radix, &consilium);
}
EOF
```

- [ ] **Step 5: Run to green on the two main fixtures**

Temporarily comment out the twelve `adversa/` entries in `FIXA[]` (they
arrive in Task 4), then:

```sh
./briar/compile_probationes.sh arbor
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`.

Plant: in `_regionem_legere` replace `_clausuram_quaerere(&p->lineae, j, c->titulus)`
with `(i32)-I` — every region runs to EOF; expected red at
`_valor_est(... BRIAR_REGIO_CLAUSUM ...)` and the emission check still
green (bytes are still all covered — note that in the worklog: the byte
law is structure-independent). Revert; green.

- [ ] **Step 6: Commit (with the adversa entries still commented out)**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P0.3 - arbor: documentum (interpres? partes finis vitium), prosa (cursus unus per lexema), regio cruda (apertum/titulus derivatus/contentum?/clausum?/vitium; clausura '</nomen>' in columna 0 sola; '<c!/>' vacua), elementum STML (usque ad clausuram in columna 0 aut linea se-claudens - emendatio spec par. II), semper arbor, emissio == fons octetim (fons ZEPHYRUM, derivata omissa). Porta probatio_briar_arbor: salve prpr, salve_vitreum peprprpr, casus inlineares, corpus octetim; culpa plantata (clausura numquam inventa) RUBRA in structura, viridis in octetis - lex octetorum a structura independens.",
 ["briar/fontes/briar_arbor.h", "briar/fontes/briar_arbor.c",
  "briar/probationes/probatio_briar_arbor.c"], ['briar'])
print(h, r)
EOF
```

---

### Task 4: Adversa — the edges are named, not hidden

**Files:**
- Create: twelve fixtures under `briar/probationes/fixa/thistle/adversa/`
- Modify: `briar/probationes/probatio_briar_arbor.c` (restore `FIXA[]`, add the adversa block)
- Modify: `briar/probationes/fixa/FONTES.md` (already lists `adversa/*`)

**Interfaces:** consumes Task 3 only.

- [ ] **Step 1: Write the fixtures (byte-exact; `printf` so CRLF and the missing final newline are real)**

```sh
A=briar/probationes/fixa/thistle/adversa; mkdir -p "$A"
printf '#!/usr/bin/env briar\n<c!>\nint x;\n' > "$A/non_clausa.thistle"
printf 'prosa\n</c>\nplus\n' > "$A/clausum_vagum.thistle"
printf 'a\n< 3\n<-\n</ x>\n <c!>\n<1>\n' > "$A/minor_in_prosa.thistle"
printf '<c!>\n</c>\n<x/>\n<c!/>\n' > "$A/vacuae.thistle"
printf '# titulus\n<c!>\nx\n</c>\n' > "$A/sine_interprete.thistle"
printf '```html\n<html>\n```\n<html!>\n<b>x</b>\n</html>\n' > "$A/saeptum_html.thistle"
printf '```\n<c!>\n' > "$A/saeptum_apertum.thistle"
printf '' > "$A/vacuum.thistle"
printf '<c!>\nx\n</c>' > "$A/sine_linea_finali.thistle"
printf '#!/usr/bin/env briar\r\n<c!>\r\nx\r\n</c>\r\n' > "$A/crlf.thistle"
printf '<fenestra>\n<a/>\n' > "$A/elementum_non_clausum.thistle"
printf '<fenestra>\n  <a/>\n</fenestra>\npost\n' > "$A/elementum_multilineare.thistle"
ls "$A" | wc -l
```
Expected: `12`.

- [ ] **Step 2: Restore `FIXA[]` and add the adversa assertions**

Uncomment the twelve entries, then insert this block before the corpus
loop in `probatio_briar_arbor.c`:

```c
    imprimere("\n--- Probans adversa ---\n");
    {
        character* textus;
              i32  mensura;
        MateriaNodus* doc;
        MateriaNodus* r;

        /* regio non clausa: contentum usque ad finem, clausum absens */
        textus = _plagulam_legere(piscina, FIXA[2], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "r") == ZEPHYRUM);
        r = briar_pars(doc, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (briar_vitium(r), (s32)BRIAR_VITIUM_REGIO_NON_CLAUSA);
        CREDO_NIHIL (briar_lexema(r, (i32)BRIAR_REGIO_CLAUSUM));
        CREDO_VERUM (_valor_est(briar_lexema(r, (i32)BRIAR_REGIO_CONTENTUM),
            "int x;\n"));

        /* clausura vaga = prosa una */
        textus = _plagulam_legere(piscina, FIXA[3], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "p") == ZEPHYRUM);

        /* '<' sine nomine, indentatum, digitus: prosa una */
        textus = _plagulam_legere(piscina, FIXA[4], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "p") == ZEPHYRUM);

        /* regiones vacuae: regio sine contento, elementum, regio auto */
        textus = _plagulam_legere(piscina, FIXA[5], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "rer") == ZEPHYRUM);
        CREDO_NIHIL (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM));
        CREDO_NON_NIHIL (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CLAUSUM));
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, I)), "x"));
        CREDO_NIHIL (briar_lexema(briar_pars(doc, II), (i32)BRIAR_REGIO_CLAUSUM));
        CREDO_AEQUALIS_S32 (briar_vitium(briar_pars(doc, II)),
            (s32)BRIAR_VITIUM_NULLUM);

        /* sine interprete */
        textus = _plagulam_legere(piscina, FIXA[6], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_AEQUALIS_S32 ((s32)doc->loci[BRIAR_DOCUMENTUM_INTERPRES].genus,
            (s32)MATERIA_VALOR_NIHIL);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "pr") == ZEPHYRUM);

        /* saeptum cum tag: prosa, deinde regio vera */
        textus = _plagulam_legere(piscina, FIXA[7], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "pr") == ZEPHYRUM);
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, I)), "html"));

        /* saeptum apertum in fine: vitium documenti */
        textus = _plagulam_legere(piscina, FIXA[8], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "p") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (briar_vitium(doc),
            (s32)BRIAR_VITIUM_SAEPTUM_NON_CLAUSUM);

        /* sine linea finali: clausum sine terminatore */
        textus = _plagulam_legere(piscina, FIXA[10], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CLAUSUM), "</c>"));

        /* CRLF: terminatores in lexematibus, positio recta */
        textus = _plagulam_legere(piscina, FIXA[11], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "r") == ZEPHYRUM);
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM), "x\r\n"));
        CREDO_AEQUALIS_I32 (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM)->linea, III);

        /* elementum non clausum */
        textus = _plagulam_legere(piscina, FIXA[12], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "e") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (briar_vitium(briar_pars(doc, ZEPHYRUM)),
            (s32)BRIAR_VITIUM_ELEMENTUM_NON_CLAUSUM);

        /* elementum multilineare, deinde prosa */
        textus = _plagulam_legere(piscina, FIXA[13], &mensura);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "ep") == ZEPHYRUM);
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_ELEMENTUM_TOK), "<fenestra>\n  <a/>\n</fenestra>\n"));
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, I),
            (i32)BRIAR_PROSA_TOK), "post\n"));
    }
```

- [ ] **Step 3: Run to green, plant, revert**

```sh
./briar/compile_probationes.sh arbor
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`; the corpus loop now covers
14 files.

Plant: in `briar_arbor_parsare` replace `saeptum.aperta ?` with `FALSUM ?`
— expected red on the `SAEPTUM_NON_CLAUSUM` assertion only. Revert.

- [ ] **Step 4: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P0.4 - adversa XII (regio non clausa, clausura vaga, '<' sine nomine, regiones vacuae, sine interprete, saeptum cum tag, saeptum apertum, plagula vacua, sine linea finali, CRLF, elementum non clausum, elementum multilineare) - quisque limes nominatus in probatio_briar_arbor, corpus XIV octetim; culpa plantata (saeptum apertum tacitum) RUBRA.",
 ["briar/probationes/probatio_briar_arbor.c",
  "briar/probationes/fixa/thistle/adversa/non_clausa.thistle",
  "briar/probationes/fixa/thistle/adversa/clausum_vagum.thistle",
  "briar/probationes/fixa/thistle/adversa/minor_in_prosa.thistle",
  "briar/probationes/fixa/thistle/adversa/vacuae.thistle",
  "briar/probationes/fixa/thistle/adversa/sine_interprete.thistle",
  "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",
  "briar/probationes/fixa/thistle/adversa/saeptum_apertum.thistle",
  "briar/probationes/fixa/thistle/adversa/vacuum.thistle",
  "briar/probationes/fixa/thistle/adversa/sine_linea_finali.thistle",
  "briar/probationes/fixa/thistle/adversa/crlf.thistle",
  "briar/probationes/fixa/thistle/adversa/elementum_non_clausum.thistle",
  "briar/probationes/fixa/thistle/adversa/elementum_multilineare.thistle"],
 ['briar'])
print(h, r)
EOF
```

---

### Task 5: STML projection, round trips, `briar/arbor.sh`

**Files:**
- Create: `briar/fontes/briar_stml.h`, `briar/fontes/briar_stml.c`
- Create: `briar/probationes/probatio_briar_stml.c`
- Create: `briar/instrumenta/arbor.c`, `briar/arbor.sh`

**Interfaces:**
- Consumes: Task 3 (`briar_arbor_parsare`), materia_arbor (`materia_arbor_consilium_nudum`,
  `materia_arbor_scribere_nodum`, `materia_arbor_legere`, `materia_arbor_aequalis`).
- Produces: `vacuum briar_stml_consilium(MateriaArborConsilium*, constans MateriaLexiconRatum*)`.

- [ ] **Step 1: Write the consilium with md's origin hook**

```sh
./silva/scribe.sh briar/fontes/briar_stml.h <<'EOF'
/* briar_stml.h - Consilium proiectionis STML arboris thistle:
 * registrum + lexicon + uncus originis pro lexematibus DERIVATIS
 * (fons I: nomen tagi octetos nullos tenet - radix emissionis vacua,
 * ut md_stml). Cursor materiae lineas novas intra valores numerat,
 * ergo comparator FIDELIS sine unco licet. */

#ifndef BRIAR_STML_H
#define BRIAR_STML_H

#include "latina.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"

vacuum
briar_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum);

#endif /* BRIAR_STML_H */
EOF
./silva/scribe.sh briar/fontes/briar_stml.c <<'EOF'
/* briar_stml.c - Vide briar_stml.h (exemplar md_stml.c). */

#include "briar_stml.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"

/* Radix derivatorum: lexema vacuum, fonte plagulae, 'iam positum'
 * (byte_offset ZEPHYRUM) - lector eam praeterit sine promotione. */
hic_manens MateriaToken RADIX_DERIVATORUM;
hic_manens b32          RADIX_PARATA = FALSUM;

interior vacuum
_sedes_quaerere (
                   vacuum* datum,
    constans MateriaToken* lexema,
             MateriaSedes* sedes)
{
    (vacuum)datum;
    sedes->byte_offset  = lexema->byte_offset;
    sedes->linea        = lexema->linea;
    sedes->columna      = lexema->columna;
    sedes->fons_index   = lexema->fons_index;
    sedes->est_fons     = (b32)(lexema->fons_index != BRIAR_FONS_DERIVATUS);
}

interior MateriaToken*
_radix_quaerere (
                vacuum* datum,
          MateriaToken* lexema,
    constans character** causa)
{
    (vacuum)datum;
    *causa = NIHIL;
    si (lexema->fons_index != BRIAR_FONS_DERIVATUS)
    {
        redde lexema;   /* se ipsum emittit */
    }
    si (!RADIX_PARATA)
    {
        RADIX_DERIVATORUM.valor.datum    = NIHIL;
        RADIX_DERIVATORUM.valor.mensura  = ZEPHYRUM;
        RADIX_DERIVATORUM.genus          = (s32)BRIAR_LEX_DERIVATUM;
        RADIX_DERIVATORUM.byte_offset    = ZEPHYRUM;
        RADIX_DERIVATORUM.linea          = I;
        RADIX_DERIVATORUM.columna        = I;
        RADIX_DERIVATORUM.fons_index     = BRIAR_FONS_PLAGULAE;
        RADIX_PARATA                     = VERUM;
    }
    redde &RADIX_DERIVATORUM;
}

hic_manens constans MateriaOrigoUncus ORIGO_BRIAR = {
    NIHIL,
    _sedes_quaerere,
    _radix_quaerere,
    NIHIL
};

vacuum
briar_stml_consilium (
           MateriaArborConsilium* consilium,
    constans MateriaLexiconRatum* ratum)
{
    materia_arbor_consilium_nudum(consilium, &BRIAR_REGISTRUM, ratum,
        "briar");
    consilium->origo = &ORIGO_BRIAR;
}
EOF
```

- [ ] **Step 2: Write the round-trip test**

```sh
./silva/scribe.sh briar/probationes/probatio_briar_stml.c <<'EOF'
/* probatio_briar_stml.c - Circuitus proiectionis STML thistle:
 * scribere -> legere -> scribere BIS octetim idem; comparator
 * STRUCTURALIS et FIDELIS (positiones ex cursore materiae, qui
 * lineas novas intra valores numerat); emissio arboris lectae ==
 * fons. Super fixa XIV et casus inlineares.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_stml.h"
#include "briar_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_lexicon.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle",
    "briar/probationes/fixa/thistle/adversa/non_clausa.thistle",
    "briar/probationes/fixa/thistle/adversa/clausum_vagum.thistle",
    "briar/probationes/fixa/thistle/adversa/minor_in_prosa.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuae.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_interprete.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_apertum.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuum.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_linea_finali.thistle",
    "briar/probationes/fixa/thistle/adversa/crlf.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_non_clausum.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_multilineare.thistle"
};

hic_manens constans character* INLINEARES[] = {
    "",
    "\n",
    "#!/usr/bin/env briar\n",
    "<c!>\n</c>\n",
    "<c!>\nno close",
    "a\n<x/>\nb\n",
    "<c! a=\"1\">\n<b>\n</c>\n<d>\n</d>\n"
};

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior b32
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura)
{
              MateriaNodus* radix;
              MateriaNodus* lecta;
              MateriaNodus* relecta;
      MateriaArborScriptura s1;
      MateriaArborScriptura s2;
      MateriaArborScriptura s3;
         MateriaArborVitium vitium;
    MateriaArborDifferentia d;
           MateriaScriptura e;

    radix = briar_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde FALSUM;
    }
    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        imprimere("    (scriptura I: %s)\n", s1.causa ? s1.causa : "-");
        redde FALSUM;
    }
    lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
        &vitium);
    si (lecta == NIHIL)
    {
        imprimere("    (lectio I: %s, linea %d)\n",
            vitium.causa ? vitium.causa : "-", (integer)vitium.linea);
        redde FALSUM;
    }
    s2 = materia_arbor_scribere_nodum(piscina, lecta, consilium);
    si (   !s2.successus || s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari)\n");
        redde FALSUM;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s2.textus, consilium,
        &vitium);
    s3 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (   relecta == NIHIL || !s3.successus
        || s1.textus.mensura != s3.textus.mensura
        || memcmp(s1.textus.datum, s3.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (circuitus II dispar)\n");
        redde FALSUM;
    }
    /* emissio arboris LECTAE == fons */
    e = briar_emittere(piscina, lecta);
    si (   !e.successus || e.textus.mensura != mensura
        || (mensura > ZEPHYRUM
            && memcmp(e.textus.datum, fons, (size_t)mensura) != ZEPHYRUM))
    {
        imprimere("    (emissio lectae dispar)\n");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d))
    {
        imprimere("    (STRUCTURALIS: %s)\n", d.campus ? d.campus : "-");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d))
    {
        imprimere("    (FIDELITAS: %s)\n", d.campus ? d.campus : "-");
        redde FALSUM;
    }
    redde VERUM;
}

s32
principale (vacuum)
{
                      b32  praeteritus;
                  Piscina* piscina;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_briar_stml", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON,
        &iudicium));
    briar_stml_consilium(&consilium, &ratum);

    imprimere("\n--- Probans casus inlineares ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(INLINEARES)
        / magnitudo(INLINEARES[0])); i++)
    {
        b32 bene = _circuitum_probare(piscina, &consilium, INLINEARES[i],
            (i32)strlen(INLINEARES[i]));

        si (!bene)
        {
            imprimere("  FRACTA inlinearis %d\n", (integer)i);
        }
        CREDO_VERUM (bene);
    }

    imprimere("\n--- Probans fixa ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXA) / magnitudo(FIXA[0])); i++)
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
              b32  bene;

        textus = _plagulam_legere(piscina, FIXA[i], &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            perge;
        }
        bene = _circuitum_probare(piscina, &consilium, textus, mensura);
        si (!bene)
        {
            imprimere("  FRACTA: %s\n", FIXA[i]);
        }
        CREDO_VERUM (bene);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
EOF
./briar/compile_probationes.sh stml
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`. If FIDELITAS fails and
`d.campus` names `linea`/`columna`/`vexilla` for some fixture, the
divergence is a real finding: record which field and which fixture in
`briar/fontes/briar.worklog.md`, keep STRUCTURALIS and the emission
check as the gate, and file the FIDELIS half as a named deferral in the
worklog — do NOT weaken the byte checks. (Expected outcome from reading
`_cursorem_promovere`: it passes; the CRLF fixture is the one to watch,
since `\r` counts as a column and the `cr` attribute restores it.)

- [ ] **Step 3: Plant, revert**

In `briar_stml.c`, change `sedes->est_fons` to `VERUM` unconditionally —
derived titles then claim bytes; expected: `(lectio I: …)` or a
FIDELITAS `byte_offset` divergence on every fixture with a region. Revert;
green.

- [ ] **Step 4: The projection tool and its script**

```sh
./silva/scribe.sh briar/instrumenta/arbor.c <<'EOF'
/* arbor.c - Proiectio STML plagulae thistle (briar/arbor.sh)
 *
 * Usus: arbor <plagula.thistle> [-tacitus]
 * Exitus: 0 sanum, 1 fractum, 2 usus/plagula absens.
 */

#include "latina.h"
#include "briar_arbor.h"
#include "briar_stml.h"
#include "briar_lexicon.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

integer
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
               character* textus;
                     i32  mensura = ZEPHYRUM;
            MateriaNodus* radix;
      MateriaLexiconRatum ratum;
       MateriaLexIudicium iudicium;
    MateriaArborConsilium consilium;
    MateriaArborScriptura s;
                     b32  tacitus = FALSUM;
                  integer i;
      constans character* via = NIHIL;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-tacitus") == ZEPHYRUM)
        {
            tacitus = VERUM;
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: arbor <plagula.thistle> [-tacitus]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("briar_arbor", 4194304);
    si (piscina == NIHIL)
    {
        redde I;
    }
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "arbor: plagula absens: %s\n", via);
        redde II;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON, &iudicium))
    {
        fprintf(stderr, "arbor: lexicon recusatum\n");
        redde I;
    }
    briar_stml_consilium(&consilium, &ratum);
    radix = briar_arbor_parsare(piscina, textus, mensura);
    si (radix == NIHIL)
    {
        fprintf(stderr, "arbor: parsura NIHIL\n");
        redde I;
    }
    s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    si (!s.successus)
    {
        fprintf(stderr, "arbor: scriptura fracta: %s\n",
            s.causa ? s.causa : "-");
        redde I;
    }
    si (tacitus)
    {
        imprimere("%d\n", (integer)s.textus.mensura);
    }
    alioquin
    {
        fwrite(s.textus.datum, I, (size_t)s.textus.mensura, stdout);
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
EOF
cp md/arbor.sh briar/arbor.sh
python3 - <<'EOF'
p = 'briar/arbor.sh'
s = open(p).read()
s = s.replace('# md/arbor.sh - proiectio STML plagulae markdown', '# briar/arbor.sh - proiectio STML plagulae thistle')
s = s.replace('./md/arbor.sh <plagula.md>', './briar/arbor.sh <plagula.thistle>')
s = s.replace('Obiecta md/build/*.o poscit (cursor md/compile_probationes.sh ea\n# struit); instrumentum in md/build/arbor.',
              'Obiecta briar/build/*.o poscit (cursor briar/compile_probationes.sh\n# ea struit); instrumentum in briar/build/arbor.')
s = s.replace('MD_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"',
              'BRIAR_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"\nMD_DIR="$(cd "$BRIAR_DIR/../md" && pwd)"')
s = s.replace('RADIX_DIR="$(cd "$MD_DIR/.." && pwd)"', 'RADIX_DIR="$(cd "$BRIAR_DIR/.." && pwd)"')
s = s.replace('BUILD_DIR="$MD_DIR/build"', 'BUILD_DIR="$BRIAR_DIR/build"')
s = s.replace('"-I$MATERIA_DIR/fontes" "-I$MD_DIR/fontes")', '"-I$MATERIA_DIR/fontes" "-I$MD_DIR/fontes" "-I$BRIAR_DIR/fontes")')
s = s.replace('SRC="$MD_DIR/instrumenta/arbor.c"', 'SRC="$BRIAR_DIR/instrumenta/arbor.c"')
s = s.replace('ls "$BUILD_DIR"/md_*.o', 'ls "$BUILD_DIR"/briar_*.o')
s = s.replace('./md/compile_probationes.sh registrum primum', './briar/compile_probationes.sh registrum primum')
s = s.replace("find \"$BUILD_DIR\" -name 'md_*.o'", "find \"$BUILD_DIR\" -name '*.o'")
assert 'MD_DIR/instrumenta' not in s
open(p, 'w').write(s)
print('arbor.sh scriptus')
EOF
chmod +x briar/arbor.sh
./briar/arbor.sh briar/probationes/fixa/thistle/salve.thistle | head -30
```
Expected: an `<arbor grammatica="briar" registrum-sigillum="…" …>` document
whose first children are `<documentum>`, `<interpres>` and a `<partes>`
list; the `registrum-sigillum` value printed here is the one Task 6 pins.

- [ ] **Step 5: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P1.1 - proiectio STML: briar_stml (consilium + uncus originis derivatorum, exemplar md_stml), porta probatio_briar_stml (scribere->legere->scribere bis octetim idem, emissio arboris lectae == fons, comparator STRUCTURALIS + FIDELIS - cursor materiae lineas novas intra valores numerat) super fixa XIV + inlineares VII; instrumentum briar/instrumenta/arbor.c + ./briar/arbor.sh. Culpa plantata (derivata octetos vindicantia) RUBRA.",
 ["briar/fontes/briar_stml.h", "briar/fontes/briar_stml.c",
  "briar/probationes/probatio_briar_stml.c",
  "briar/instrumenta/arbor.c", "briar/arbor.sh"], ['briar'])
print(h, r)
EOF
```

---

### Task 6: The canon — `briar.canon`, drift guard, seal pin, fixtures judged

**Files:**
- Create: `briar/grammatica/briar.canon`
- Create: `briar/probationes/probatio_briar_canon.c`

**Interfaces:**
- Consumes: Task 5 (`briar_stml_consilium`), `canon_legere`, `canon_iudicare`,
  `canon_nuntius` (canon.h), `stml_legere`, `stml_attributum_capere`,
  `stml_textus_valor` (stml.h), `materia_arbor_sigillum` (materia_arbor.h).
- Produces: the canon file; nothing consumed by later tasks.

The canon is loaded BY PATH (`canon_legere` on the file), like css's and
md's — it is not registered in `canones.registrum`, because the `<arbor>`
root is shared by every materia dialect and the registry keys on the root
element. `bin/canon_examen -canon briar/grammatica/briar.canon <doc.stml>`
judges by hand.

- [ ] **Step 1: Get the live seal, then write the canon**

```sh
./briar/compile_probationes.sh registrum >/dev/null; ./briar/arbor.sh briar/probationes/fixa/thistle/salve.thistle | grep -o 'registrum-sigillum="[0-9a-f]*"'
```
Paste the eight hex characters into the `<optio>` below (marked `SIGILLUM`).

```sh
cat > briar/grammatica/briar.canon <<'EOF'
<!--
  briar.canon
  MANU SCRIPTUM (ut css.canon et md.canon). Registrum briar_registrum.c
  et lexicon briar_lexicon.c manu scripta sunt, ergo canon quoque.
  Custos derivae (probatio_briar_canon) regulas huius plagulae contra
  registrum generum, loci, lexicon et involucrum UTRIMQUE retiret;
  sigillum registri infra PINNATUM contra vivum conferitur.

  MODELLUM: trivia NULLA (involucra <ante>/<post> non declarantur).
  Locus INDEX = involucrum cum TEXTU solo (textus="verum"); locus
  TOKEN = involucrum continens elementum lexematis briar-*; locus
  NODUS = involucrum continens elementum generis; LISTA_NODUS =
  involucrum continens elementa generum. Lexema DERIVATUM (fons I)
  f="1" + linea/columna fert, b numquam. 'cr' in lexematibus verbatim
  (offsets '\r'), 'crlf' nusquam (nullum genus TERMINATOR).

  Iudicium manuale:
    bin/canon_examen -canon briar/grammatica/briar.canon <doc.stml>
-->
<canon dialectus="briar" versio="1">

  <!-- =============== FRAGMENTA =============== -->

  <!-- partes documenti (locus LISTA_NODUS) -->
  <#partes>
    <liberum nomen="prosa"/>
    <liberum nomen="regio"/>
    <liberum nomen="elementum"/>
  </#>

  <!-- lexema verbatim: attributa quae materia scribit -->
  <#attributa-lexematis>
    <attributum nomen="cr"      genus="textus"/>
    <attributum nomen="f"       genus="numerus"/>
    <attributum nomen="linea"   genus="numerus"/>
    <attributum nomen="columna" genus="numerus"/>
  </#>

  <!-- =============== INVOLUCRUM =============== -->

  <elementum nomen="arbor" radix="verum"
    nota="documentum subarboris (arbor M1): liberum unum, genus
          quodlibet">
    <attributum nomen="grammatica" genus="electio"
      necessarium="verum">
      <optio>briar</optio>
    </attributum>
    <attributum nomen="registrum-sigillum" genus="electio"
      necessarium="verum"
      nota="PINNA: sigillum tabularum briar_registrum.c (V genera, XIV
            loci). Tabula mutata sine hac optione mutata rubet in iudicio
            ET in custode derivae.">
      <optio>SIGILLUM</optio>
    </attributum>
    <attributum nomen="fons"    genus="numerus"/>
    <attributum nomen="b"       genus="numerus"/>
    <attributum nomen="linea"   genus="numerus"/>
    <attributum nomen="columna" genus="numerus"/>
    <liberum nomen="documentum" maximum="1"/>
    <liberum nomen="interpres"  maximum="1"/>
    <liberum nomen="prosa"      maximum="1"/>
    <liberum nomen="regio"      maximum="1"/>
    <liberum nomen="elementum"  maximum="1"/>
  </elementum>

  <!-- =============== GENERA =============== -->

  <elementum nomen="documentum">
    <liberum nomen="interpres" maximum="1"/>
    <liberum nomen="partes"    maximum="1"/>
    <liberum nomen="finis"     maximum="1"/>
    <liberum nomen="vitium"    maximum="1"/>
  </elementum>
  <elementum nomen="interpres" intra="documentum">
    <liberum nomen="interpres" maximum="1"/>
  </elementum>
  <elementum nomen="partes" intra="documentum">
    <<#partes>>
  </elementum>
  <elementum nomen="finis" intra="documentum">
    <liberum nomen="briar-finis" maximum="1"/>
  </elementum>
  <elementum nomen="vitium" intra="documentum" textus="verum"/>

  <elementum nomen="interpres">
    <liberum nomen="tok" maximum="1"/>
  </elementum>
  <elementum nomen="tok" intra="interpres">
    <liberum nomen="briar-interpres" maximum="1"/>
  </elementum>

  <elementum nomen="prosa">
    <liberum nomen="tok" maximum="1"/>
  </elementum>
  <elementum nomen="tok" intra="prosa">
    <liberum nomen="briar-prosa" maximum="1"/>
  </elementum>

  <elementum nomen="regio">
    <liberum nomen="apertum"   maximum="1"/>
    <liberum nomen="titulus"   maximum="1"/>
    <liberum nomen="contentum" maximum="1"/>
    <liberum nomen="clausum"   maximum="1"/>
    <liberum nomen="vitium"    maximum="1"/>
  </elementum>
  <elementum nomen="apertum" intra="regio">
    <liberum nomen="briar-tag-apertum" maximum="1"/>
  </elementum>
  <elementum nomen="titulus" intra="regio">
    <liberum nomen="briar-derivatum" maximum="1"/>
  </elementum>
  <elementum nomen="contentum" intra="regio">
    <liberum nomen="briar-crudum" maximum="1"/>
  </elementum>
  <elementum nomen="clausum" intra="regio">
    <liberum nomen="briar-tag-clausum" maximum="1"/>
  </elementum>
  <elementum nomen="vitium" intra="regio" textus="verum"/>

  <elementum nomen="elementum">
    <liberum nomen="tok"     maximum="1"/>
    <liberum nomen="titulus" maximum="1"/>
    <liberum nomen="vitium"  maximum="1"/>
  </elementum>
  <elementum nomen="tok" intra="elementum">
    <liberum nomen="briar-stml" maximum="1"/>
  </elementum>
  <elementum nomen="titulus" intra="elementum">
    <liberum nomen="briar-derivatum" maximum="1"/>
  </elementum>
  <elementum nomen="vitium" intra="elementum" textus="verum"/>

  <!-- =============== LEXEMATA =============== -->

  <elementum nomen="briar-finis"/>
  <elementum nomen="briar-interpres" textus="verum">
    <<#attributa-lexematis>>
  </elementum>
  <elementum nomen="briar-tag-apertum" textus="verum">
    <<#attributa-lexematis>>
  </elementum>
  <elementum nomen="briar-tag-clausum" textus="verum">
    <<#attributa-lexematis>>
  </elementum>
  <elementum nomen="briar-crudum" textus="verum">
    <<#attributa-lexematis>>
  </elementum>
  <elementum nomen="briar-prosa" textus="verum">
    <<#attributa-lexematis>>
  </elementum>
  <elementum nomen="briar-stml" textus="verum">
    <<#attributa-lexematis>>
  </elementum>
  <elementum nomen="briar-derivatum" textus="verum"
    nota="fons I: nomen tagi; f=1, linea/columna portatae, b numquam">
    <<#attributa-lexematis>>
  </elementum>

</canon>
EOF
bin/canon_examen -canon briar/grammatica/briar.canon <(./briar/arbor.sh briar/probationes/fixa/thistle/salve_vitreum.thistle); echo "rc=$?"
```
Expected: `VITIA 0`, rc 0. If it reports `elementum ignotum` for a name,
the projection uses a wrapper the canon does not list — read the printed
element and add its rule; the writer, not the canon, is the authority on
wrapper names (`materia_arbor.c` `MATERIA_ARBOR_TAG_*`). If `bin/canon_examen`
is absent, build it: `./compile_tools.sh canon_examen`.

- [ ] **Step 2: Write the canon gate**

```sh
./silva/scribe.sh briar/probationes/probatio_briar_canon.c <<'EOF'
/* probatio_briar_canon.c - Porta canonis thistle
 *
 *  I.  CUSTOS DERIVAE utrimque: genus quodque registri regulam
 *      globalem habet; locus quisque regulam intra= genus suum; genus
 *      lexematis quodque regulam 'briar-'; et REVERSUM: regula omnis
 *      canonis uni tabularum aut involucro congruit.
 *  II. PINNA SIGILLI: optio 'registrum-sigillum' == sigillum vivum.
 *  III. IUDICIUM: proiectio cuiusque fixturae contra canonem vitia
 *      ZEPHYRUM; numerus documentorum se metitur.
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "briar_arbor.h"
#include "briar_stml.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

#define REGULAE_MAXIMAE 64

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle",
    "briar/probationes/fixa/thistle/adversa/non_clausa.thistle",
    "briar/probationes/fixa/thistle/adversa/clausum_vagum.thistle",
    "briar/probationes/fixa/thistle/adversa/minor_in_prosa.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuae.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_interprete.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_apertum.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuum.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_linea_finali.thistle",
    "briar/probationes/fixa/thistle/adversa/crlf.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_non_clausum.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_multilineare.thistle"
};

nomen structura {
    chorda nomen_regulae;
    chorda intra;        /* mensura ZEPHYRUM = globalis */
       b32 congruens;
} RegulaCanonis;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 mensura = (i32)strlen(literae);

    redde (b32)(c.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)mensura) == ZEPHYRUM));
}

interior b32
_aequalis (
    chorda a,
    chorda b)
{
    redde (b32)(a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum, (size_t)a.mensura) == ZEPHYRUM));
}

/* tag lexematis mangulatum: "briar-" + minusculae, '_' -> '-' */
interior vacuum
_tagum_lexematis (
    constans character* titulus,
             character* exitus)
{
    i32 i = ZEPHYRUM;
    i32 k;

    memcpy(exitus, "briar-", (size_t)6);
    k = (i32)6;
    dum (titulus[i] != '\0')
    {
        character c = titulus[i];

        exitus[k] = (c == '_') ? '-'
                  : (c >= 'A' && c <= 'Z') ? (character)(c + ('a' - 'A')) : c;
        i = i + I;
        k = k + I;
    }
    exitus[k] = '\0';
}

/* regula canonis quaerere (nomen, intra); NIHIL si nulla */
interior RegulaCanonis*
_regulam (
     RegulaCanonis* regulae,
               i32  numerus,
    constans character* nomen_regulae,
    constans character* intra)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   _aequalis_literis(regulae[i].nomen_regulae, nomen_regulae)
            && _aequalis_literis(regulae[i].intra, intra))
        {
            redde &regulae[i];
        }
    }
    redde NIHIL;
}

/* proiectionem plagulae contra canonem iudicare: numerus vitiorum,
 * -I = apparatus */
interior s32
_iudicare (
                 Piscina* piscina,
                   Canon* canon,
     InternamentumChorda* intern,
    constans MateriaArborConsilium* consilium,
      constans character* titulus,
      constans character* fons,
                     i32  mensura)
{
            MateriaNodus* radix;
    MateriaArborScriptura scriptura;
             StmlResultus res;
                     Xar* vitia;
                     i32  n;
                     i32  i;

    radix = briar_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        redde (s32)-I;
    }
    scriptura = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!scriptura.successus)
    {
        imprimere("  %s: scriptura fracta: %s\n", titulus,
            scriptura.causa ? scriptura.causa : "?");
        redde (s32)-I;
    }
    res = stml_legere(scriptura.textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < (i32)10; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: VITIUM %s", titulus, canon_nuntius(v->genus));
        si (v->elementum != NIHIL)
        {
            imprimere(" <%.*s>", (integer)v->elementum->mensura,
                (constans character*)v->elementum->datum);
        }
        imprimere("\n");
    }
    redde (s32)n;
}

s32
principale (vacuum)
{
                      b32  praeteritus;
                  Piscina* piscina;
      InternamentumChorda* intern;
                   chorda  fons_canonis;
                   chorda  causa;
                    Canon* canon;
             StmlResultus  canon_res;
            RegulaCanonis  regulae[REGULAE_MAXIMAE];
                      i32  numerus_regularum = ZEPHYRUM;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_briar_canon", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON,
        &iudicium));
    briar_stml_consilium(&consilium, &ratum);

    {
        character* textus;
              i32  mensura = ZEPHYRUM;

        textus = _plagulam_legere(piscina, "briar/grammatica/briar.canon",
            &mensura);
        si (textus == NIHIL)
        {
            CREDO_CULPA ("briar/grammatica/briar.canon absens");
            credo_imprimere_compendium();
            redde I;
        }
        fons_canonis.datum    = (i8*)textus;
        fons_canonis.mensura  = mensura;
    }
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL)
    {
        imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- I. custos derivae ---\n");
    canon_res = stml_legere(fons_canonis, piscina, intern);
    CREDO_VERUM (canon_res.successus && canon_res.elementum_radix != NIHIL);
    {
        Xar* liberi = canon_res.elementum_radix->liberi;
        i32  n = (liberi != NIHIL) ? xar_numerus(liberi) : ZEPHYRUM;

        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* e = *(StmlNodus**)xar_obtinere(liberi, i);
               chorda* nomen_regulae;
               chorda* intra;

            si (e->titulus == NIHIL || !_aequalis_literis(*e->titulus, "elementum"))
            {
                perge;
            }
            nomen_regulae = stml_attributum_capere(e, "nomen");
            intra         = stml_attributum_capere(e, "intra");
            CREDO_NON_NIHIL (nomen_regulae);
            si (nomen_regulae == NIHIL || numerus_regularum >= REGULAE_MAXIMAE)
            {
                perge;
            }
            regulae[numerus_regularum].nomen_regulae  = *nomen_regulae;
            regulae[numerus_regularum].intra          = (intra != NIHIL)
                ? *intra : chorda_ex_literis("", piscina);
            regulae[numerus_regularum].congruens      = FALSUM;
            numerus_regularum = numerus_regularum + I;
        }
    }
    CREDO_MAIOR_I32 (numerus_regularum, (i32)20);

    /* involucrum */
    {
        RegulaCanonis* r = _regulam(regulae, numerus_regularum, "arbor", "");

        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            r->congruens = VERUM;
        }
    }
    /* genera: regula globalis quaeque */
    per (i = ZEPHYRUM; i < BRIAR_REGISTRUM.numerus_generum; i++)
    {
        RegulaCanonis* r = _regulam(regulae, numerus_regularum,
            BRIAR_REGISTRUM.genera[i].titulus, "");

        si (r == NIHIL)
        {
            imprimere("  genus sine regula: %s\n",
                BRIAR_REGISTRUM.genera[i].titulus);
        }
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            r->congruens = VERUM;
        }
    }
    /* loci: regula intra= genus suum */
    per (i = ZEPHYRUM; i < BRIAR_REGISTRUM.numerus_generum; i++)
    {
        constans MateriaTabGenus* g = &BRIAR_REGISTRUM.genera[i];
                             i32  k;

        per (k = ZEPHYRUM; k < g->loci_numerus; k++)
        {
            RegulaCanonis* r = _regulam(regulae, numerus_regularum,
                BRIAR_REGISTRUM.loci[g->loci_offset + k].titulus,
                g->titulus);

            si (r == NIHIL)
            {
                imprimere("  locus sine regula: %s intra %s\n",
                    BRIAR_REGISTRUM.loci[g->loci_offset + k].titulus,
                    g->titulus);
            }
            CREDO_NON_NIHIL (r);
            si (r != NIHIL)
            {
                r->congruens = VERUM;
            }
        }
    }
    /* lexemata: regula 'briar-...' quaeque */
    per (i = ZEPHYRUM; i < (i32)BRIAR_LEX_NUMERUS_GENERUM; i++)
    {
        character      tag[64];
        RegulaCanonis* r;

        _tagum_lexematis(materia_lexicon_titulus(&ratum, (s32)i), tag);
        r = _regulam(regulae, numerus_regularum, tag, "");
        si (r == NIHIL)
        {
            imprimere("  lexema sine regula: %s\n", tag);
        }
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            r->congruens = VERUM;
        }
    }
    /* reversum: regula omnis congruit */
    per (i = ZEPHYRUM; i < numerus_regularum; i++)
    {
        si (!regulae[i].congruens)
        {
            imprimere("  regula rancida: %.*s (intra %.*s)\n",
                (integer)regulae[i].nomen_regulae.mensura,
                (constans character*)regulae[i].nomen_regulae.datum,
                (integer)regulae[i].intra.mensura,
                (constans character*)regulae[i].intra.datum);
        }
        CREDO_VERUM (regulae[i].congruens);
    }

    imprimere("\n--- II. pinna sigilli ---\n");
    {
        chorda sigillum_vivum = materia_arbor_sigillum(piscina,
            &BRIAR_REGISTRUM);
           Xar* liberi = canon_res.elementum_radix->liberi;
           i32  n = (liberi != NIHIL) ? xar_numerus(liberi) : ZEPHYRUM;
           b32  inventum = FALSUM;

        per (i = ZEPHYRUM; i < n && !inventum; i++)
        {
            StmlNodus* e = *(StmlNodus**)xar_obtinere(liberi, i);
               chorda* nomen_regulae;
                  Xar* attributa;
                  i32  m;
                  i32  k;

            si (e->titulus == NIHIL || !_aequalis_literis(*e->titulus, "elementum"))
            {
                perge;
            }
            nomen_regulae = stml_attributum_capere(e, "nomen");
            si (nomen_regulae == NIHIL || !_aequalis_literis(*nomen_regulae, "arbor"))
            {
                perge;
            }
            attributa = e->liberi;
            m = (attributa != NIHIL) ? xar_numerus(attributa) : ZEPHYRUM;
            per (k = ZEPHYRUM; k < m; k++)
            {
                StmlNodus* a = *(StmlNodus**)xar_obtinere(attributa, k);
                   chorda* an;

                si (a->titulus == NIHIL || !_aequalis_literis(*a->titulus, "attributum"))
                {
                    perge;
                }
                an = stml_attributum_capere(a, "nomen");
                si (an != NIHIL && _aequalis_literis(*an, "registrum-sigillum")
                    && a->liberi != NIHIL && xar_numerus(a->liberi) > ZEPHYRUM)
                {
                    i32 q;

                    per (q = ZEPHYRUM; q < xar_numerus(a->liberi); q++)
                    {
                        StmlNodus* o = *(StmlNodus**)xar_obtinere(a->liberi, q);

                        si (o->titulus != NIHIL && _aequalis_literis(*o->titulus, "optio"))
                        {
                            chorda pinna = stml_textus_valor(o, piscina);

                            imprimere("  pinna %.*s, vivum %.*s\n",
                                (integer)pinna.mensura,
                                (constans character*)pinna.datum,
                                (integer)sigillum_vivum.mensura,
                                (constans character*)sigillum_vivum.datum);
                            CREDO_VERUM (_aequalis(pinna, sigillum_vivum));
                            inventum = VERUM;
                        }
                    }
                }
            }
        }
        CREDO_VERUM (inventum);
    }

    imprimere("\n--- III. iudicium fixturarum ---\n");
    {
        i32 iudicata = ZEPHYRUM;

        per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXA) / magnitudo(FIXA[0])); i++)
        {
            character* textus;
                  i32  mensura = ZEPHYRUM;
                  s32  vitia;

            textus = _plagulam_legere(piscina, FIXA[i], &mensura);
            CREDO_NON_NIHIL (textus);
            si (textus == NIHIL)
            {
                perge;
            }
            vitia = _iudicare(piscina, canon, intern, &consilium, FIXA[i],
                textus, mensura);
            CREDO_AEQUALIS_S32 (vitia, ZEPHYRUM);
            iudicata = iudicata + I;
        }
        CREDO_AEQUALIS_I32 (iudicata, (i32)14);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
EOF
./briar/compile_probationes.sh canon
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`. Two things can be red on
first contact, both informative: (a) a wrapper the writer emits that the
canon lacks (part III names the element) — add the rule; (b) the canon's
`<attributum>` children not being direct `liberi` of the `<elementum
nomen="arbor">` node if the STML parser nests them differently — print
`a->titulus` in the loop and adjust the walk; the pin assertion must stay.

- [ ] **Step 3: Plant twice, revert**

1. Change one hex digit of the pinned `<optio>` → part II red, part III
   red (`registrum-sigillum` not in the electio). Revert.
2. Rename the rule `<elementum nomen="contentum" intra="regio">` to
   `contentus` → part I red twice (locus without rule, stale rule) and
   part III red (unknown element). Revert. Green.

- [ ] **Step 4: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P1.2 - briar.canon manu scriptum (involucrum arbor + sigillum pinnatum, genera V, loci XIV intra=, lexemata VIII briar-*, fragmenta partes/attributa-lexematis; per viam onerandum - radix <arbor> communis dialectis materiae) + porta probatio_briar_canon: custos derivae utrimque, pinna sigilli contra materia_arbor_sigillum vivum, fixa XIV iudicata vitia ZEPHYRUM. Culpae plantatae (pinna mutata; regula renominata) RUBRAE.",
 ["briar/grammatica/briar.canon", "briar/probationes/probatio_briar_canon.c"],
 ['briar'])
print(h, r)
EOF
```

---

### Task 7: Totality — never crashes, always a tree, emission == source

**Files:**
- Create: `briar/probationes/probatio_briar_totalitas.c`

**Interfaces:** consumes Task 3 (`briar_arbor_parsare`, `briar_emittere`);
`CREDO_NON_RUIT` (credo.h, fork-based statement macro).

- [ ] **Step 1: Derive the harness from md's and replace its generators**

```sh
cp md/probationes/probatio_md_totalitas.c briar/probationes/probatio_briar_totalitas.c
python3 - <<'EOF'
p = 'briar/probationes/probatio_briar_totalitas.c'
s = open(p).read()
for a, b in [
    ('probatio_md_totalitas.c - Porta totalitatis markdown (A8c, 2026-09-03)',
     'probatio_briar_totalitas.c - Porta totalitatis thistle (plan 1, Task 7)'),
    ('#include "md_arbor.h"\n#include "md_registrum.h"\n#include "md_lexicon.h"',
     '#include "briar_arbor.h"\n#include "briar_registrum.h"\n#include "briar_lexicon.h"'),
    ('"md/probationes/fixa/md/spica.md",\n    "md/probationes/fixa/md/episodium.md",\n    "md/probationes/fixa/md/piscina.md",\n    "md/probationes/fixa/md/sabaw.md"',
     '"briar/probationes/fixa/thistle/salve.thistle",\n    "briar/probationes/fixa/thistle/salve_vitreum.thistle",\n    "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",\n    "briar/probationes/fixa/thistle/adversa/crlf.thistle"'),
    ('md/build/totalitas_fractum.md', 'briar/build/totalitas_fractum.thistle'),
    ('md_arbor_parsare(', 'briar_arbor_parsare('),
    ('MD_REGISTRUM', 'BRIAR_REGISTRUM'),
    ('MD_FONS_PLAGULAE', 'BRIAR_FONS_PLAGULAE'),
    ('totalitas_casus', 'briar_totalitas_casus'),
]:
    assert s.count(a) >= 1, a
    s = s.replace(a, b)
open(p, 'w').write(s)
print('derivatum')
EOF
grep -n 'materia_scribere_nodum\|materia_scriptura_consilium_nudum\|_totum (' briar/probationes/probatio_briar_totalitas.c | head
```

Then open the file and make these three edits by hand (they are the
places where md's shape differs from briar's):

1. In `_totum(...)`: replace the emission block (the
   `materia_scriptura_consilium_nudum` + `materia_scribere_nodum` pair)
   with `MateriaScriptura s = briar_emittere(piscina, radix);` and keep
   the existing comparisons against `fons`/`mensura`.
2. Replace md's nesting generator (the function that builds `>`/`-`/
   brackets/asterisks/backticks to depth N) with a thistle one:

```c
/* nidificatio thistle: regiones consecutivae, tagi intra prosam,
 * saepta intra regiones, clausurae vagae, ad profunditatem n */
interior i32
_nidificare (
    character* buffer,
          i32  capacitas,
          i32  n,
          i32  forma)
{
    i32 k = ZEPHYRUM;
    i32 i;
    constans character* formae[] = {
        "<c!>\nx\n</c>\n",
        "<a>\n<b/>\n</a>\n",
        "```\n<c!>\n```\n",
        "</c>\n<c!\n",
        "<c! m=\"v\">\n```\n</c>\n",
        "#!/x\n<d/>\n"
    };
    constans character* f = formae[forma % (i32)6];
    i32 m = (i32)strlen(f);

    per (i = ZEPHYRUM; i < n && k + m < capacitas; i++)
    {
        memcpy(buffer + k, f, (size_t)m);
        k = k + m;
    }
    redde k;
}
```
   and call it where md called its generator, for depths 1, 2, 3, 10,
   100, 1000, 10000 and all six `forma` values (a region opened in one
   copy is closed by the next copy's `</c>` in forma 3 — deliberately
   mis-nested).
3. Keep the random-bytes (LCG), mutation (one byte in forty) and
   truncation (23 steps) generators exactly as md has them; they are
   language-independent.

- [ ] **Step 2: Run to green, plant, revert**

```sh
./briar/compile_probationes.sh totalitas
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`, with per-class counts
printed > 0 (the harness asserts each class ran).

Plant: in `briar_arbor.c`'s `_clausura_est`, remove the
`n < titulus.mensura + III` guard — a short line then reads past its
end; expected: a `CREDO_NON_RUIT` red (WIFSIGNALED) or a `TOTUM_DISPAR`
under the random-bytes class. Revert; green.

- [ ] **Step 3: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P1.3 - porta totalitatis (exemplar md): octeti fortuiti, mutationes, truncationes, nidificatio thistle VI formae ad X milia - numquam ruit (furca), semper arbor, emissio == fons; fons peccans in briar/build/totalitas_fractum.thistle. Culpa plantata (custos longitudinis clausurae sublatus) RUBRA.",
 ["briar/probationes/probatio_briar_totalitas.c"], ['briar'])
print(h, r)
EOF
```

---

### Task 8: Computus — the bench twin and its golden

**Files:**
- Create: `briar/fontes/briar_computus.h`, `briar/fontes/briar_computus.c`
- Create: `briar/instrumenta/computus.c`, `briar/computus.sh`
- Create: `briar/probationes/probatio_briar_computus.c`,
  `briar/probationes/fixa/computus/basis.tsv`

**Interfaces:**
- Consumes: Tasks 3 and 5.
- Produces: `b32 briar_computus_metiri(constans character* fons, i32 mensura, BriarComputus* exitus)`
  with the SAME eleven pinned columns as `MdComputus` (octeti, lexemata,
  nodi, octeti_stml, usus, commissa, otiosa, apex, alvei, allocationes,
  allocationes_arboris) plus the five `ms_*` timings.

- [ ] **Step 1: Derive all four files from md's twins by substitution**

```sh
cp md/fontes/md_computus.h briar/fontes/briar_computus.h
cp md/fontes/md_computus.c briar/fontes/briar_computus.c
cp md/instrumenta/computus.c briar/instrumenta/computus.c
cp md/computus.sh briar/computus.sh
cp md/probationes/probatio_md_computus.c briar/probationes/probatio_briar_computus.c
mkdir -p briar/probationes/fixa/computus
python3 - <<'EOF'
import re
subs = [
    ('md_computus', 'briar_computus'), ('MD_COMPUTUS_', 'BRIAR_COMPUTUS_'),
    ('MdComputus', 'BriarComputus'), ('md_arbor_parsare', 'briar_arbor_parsare'),
    ('md_stml_consilium', 'briar_stml_consilium'), ('MD_REGISTRUM', 'BRIAR_REGISTRUM'),
    ('MD_LEXICON', 'BRIAR_LEXICON'), ('MD_FONS_PLAGULAE', 'BRIAR_FONS_PLAGULAE'),
    ('"md_arbor.h"', '"briar_arbor.h"'), ('"md_stml.h"', '"briar_stml.h"'),
    ('"md_registrum.h"', '"briar_registrum.h"'), ('"md_lexicon.h"', '"briar_lexicon.h"'),
    ('probatio_md_computus', 'probatio_briar_computus'),
    ('md/probationes/fixa/computus/basis.tsv', 'briar/probationes/fixa/computus/basis.tsv'),
    ('md/probationes/fixa/md/%s', 'briar/probationes/fixa/thistle/%s'),
    ('./md/computus.sh', './briar/computus.sh'), ('MD_DIR', 'BRIAR_DIR'),
    ('"$BRIAR_DIR/../materia"', '"$BRIAR_DIR/../materia"'),
    ('md/build', 'briar/build'), ('md/compile_probationes.sh', 'briar/compile_probationes.sh'),
    ('parsatoris markdown', 'parsatoris thistle'), ('[computus] md', '[computus] briar'),
]
for p in ['briar/fontes/briar_computus.h', 'briar/fontes/briar_computus.c',
          'briar/instrumenta/computus.c', 'briar/computus.sh',
          'briar/probationes/probatio_briar_computus.c']:
    s = open(p).read()
    for a, b in subs:
        s = s.replace(a, b)
    # md/computus.sh includes md/fontes for md's own headers; briar needs md + briar
    if p == 'briar/computus.sh':
        s = s.replace('-I"$BRIAR_DIR/fontes"', '-I"$RADIX_DIR/md/fontes" -I"$BRIAR_DIR/fontes"')
    if p.endswith('probatio_briar_computus.c'):
        s = re.sub(r'#define NUMERUS_CORPORIS 4', '#define NUMERUS_CORPORIS 2', s)
        s = re.sub(r'hic_manens constans character\* CORPUS\[NUMERUS_CORPORIS\] = \{[^}]*\};',
                   'hic_manens constans character* CORPUS[NUMERUS_CORPORIS] = {\n    "salve.thistle",\n    "salve_vitreum.thistle"\n};', s)
    open(p, 'w').write(s)
    print('derivatum:', p)
EOF
grep -n 'md_\|MD_\|/md/' briar/fontes/briar_computus.c briar/instrumenta/computus.c briar/probationes/probatio_briar_computus.c briar/computus.sh | grep -v 'md/fontes' | head
```
Expected: the last grep prints nothing (no md identifiers left except the
include path in the script). `briar_computus.c` needs no further edits:
its measurement walk (`_numerare`, parse, emit, arbor write/read,
compare) is language-independent once the six names are substituted.
Read it once end to end to confirm every call resolves to a briar symbol
(`briar_arbor_parsare`, `briar_stml_consilium`, `BRIAR_REGISTRUM`,
`BRIAR_LEXICON`, `BRIAR_FONS_PLAGULAE`).

- [ ] **Step 2: Birth the golden with a named cause, then gate**

```sh
./briar/compile_probationes.sh computus            # RED: aurum absens
COMPUTUS_SCRIBERE=1 ./briar/compile_probationes.sh computus
./briar/compile_probationes.sh computus            # GREEN against the golden
cat briar/probationes/fixa/computus/basis.tsv
```
Expected: two golden rows (`salve.thistle`, `salve_vitreum.thistle`);
the named cause for the commit is "nativitas portae".

Plant: in `briar_arbor.c` `_prosam_deponere`, create the prosa node twice
(duplicate the `_nodus` call into a dead variable) — `allocationes`
rises; expected red on the golden compare. Revert; green.

- [ ] **Step 3: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P1.4 - computus (gemellus md_computus: columnae XI eaedem, tempora impressa non pinnata), instrumentum briar/instrumenta/computus.c + ./briar/computus.sh [-machina] [-iter N], porta probatio_briar_computus cum auro fixa/computus/basis.tsv (salve, salve_vitreum) - CAUSA: nativitas portae. Culpa plantata (allocatio duplex) RUBRA.",
 ["briar/fontes/briar_computus.h", "briar/fontes/briar_computus.c",
  "briar/instrumenta/computus.c", "briar/computus.sh",
  "briar/probationes/probatio_briar_computus.c",
  "briar/probationes/fixa/computus/basis.tsv"], ['briar'])
print(h, r)
EOF
```

---

### Task 9: The nexus — inner trees by identity (md, STML, attributes)

**Files:**
- Create: `briar/fontes/briar_nexus.h`, `briar/fontes/briar_nexus.c`
- Create: `briar/probationes/probatio_briar_nexus.c`

**Interfaces:**
- Consumes: Task 3 (`briar_pars`, `briar_lexema`, `briar_titulus`);
  `md_arbor_parsare` (md/fontes/md_arbor.h); `stml_legere`,
  `stml_attributum_capere`, `stml_attributum_habet` (stml.h);
  `internamentum_creare`; `chorda_aedificator_*`.
- Produces:
  `Xar* briar_nexus_texere(Piscina*, constans MateriaNodus* documentum, InternamentumChorda* intern)`
  (Xar of `BriarNexusRes`, document order, one per part);
  `chorda briar_nexus_attributum(constans BriarNexusRes*, constans character* titulus)`;
  `b32 briar_nexus_attributum_habet(constans BriarNexusRes*, constans character* titulus)`;
  `b32 briar_nexus_titulus_est(constans BriarNexusRes*, constans character* literae)`.

- [ ] **Step 1: Write the header**

```sh
./silva/scribe.sh briar/fontes/briar_nexus.h <<'EOF'
/* briar_nexus.h - Arbores interiores partium documenti thistle,
 * identitate nodi ligatae (exemplar materiae: arbor exterior octetos
 * ut lexema UNUM crudum tenet, arbor interior seorsum).
 *
 * Tria genera in plano I: prosa (et regio 'md') -> arbor markdown
 * (md_arbor_parsare super octetos lexematis); elementum STML ->
 * StmlNodus (stml_legere super extensionem); regio cruda -> tag
 * apertus ut '<nomen attrs/>' parsatus (attributa sine grammatica
 * secunda). Positiones: linea_initium = linea .thistle primi octeti
 * (prosa/elementum: pars; regio: contentum); vitia STML in lineas
 * .thistle translata (linea_erroris). Regiones C per silvam = planum
 * II (fabrica).
 */

#ifndef BRIAR_NEXUS_H
#define BRIAR_NEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "stml.h"

nomen enumeratio {
    BRIAR_NEXUS_MD = 0,      /* prosa aut <md!>: arbor_md */
    BRIAR_NEXUS_STML,        /* elementum: elementum (radix) */
    BRIAR_NEXUS_REGIO        /* regio cruda: elementum = tag apertus */
} BriarNexusGenus;

nomen structura {
    constans MateriaNodus* pars;
           BriarNexusGenus  genus;
             MateriaNodus* arbor_md;        /* MD; NIHIL alias */
                StmlNodus* elementum;       /* STML/REGIO; NIHIL si vitium */
                   chorda  titulus;         /* STML/REGIO: nomen; MD: vacua */
                   chorda  contentum;       /* REGIO: octeti contenti (vacua licet) */
                      i32  linea_initium;   /* linea .thistle (I-basata) */
                      i32  linea_erroris;   /* ZEPHYRUM = sanum */
                   chorda  causa;           /* nuntius parsatoris; vacua = sanum */
} BriarNexusRes;

/* Xar de BriarNexusRes, ordine partium; NIHIL = memoria */
Xar*
briar_nexus_texere (
                 Piscina* piscina,
    constans MateriaNodus* documentum,
     InternamentumChorda* intern);

/* attributum tagi (STML/REGIO); vacua si absens aut vitium */
chorda
briar_nexus_attributum (
    constans BriarNexusRes* res,
       constans character* titulus);

b32
briar_nexus_attributum_habet (
    constans BriarNexusRes* res,
       constans character* titulus);

b32
briar_nexus_titulus_est (
    constans BriarNexusRes* res,
       constans character* literae);

#endif /* BRIAR_NEXUS_H */
EOF
```

- [ ] **Step 2: Write the failing test**

```sh
./silva/scribe.sh briar/probationes/probatio_briar_nexus.c <<'EOF'
/* probatio_briar_nexus.c - Nexus: prosa -> arbor markdown, elementum
 * -> StmlNodus, regio -> attributa tagi; lineae .thistle; vitium STML
 * plantatum ad lineam .thistle translatum; <md!> ut prosa.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_nexus.h"
#include "briar_registrum.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "md_registrum.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* VITREUM =
    "briar/probationes/fixa/thistle/salve_vitreum.thistle";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* linea (I-basata) in qua 'acus' primum incipit in columna 0; ZEPHYRUM = nulla */
interior i32
_linea_ubi (
    constans character* fons,
                   i32  mensura,
    constans character* acus)
{
    i32 m = (i32)strlen(acus);
    i32 linea = I;
    i32 i;

    per (i = ZEPHYRUM; i + m <= mensura; i++)
    {
        si (   (i == ZEPHYRUM || fons[i - I] == '\n')
            && memcmp(fons + i, acus, (size_t)m) == ZEPHYRUM)
        {
            redde linea;
        }
        si (fons[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde ZEPHYRUM;
}

interior b32
_chorda_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM));
}

interior BriarNexusRes*
_res (
    Xar* nexus,
    i32  i)
{
    redde (BriarNexusRes*)xar_obtinere(nexus, i);
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_briar_nexus", 8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans salve_vitreum: octo partes, tria genera ---\n");
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
        MateriaNodus* doc;
                 Xar* nexus;
        BriarNexusRes* r;

        textus = _plagulam_legere(piscina, VITREUM, &mensura);
        CREDO_NON_NIHIL (textus);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        nexus = briar_nexus_texere(piscina, doc, intern);
        CREDO_NON_NIHIL (nexus);
        CREDO_AEQUALIS_I32 (xar_numerus(nexus), (i32)8);

        /* prosa prima: arbor md cum capitulo */
        r = _res(nexus, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_MD);
        CREDO_NON_NIHIL (r->arbor_md);
        CREDO_AEQUALIS_S32 (r->arbor_md->genus, (s32)MD_GENUS_DOCUMENTUM);
        CREDO_AEQUALIS_I32 (r->linea_initium, II);
        CREDO_AEQUALIS_I32 (r->linea_erroris, ZEPHYRUM);

        /* fenestra: attributa tria */
        r = _res(nexus, I);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_STML);
        CREDO_VERUM (briar_nexus_titulus_est(r, "fenestra"));
        CREDO_NON_NIHIL (r->elementum);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "titulus"), "salve"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "latitudo"), "640"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "altitudo"), "400"));
        CREDO_FALSUM (briar_nexus_attributum_habet(r, "profunditas"));
        CREDO_AEQUALIS_I32 (r->linea_initium,
            _linea_ubi(textus, mensura, "<fenestra"));

        /* html: regio sine attributis */
        r = _res(nexus, III);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_REGIO);
        CREDO_VERUM (briar_nexus_titulus_est(r, "html"));
        CREDO_NON_NIHIL (r->elementum);
        CREDO_FALSUM (briar_nexus_attributum_habet(r, "methodus"));
        CREDO_CHORDA_INCIPIT (r->contentum, chorda_ex_literis("<h1>", piscina));
        CREDO_AEQUALIS_I32 (r->linea_initium,
            _linea_ubi(textus, mensura, "<html!>") + I);

        /* c methodus="salve" */
        r = _res(nexus, (i32)5);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_REGIO);
        CREDO_VERUM (briar_nexus_titulus_est(r, "c"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "methodus"), "salve"));
        CREDO_AEQUALIS_I32 (r->linea_initium,
            _linea_ubi(textus, mensura, "<c! methodus=") + I);

        /* c munus="probatio" */
        r = _res(nexus, (i32)7);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "munus"), "probatio"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "methodus"), ""));
    }

    imprimere("\n--- Probans casus inlineares ---\n");
    {
        MateriaNodus* doc;
                 Xar* nexus;
        BriarNexusRes* r;
        constans character* a = "<md!>\n# T\n</md>\n";
        constans character* b = "prosa\n<fenestra>\n<a>\n</fenestra>\n";
        constans character* c = "<c!\\ methodus=\"m\">\n  x\n</c>\n";
        constans character* d = "<c! bad=>\nx\n</c>\n";

        /* <md!> = prosa explicita, linea contenti II */
        doc = briar_arbor_parsare(piscina, a, (i32)strlen(a));
        nexus = briar_nexus_texere(piscina, doc, intern);
        r = _res(nexus, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_MD);
        CREDO_NON_NIHIL (r->arbor_md);
        CREDO_AEQUALIS_I32 (r->linea_initium, II);

        /* vitium STML plantatum: <a> non clausum intra elementum lineae II
         * -> linea erroris .thistle >= II, causa non vacua, elementum NIHIL */
        doc = briar_arbor_parsare(piscina, b, (i32)strlen(b));
        nexus = briar_nexus_texere(piscina, doc, intern);
        CREDO_AEQUALIS_I32 (xar_numerus(nexus), II);
        r = _res(nexus, I);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_STML);
        CREDO_NIHIL (r->elementum);
        CREDO_MAIOR_AUT_AEQUALIS_I32 (r->linea_erroris, II);
        CREDO_MINOR_AUT_AEQUALIS_I32 (r->linea_erroris, (i32)4);
        CREDO_CHORDA_NON_VACUA (r->causa);

        /* '\' dedens: attributa tamen leguntur */
        doc = briar_arbor_parsare(piscina, c, (i32)strlen(c));
        nexus = briar_nexus_texere(piscina, doc, intern);
        r = _res(nexus, ZEPHYRUM);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "methodus"), "m"));

        /* tag apertus malformatus: vitium nominatum, linea I */
        doc = briar_arbor_parsare(piscina, d, (i32)strlen(d));
        nexus = briar_nexus_texere(piscina, doc, intern);
        r = _res(nexus, ZEPHYRUM);
        CREDO_NIHIL (r->elementum);
        CREDO_AEQUALIS_I32 (r->linea_erroris, I);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "bad"), ""));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
EOF
./briar/compile_probationes.sh nexus
```
Expected: `FRACTA (compilatio): probatio_briar_nexus`.

- [ ] **Step 3: Write the nexus**

```sh
./silva/scribe.sh briar/fontes/briar_nexus.c <<'EOF'
/* briar_nexus.c - Vide briar_nexus.h. */

#include "briar_nexus.h"
#include "briar_arbor.h"
#include "briar_registrum.h"
#include "chorda_aedificator.h"
#include "materia_token.h"
#include "md_arbor.h"
#include <string.h>

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_titulus_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM);
}

/* '<nomen! attrs>' (cum terminatore) -> '<nomen attrs/>' */
interior chorda
_tagum_rescribere (
             Piscina* piscina,
    constans MateriaToken* apertum,
              chorda  titulus)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
               chorda  v = apertum->valor;
                  i32  i;
                  i32  f;

    si (aed == NIHIL)
    {
        redde _vacua();
    }
    /* finis: terminator et spatia finalia abscisa */
    f = v.mensura;
    dum (f > ZEPHYRUM && ((character)v.datum[f - I] == '\n'
        || (character)v.datum[f - I] == '\r'
        || (character)v.datum[f - I] == ' '
        || (character)v.datum[f - I] == '\t'))
    {
        f = f - I;
    }
    /* '<' + nomen */
    i = I + titulus.mensura;
    chorda_aedificator_appendere_literis(aed, "<");
    chorda_aedificator_appendere_chorda(aed, titulus);
    /* '!' et '\' praeteriti */
    dum (i < f && ((character)v.datum[i] == '!' || (character)v.datum[i] == '\\'))
    {
        i = i + I;
    }
    /* reliquum sine '>' finali (aut '/>') */
    si (f >= II && (character)v.datum[f - II] == '/' && (character)v.datum[f - I] == '>')
    {
        f = f - II;
    }
    alioquin si (f >= I && (character)v.datum[f - I] == '>')
    {
        f = f - I;
    }
    si (i < f)
    {
        chorda_aedificator_appendere_chorda(aed, chorda_sectio(v, i, f));
    }
    chorda_aedificator_appendere_literis(aed, "/>");
    redde chorda_aedificator_finire(aed);
}

interior vacuum
_res_incipere (
        BriarNexusRes* r,
    constans MateriaNodus* pars,
      BriarNexusGenus  genus)
{
    r->pars           = pars;
    r->genus          = genus;
    r->arbor_md       = NIHIL;
    r->elementum      = NIHIL;
    r->titulus        = _vacua();
    r->contentum      = _vacua();
    r->linea_initium  = ZEPHYRUM;
    r->linea_erroris  = ZEPHYRUM;
    r->causa          = _vacua();
}

interior vacuum
_stml_legere_in (
           Piscina* piscina,
    InternamentumChorda* intern,
       BriarNexusRes* r,
            chorda  textus,
               i32  linea_basis)
{
    StmlResultus res = stml_legere(textus, piscina, intern);

    si (res.successus && res.elementum_radix != NIHIL)
    {
        r->elementum = res.elementum_radix;
        redde;
    }
    r->elementum      = NIHIL;
    r->causa          = res.error;
    r->linea_erroris  = linea_basis + ((res.linea_erroris > ZEPHYRUM)
        ? res.linea_erroris - I : ZEPHYRUM);
    si (r->causa.mensura == ZEPHYRUM)
    {
        r->causa = chorda_ex_literis("stml: parsura fracta sine nuntio",
            piscina);
    }
}

Xar*
briar_nexus_texere (
                 Piscina* piscina,
    constans MateriaNodus* documentum,
     InternamentumChorda* intern)
{
    Xar* nexus;
    i32  n;
    i32  i;

    si (piscina == NIHIL || documentum == NIHIL)
    {
        redde NIHIL;
    }
    nexus = xar_creare(piscina, (i32)magnitudo(BriarNexusRes));
    si (nexus == NIHIL)
    {
        redde NIHIL;
    }
    n = briar_partes_numerus(documentum);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* pars = briar_pars(documentum, i);
               BriarNexusRes* r    = (BriarNexusRes*)xar_addere(nexus);

        si (r == NIHIL)
        {
            redde NIHIL;
        }
        si (pars->genus == (s32)BRIAR_GENUS_PROSA)
        {
            MateriaToken* t = briar_lexema(pars, (i32)BRIAR_PROSA_TOK);

            _res_incipere(r, pars, BRIAR_NEXUS_MD);
            r->linea_initium  = t->linea;
            r->arbor_md       = md_arbor_parsare(piscina,
                (constans character*)t->valor.datum, t->valor.mensura);
        }
        alioquin si (pars->genus == (s32)BRIAR_GENUS_ELEMENTUM)
        {
            MateriaToken* t = briar_lexema(pars, (i32)BRIAR_ELEMENTUM_TOK);

            _res_incipere(r, pars, BRIAR_NEXUS_STML);
            r->titulus        = briar_titulus(pars);
            r->linea_initium  = t->linea;
            _stml_legere_in(piscina, intern, r, t->valor, t->linea);
        }
        alioquin
        {
            MateriaToken* apertum   = briar_lexema(pars, (i32)BRIAR_REGIO_APERTUM);
            MateriaToken* contentum = briar_lexema(pars, (i32)BRIAR_REGIO_CONTENTUM);
                  chorda  titulus   = briar_titulus(pars);

            _res_incipere(r, pars, BRIAR_NEXUS_REGIO);
            r->titulus        = titulus;
            r->contentum      = (contentum != NIHIL) ? contentum->valor : _vacua();
            r->linea_initium  = apertum->linea + I;
            _stml_legere_in(piscina, intern, r,
                _tagum_rescribere(piscina, apertum, titulus), apertum->linea);
            /* <md!> = prosa explicita: arbor markdown super contentum */
            si (_titulus_est(titulus, "md"))
            {
                r->genus     = BRIAR_NEXUS_MD;
                r->arbor_md  = md_arbor_parsare(piscina,
                    (constans character*)r->contentum.datum,
                    r->contentum.mensura);
            }
        }
    }
    redde nexus;
}

chorda
briar_nexus_attributum (
    constans BriarNexusRes* res,
       constans character* titulus)
{
    chorda* v;

    si (res == NIHIL || res->elementum == NIHIL)
    {
        redde _vacua();
    }
    v = stml_attributum_capere(res->elementum, titulus);
    redde (v != NIHIL) ? *v : _vacua();
}

b32
briar_nexus_attributum_habet (
    constans BriarNexusRes* res,
       constans character* titulus)
{
    si (res == NIHIL || res->elementum == NIHIL)
    {
        redde FALSUM;
    }
    redde stml_attributum_habet(res->elementum, titulus);
}

b32
briar_nexus_titulus_est (
    constans BriarNexusRes* res,
       constans character* literae)
{
    redde (b32)(res != NIHIL && res->titulus.mensura > ZEPHYRUM
        && _titulus_est(res->titulus, literae));
}
EOF
./briar/compile_probationes.sh nexus
```
Expected: `BRIAR PROBATIONES: 1/1 praeteritae`. Two API details, both verified against the headers: `chorda_aedificator_appendere_chorda` (include/chorda_aedificator.h:76) appends a `chorda`; `stml_attributum_capere` takes a non-const `StmlNodus*`, so `res->elementum` is stored non-const on purpose. `md_arbor_parsare` takes `constans character*` from a `chorda`'s `i8*` — the cast above adds `constans`, which `-Wcast-qual` permits.

- [ ] **Step 4: Plant, revert**

In `_tagum_rescribere`, stop skipping the `!` (delete the `dum` loop) —
the rewritten tag becomes `<c! methodus=…/>`, an STML raw form with no
close; expected: `elementum` NIHIL on every region, `methodus` assertions
red. Revert; green.

- [ ] **Step 5: Commit**

```sh
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: P2.1 - nexus: arbores interiores identitate partis ligatae - prosa et <md!> -> md_arbor_parsare super octetos lexematis, elementum STML -> stml_legere super extensionem, regio cruda -> tag apertus ut '<nomen attrs/>' rescriptus et parsatus (attributa methodus/munus sine grammatica secunda); lineae .thistle (linea_initium, linea_erroris ex vitio STML translata, causa). Porta probatio_briar_nexus: salve_vitreum VIII res, fenestra III attributa, methodus salve, munus probatio, lineae per _linea_ubi, <md!>, vitium plantatum translatum, '\\' dedens, tag malformatus. Culpa plantata ('!' non praeteritum) RUBRA.",
 ["briar/fontes/briar_nexus.h", "briar/fontes/briar_nexus.c",
  "briar/probationes/probatio_briar_nexus.c"], ['briar'])
print(h, r)
EOF
```

---

### Task 10: Seal plan 1 — worklog, orientation, spec amendments, whole suite

**Files:**
- Create: `briar/fontes/briar.worklog.md`, `briar/CLAUDE.md`
- Modify: `project-specs/briar-spec.md` (§2 table row `elementum`, §3.1 regio loci names, §8 P0–P2 marked done)
- Modify: `briar/probationes/fixa/FONTES.md` (adversa list is already there; add the computus golden line)

**Interfaces:** none new.

- [ ] **Step 1: Run the whole briar suite and the neighbours it touches**

```sh
./briar/compile_probationes.sh
./md/compile_probationes.sh registrum
./materia/compile_probationes.sh
```
Expected: `BRIAR PROBATIONES: 9/9 praeteritae`; md and materia unchanged
(they were only consumed). If the briar total is not 9, a task's test
file is missing from `briar/probationes/` — the runner counts files.

- [ ] **Step 2: Write the worklog (English allowed) with the findings this plan produced**

`briar/fontes/briar.worklog.md` — one dated section, 2026-09-xx, with
at least these entries, each stated as measured during the tasks (fill
the numbers from the runs):

```
# briar — worklog

## 2026-09-xx — plan 1 (parser) sealed

- Extent of an STML element: column-0 `</name>` or single self-closing
  line (spec §2 amended). A lexeme walk was designed and dropped: the
  column-0 law already governs every other boundary, and a multi-line
  self-closing element is the one edge it costs (named in adversa? no —
  named here: `<fenestra\n titulus=…/>` becomes elementum-non-clausum).
- Fence law: fences toggle only in the lexer's prose state; inside a
  raw region no fence is tracked, so ``` inside C never affects the
  scan. The crude lexer (per-line, no region state) DOES toggle on a
  ``` inside a region — it is a classification gate, not the arbor.
- FIDELIS comparator: passed/failed on <fixture> — because materia's
  `_cursorem_promovere` counts '\n' inside token values. (Record the
  actual outcome and, if failed, the campus named.)
- Byte law is structure-independent: the Task 3 plant (close never
  found) stayed green on emission while red on structure.
- Canon loaded by path; `<arbor>` root shared across materia dialects
  (as css/md). Seal pinned: <hex>.
- Totality: nesting to 10,000 copies of six mis-nested forms; slowest
  class <n> ms.
- Computus golden born: rows for salve, salve_vitreum (cause: birth).
- Nexus: raw open tag rewritten to `<name attrs/>` and parsed by stml —
  no second attribute grammar; `!`/`\` stripped; a malformed tag is a
  named vitium at the region's line.
```

- [ ] **Step 3: Write `briar/CLAUDE.md` (orientation, MAP + gates + laws)**

```
# briar — orientation

*You are reading this because you touched a file in `briar/`.*

briar reads `.thistle` files (literate C89 scripts: markdown prose,
STML config tags, raw `<c!>` regions, a `#!` line). Plan 1 built the
PARSER as a materia client; plan 2 builds the fabrica and the binary.
Spec: `project-specs/briar-spec.md`; plans `briar-plan-1-parsator.md`
(done), `briar-plan-2-fabrica.md` (next). Findings: `fontes/briar.worklog.md`.

## Map
- `briar_registrum` (5 genera, 14 loci) · `briar_lexicon` (8 token
  genera, prefix `briar-`, no LINEA: newlines live inside values)
- `briar_lexema`: line table, column-0 classification (tag open/close,
  `!` raw, `\` dedent, `/>` self-closing), markdown FENCE state (inside a
  fence every line is prose), token factory across lines
- `briar_arbor`: regions → tree (documentum, interpres, prosa, regio,
  elementum); always a tree; `briar_emittere` == source bytes
- `briar_stml`: projection consilium + origin hook; `briar/grammatica/
  briar.canon` (load by path; seal pinned)
- `briar_nexus`: inner trees by part identity — md for prose and
  `<md!>`, stml for elements and for raw open tags (attributes
  `methodus=`, `munus=`); `.thistle` line offsets and translated errors
- `briar_computus`: bench twin; instruments `./briar/arbor.sh`,
  `./briar/computus.sh`

## Gates (`./briar/compile_probationes.sh [filter]`; exit 2 = NOTHING RAN)
registrum · lexema · arbor · stml · canon · totalitas · computus · nexus
(nine files; every one born red by a planted fault — see the worklog).
Fixtures: `probationes/fixa/thistle/` (+ `adversa/`), inventory in
`fixa/FONTES.md`; computus golden `fixa/computus/basis.tsv`
(`COMPUTUS_SCRIBERE=1` + a named cause).

## Laws
- Column 0 is the only place briar looks. Prose needs no escaping.
- A raw region ends only at a column-0 `</name>`; an STML element at
  its column-0 close or its own self-closing line.
- Every byte belongs to exactly one token; derived tokens (fons 1) hold
  none. Emission is byte-exact by construction.
- Never a second grammar for attributes: the open tag is rewritten to
  `<name attrs/>` and given to stml.
- Worktree rules (until merged): `./silva/scribe.sh` not `~/.bin/scribe`;
  no ledger residents; commit with `silva.commissio_umbra(..., ['briar'])`.
```

- [ ] **Step 4: Amend the spec (three targeted edits)**

In `project-specs/briar-spec.md`:
1. §2 table, row `elementum`, column "ends": replace "the balanced close
   found by the STML lexeme stream (`stml_lexemata_colligere`, exists —
   stml.h:345), or the `/>`" with "the first later column-0 line
   `</name>`, or the `/>` when the opening line itself is self-closing
   (plan 1 amendment: the column-0 law governs every boundary; a
   multi-line self-closing element is a named vitium)".
2. §3.1 registry table, row `regio`: `apertum · titulus · contentum ·
   clausum · vitium` (drop the `tok_` prefixes; canon element names).
3. §8: prefix P0, P1, P2 with "DONE (plan 1, <commit>)".

- [ ] **Step 5: Final whole-suite run and commit**

```sh
./briar/compile_probationes.sh | tail -3
python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
h, r = silva.commissio_umbra(
 "briar: planum I SIGNATUM - parsator thistle cliens materiae: portae IX/IX (registrum lexema arbor stml canon totalitas computus nexus), instrumenta arbor.sh/computus.sh, worklog inventorum, briar/CLAUDE.md orientatio; spec par. II (extensio elementi = clausura in columna 0 aut linea se-claudens), par. III.1 (loci regionis sine praefixo), par. VIII (P0-P2 facta) emendata. Proximum: planum II (silva in nexu, fabrica, binarium).",
 ["briar/fontes/briar.worklog.md", "briar/CLAUDE.md",
  "project-specs/briar-spec.md", "briar/probationes/fixa/FONTES.md"],
 ['briar'])
print(h, r)
EOF
```

---

## Self-review (done while writing; the executor re-runs it at the end)

**Spec coverage.** §2 format: Tasks 2–4 (laws, fences, vitia, vocabulary
via nexus attributes in Task 9). §3.1 registry/lexicon: Task 1. §3.2
lexer/arbor: Tasks 2–3. §3.3 projection/canon/nexus (md, STML,
attributes): Tasks 5, 6, 9; the silva inner kind and its capsula
provider are plan 2 by this plan's scoping note (spec §3.3 fourth kind,
§8 P2 "C regions through silva"). §6 gates: registrum (1), lexema (2),
arbor (3–4), stml (5), canon (6), totalitas (7), computus (8), nexus (9);
`probatio_silex` unchanged is plan 2's concern (no silex touch here).
§7 modules: all parser-side modules present; fabrica/binary/struere/fumus
are plan 2. §8 P0–P2: covered; P2's "planted bare-parse fault" belongs
to the silva kind, plan 2. Gaps, deliberate: silva inner trees; the
fabrica; pythonica `arbor()` dispatch for `.thistle` (no consumer yet —
noted in the worklog by Task 10).

**Placeholder scan.** Every code step carries the code. The three
"derive by substitution" steps (Tasks 7, 8) list the exact substitution
table and the exact hand edits; nothing says "similar to". No step describes
work without showing it.

**Type consistency.** `briar_lexema_fabricare(BriarFabrica*, s32 genus,
s32 ab, s32 ad)` is used with those types in Tasks 3 and 9; `briar_pars`
/ `briar_lexema` / `briar_titulus` / `briar_vitium` / `briar_emittere`
signatures match between header (Task 3) and uses (Tasks 4, 5, 6, 7, 8,
9); `BriarNexusRes` fields used in Task 9's test are exactly the header's;
locus enums `BRIAR_REGIO_APERTUM/TITULUS/CONTENTUM/CLAUSUM/VITIUM` and
`BRIAR_ELEMENTUM_TOK/TITULUS/VITIUM` are used with those names
everywhere; the canon's slot elements are named the same as the registry
loci (`apertum`, `titulus`, `contentum`, `clausum`, `vitium`, `tok`,
`interpres`, `partes`, `finis`).

## Execution handoff

Plan complete and saved to `project-specs/briar-plan-1-parsator.md`
(worktree `../rhubarb-briar`). Two execution options:

1. **Subagent-driven** (recommended by the skill) — a fresh subagent per
   task, review between tasks. NOTE for this repo: subagents must be
   launched with their working directory pinned to the worktree, must
   not run ledger residents, and inherit the commit rule (explicit paths
   through `silva.commissio_umbra`).
2. **Inline execution** — the session executes tasks in order with a
   checkpoint after each commit.

Plan 2 (`briar-plan-2-fabrica.md`) follows plan 1's seal: silva in the
nexus with the capsula include provider, silex API promotion, fabrica
with goldens, the binary, struere, fumus, first shebang run.
