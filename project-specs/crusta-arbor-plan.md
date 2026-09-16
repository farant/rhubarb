# crusta_arbor Plan — the shell parser on materia (sixth client)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> (inline, per house convention — no subagents) to implement this plan
> task-by-task, ONE TASK PER TURN with Fran's approval. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** The shell tree on materia — POSIX sh plus bash 5.2's whole
default-option syntax — total, byte-exact through the emitter, projected to
STML and read back through two cycles and the comparator, judged by a
hand-written canon, benched by a computus twin, and proven against bash's
own deparse (`declare -f`) and `bash -n`. Fourteen gates, each born red.

**Architecture:** `crusta-arbor-spec.md` §§2–8. `crusta_lector` (a pull
lexer, a function of mode and position, with regions and a heredoc queue)
→ `crusta_arbor` (an ITERATIVE builder over a frame stack that IS the mode
stack; a ligator binding trivia backward through the last newline) →
`MateriaNodus` against the generated `CRUSTA_REGISTRUM`. `crusta_arithmetica`
is the precedence machine the builder drives for `(( ))`. Emission
`materia_scribere_nodum`; projection `materia_arbor_scribere_nodum` /
`materia_arbor_legere`; oracle `materia_arbor_aequalis`; cooked view
`crusta_coctum` against bash.

**Tech Stack:** C89 via latina.h, house flags (`tools/vexilla.sh`), credo,
piscina, chorda, Xar, processus (the oracle instrument), materia
(`materia/fontes`), the generator `./materia/coquere.sh`, canon, pythonica
(`silva.commissio`, `silva.planta`). Oracle binary `/opt/homebrew/bin/bash`
(5.2.15), never `/bin/bash` (3.2).

**Spec:** `project-specs/crusta-arbor-spec.md` — §4 is the declaration to
transcribe, §3 the lexer, §5 the builder, §6 arithmetic, §7 the cooked view
and oracles, §8 the gates. Sibling precedents: `html-arbor-plan.md` (task
shape, closed), `html/CLAUDE.md` and `md/CLAUDE.md` (the client shape as
built), `md/fontes/md_lexema.c` (a client-owned lexer making
`MateriaToken`s), `html/fontes/html_adaptare.c` (the ligator shape).

## Global Constraints

- **Latin throughout** — identifiers, comments, messages. `html`, `stml`,
  `token`, `offset`, `index`, `byte` are already permitted; this plan adds
  `heredoc` and `bash` to `oratio/glossarium.stml` as `ignotum-permissum`
  in P1 (the lint refuses them otherwise).
- **Every latina.h word is a FORBIDDEN identifier** (`nomen`, `registrum`,
  `magnitudo`, `structura`, `casus`, `brevis`, `duplex`, `si`, `per`,
  `dum`, `fac`, `exire`…); single capitals `I V X L C D M` are Roman
  numerals; two-letter locals are refused by the lint; a digit suffix is a
  NEW word. The shell keywords `if for while do case` are therefore NEVER
  identifiers: loci and enums name ROLES (`tok_apertura`, `tok_deinde`,
  `tok_clausura`, `tok_in`).
- **`chorda` is NOT null-terminated. `i32`/`i64` are UNSIGNED**; anything
  subtraction-derived or a sentinel takes `s32`/`s64` (the cursor, region
  ends, `-I` sentinels).
- **`??-` in a string literal is a trigraph; `<*/>` or `lib/*.c` inside a
  BLOCK COMMENT breaks it; `" */"` inside a doc comment closes it; a
  `<tag>` at the START of a comment is an annotation anchor** — write
  `sedes/…` or put the tag mid-sentence. Shell text in C comments: `$(`
  and backticks are harmless; `*/` inside a glob example is not.
- **New C files are written with `./silva/scribe.sh <via> <<'EOF'`**; a
  generated `.c` needs ITS OWN `.h`; new identifier WORDS are checked with
  `./oratio/quaere.sh w1 w2…` BEFORE staging (untracked files are
  invisible to `-nova`); `./silva/nexus.sh <symbol>` before
  `./oratio/vocabula.sh -nova`; a missing word is a glossary line.
- **House format**: `./silva/formator.sh <viae> -scribere` then `-vitia`
  before every commit (banners of 50 `=`, lines ≤ 72; comment tails that
  overflow go on their own line).
- **Generated files are never hand-edited**: `crusta_registrum_coctum.{h,c}`
  come from `./materia/coquere.sh crusta/grammatica/crusta.registrum.stml
  -scribere`; the registrum probatio compares bytes. Genera and loci are
  APPENDED, never reordered; the seal in `crusta.canon` moves by hand with
  a cause. Node sizes come from `CRUSTA_REGISTRUM.genera[g].loci_numerus`,
  never a hand count (html's O7c lesson).
- **Exit 2 from any runner = NOTHING RAN.** Never gate on exit 0 alone;
  never read `$?` after a pipe (zsh `${pipestatus[1]}`). The runner locks
  `crusta/build/cursor.sera`.
- **Runners**: `./crusta/compile_probationes.sh [filter]` (auto-discovers
  `fontes/*.c` and `probationes/probatio_*.c`; compiles materia's sources
  into `crusta/build/`), `./materia/compile_probationes.sh`,
  `./materia/shim_probare.sh`. Logs: `build/test_logs/crusta.log`; one
  probatio's credo lines: `build/test_logs/singulae/<probatio>.log`.
- **Every gate red at birth by a planted fault** that fails the ASSERTION,
  not the build: `silva.planta(via, vetus, novus, porta_nomen, filtrum,
  tolerans=…)` (a plant inside a comment needs `tolerans='verba'`; plants
  in non-C files are done by hand). A plant that does not fire is a
  measurement of the TEST.
- **Docs first, gates last, then commit** through
  `silva.commissio(msg, viae, portae)` with explicit paths — never
  `git add -A`. A gate already green via `silva.porta`/`planta` on an
  unchanged tree is not rerun by commissio (write worklog/README first).
  Commit messages end with `Co-Authored-By: Claude Fable 5.1
  <noreply@anthropic.com>`.
- **Never stage Fran's files**: `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`; never touch the untracked
  `project-specs/exempla/*.html`.
- **Silva stays FROZEN; materia is expected to need ZERO changes** (html
  needed none). A substrate defect found here gets its own gate in
  `materia/probationes`, its own commit, and `./materia/shim_probare.sh`.
- **Positions are byte offsets** + 1-based linea/columna; the source is
  never copied (tokens point into it), so test sources are allocated from
  the piscina, never a stack buffer.
- **Every fetch is an external action — ASK Fran before it** (P6, the
  FreeBSD cases). Bash's own `tests/` are GPLv3: never vendored.
- After any edit to `.stml`/`.canon` files: `./tools/natura_struere.sh`.
- **Numbers and names fixed by the spec:** 50 genera, 150 loci, 15 modes,
  the token genera of §3, the dialect tables of §5. A task that needs to
  change one of them says so in its commit message and moves the pins.

---

## Task P1: Scaffold — declaration, generated registry, dialect, lexicon, runner, registrum gate

**Files:**
- Create: `crusta/grammatica/crusta.registrum.stml`
- Generate: `crusta/fontes/crusta_registrum_coctum.{h,c}`
- Create: `crusta/fontes/crusta_registrum.h`, `crusta/fontes/crusta_lexicon.{h,c}`
- Create: `crusta/compile_probationes.sh` (from `html/compile_probationes.sh`)
- Create: `crusta/probationes/probatio_crusta_registrum.c`
- Create: `crusta/CLAUDE.md` (orientation stub: what this is, the spec, the plan, the runner)
- Modify: `oratio/glossarium.stml` (two `ignotum-permissum` lines), `pythonica/silva.py` (`PORTAE`, `FORMAE`, `SUITAE`)

**Interfaces:**
- Consumes: `materia_lexicon.h` (`MateriaLexGenus {titulus, orthographia,
  species, munus}`, `MateriaLexiconCoctum {genera, numerus_generum,
  praefixum_tagi, genus_spatii}`), `materia_coctor.h`
  (`materia_registrum_recens`), `materia_arbor.h`
  (`materia_arbor_genus_index`, `materia_arbor_consilium_nudum`).
- Produces: `CRUSTA_REGISTRUM`, `CrustaGenus` (`CRUSTA_GENUS_PROGRAMMA` …
  `CRUSTA_GENUS_IUDICIUM_INCLUSA`, `CRUSTA_GENUS_NUMERUS_GENERUM` = 50; a
  hyphen in a title becomes `_`: `CRUSTA_GENUS_PARS_LITTERALIS`),
  `CRUSTA_LEXICON`, `CrustaLexGenus`, `CrustaDialectus`, `CRUSTA_BASH`,
  the slot enums below.

- [x] **Step 1: Glossary** — after the `html` line of `oratio/glossarium.stml`
  (line 744 today) add:

```xml
  <vocabulum lemma="heredoc" lingua="anglica" classis="ignotum-permissum" nota="shell; sedes crusta (2026-09-16)">
    <forma textus="heredoca"/>
  </vocabulum>
  <vocabulum lemma="bash" lingua="anglica" classis="ignotum-permissum" nota="dialectus crustae; CRUSTA_BASH (2026-09-16)"/>
```

  then `./tools/natura_struere.sh`.

- [x] **Step 2: The declaration** — `crusta/grammatica/crusta.registrum.stml`
  verbatim from spec §4 (fifty genera in that order; loci in BYTE order).
  Header comment: the spec path, "genera APPENDUNTUR, numquam
  interponuntur", the C5 note on `heredoc` as the `corpus` target (`id`
  written by materia), and C13 ("nihil reservatum; sub-parsatores super
  verba futurum nominatum, spec IX"). Judge it:
  `bin/canon_examen crusta/grammatica/crusta.registrum.stml` — no vitia
  (exit 2 = nothing judged = the root is not registered; check the path).

- [x] **Step 3: Generate** —
  `./materia/coquere.sh crusta/grammatica/crusta.registrum.stml -scribere`,
  then the same without `-scribere` (compare mode, exit 0 = recens). Read
  the generated header once: `CrustaGenus`, `CRUSTA_GENUS_NUMERUS_GENERUM`,
  `CRUSTA_REGISTRUM`; confirm `numerus_locorum == 150`.

- [x] **Step 4: Slot enums and the dialect (`crusta_registrum.h`)** —
  includes the generated header; one enum per genus in declaration order,
  members in locus order (the registrum gate asserts each by title):

```c
nomen enumeratio { CRUSTA_PROGRAMMA_LIBERI = 0, CRUSTA_PROGRAMMA_CAUDA }
    CrustaProgrammaLocus;
nomen enumeratio { CRUSTA_SEPARATOR_TOK = 0 } CrustaSeparatorLocus;
nomen enumeratio { CRUSTA_MALUM_TOKENS = 0 } CrustaMalumLocus;
nomen enumeratio { CRUSTA_HEREDOC_PARTES = 0, CRUSTA_HEREDOC_TOK_DELIMITATOR,
    CRUSTA_HEREDOC_TOK_FINIS } CrustaHeredocLocus;
nomen enumeratio { CRUSTA_CATENA_LIBERI = 0 } CrustaCatenaLocus;
nomen enumeratio { CRUSTA_PIPA_PRAEFIXA = 0, CRUSTA_PIPA_LIBERI } CrustaPipaLocus;
nomen enumeratio { CRUSTA_OPERATOR_TOK = 0 } CrustaOperatorLocus;
nomen enumeratio { CRUSTA_IMPERIUM_LIBERI = 0 } CrustaImperiumLocus;
nomen enumeratio { CRUSTA_ASSIGNATIO_TOK_TITULUS = 0,
    CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM, CRUSTA_ASSIGNATIO_TOK_OPERATOR,
    CRUSTA_ASSIGNATIO_VALOR } CrustaAssignatioLocus;
nomen enumeratio { CRUSTA_TABULATUM_TOK_APERTURA = 0, CRUSTA_TABULATUM_LIBERI,
    CRUSTA_TABULATUM_TOK_CLAUSURA } CrustaTabulatumLocus;
nomen enumeratio { CRUSTA_REDIRECTIO_TOK_FD = 0, CRUSTA_REDIRECTIO_TOK_OPERATOR,
    CRUSTA_REDIRECTIO_SCOPUS, CRUSTA_REDIRECTIO_CORPUS } CrustaRedirectioLocus;
nomen enumeratio { CRUSTA_FUNCTIO_TOK_VERBUM = 0, CRUSTA_FUNCTIO_TOK_TITULUS,
    CRUSTA_FUNCTIO_TOK_APERTURA, CRUSTA_FUNCTIO_TOK_CLAUSURA,
    CRUSTA_FUNCTIO_CORPUS } CrustaFunctioLocus;
nomen enumeratio { CRUSTA_GREX_TOK_APERTURA = 0, CRUSTA_GREX_LIBERI,
    CRUSTA_GREX_TOK_CLAUSURA, CRUSTA_GREX_REDIRECTIONES } CrustaGrexLocus;
nomen enumeratio { CRUSTA_CRUSTULA_TOK_APERTURA = 0, CRUSTA_CRUSTULA_LIBERI,
    CRUSTA_CRUSTULA_TOK_CLAUSURA, CRUSTA_CRUSTULA_REDIRECTIONES }
    CrustaCrustulaLocus;
nomen enumeratio { CRUSTA_CONDITIO_TOK_APERTURA = 0, CRUSTA_CONDITIO_PROBATIO,
    CRUSTA_CONDITIO_TOK_DEINDE, CRUSTA_CONDITIO_LIBERI, CRUSTA_CONDITIO_RAMI,
    CRUSTA_CONDITIO_TOK_CLAUSURA, CRUSTA_CONDITIO_REDIRECTIONES }
    CrustaConditioLocus;
nomen enumeratio { CRUSTA_RAMUS_TOK_APERTURA = 0, CRUSTA_RAMUS_PROBATIO,
    CRUSTA_RAMUS_TOK_DEINDE, CRUSTA_RAMUS_LIBERI } CrustaRamusLocus;
nomen enumeratio { CRUSTA_ITERATIO_TOK_APERTURA = 0, CRUSTA_ITERATIO_TOK_TITULUS,
    CRUSTA_ITERATIO_TOK_IN, CRUSTA_ITERATIO_VERBA, CRUSTA_ITERATIO_TOK_SEPARATOR,
    CRUSTA_ITERATIO_CURSUS, CRUSTA_ITERATIO_REDIRECTIONES } CrustaIteratioLocus;
nomen enumeratio { CRUSTA_CYCLUS_TOK_APERTURA = 0, CRUSTA_CYCLUS_TOK_PARENTHESIS,
    CRUSTA_CYCLUS_LIBERI, CRUSTA_CYCLUS_TOK_PARENTHESIS_CLAUSURA,
    CRUSTA_CYCLUS_TOK_SEPARATOR, CRUSTA_CYCLUS_CURSUS,
    CRUSTA_CYCLUS_REDIRECTIONES } CrustaCyclusLocus;
nomen enumeratio { CRUSTA_REPETITIO_TOK_APERTURA = 0, CRUSTA_REPETITIO_PROBATIO,
    CRUSTA_REPETITIO_CURSUS, CRUSTA_REPETITIO_REDIRECTIONES }
    CrustaRepetitioLocus;
nomen enumeratio { CRUSTA_CURSUS_TOK_APERTURA = 0, CRUSTA_CURSUS_LIBERI,
    CRUSTA_CURSUS_TOK_CLAUSURA } CrustaCursusLocus;
nomen enumeratio { CRUSTA_ELECTIO_TOK_APERTURA = 0, CRUSTA_ELECTIO_VERBUM,
    CRUSTA_ELECTIO_TOK_IN, CRUSTA_ELECTIO_LIBERI, CRUSTA_ELECTIO_TOK_CLAUSURA,
    CRUSTA_ELECTIO_REDIRECTIONES } CrustaElectioLocus;
nomen enumeratio { CRUSTA_OPTIO_TOK_PARENTHESIS = 0, CRUSTA_OPTIO_EXEMPLARIA,
    CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA, CRUSTA_OPTIO_LIBERI,
    CRUSTA_OPTIO_TOK_TERMINATOR } CrustaOptioLocus;
nomen enumeratio { CRUSTA_ARITHMETICA_TOK_APERTURA = 0, CRUSTA_ARITHMETICA_EXPRESSIO,
    CRUSTA_ARITHMETICA_TOK_CLAUSURA, CRUSTA_ARITHMETICA_REDIRECTIONES }
    CrustaArithmeticaLocus;
nomen enumeratio { CRUSTA_IUDICIUM_TOK_APERTURA = 0, CRUSTA_IUDICIUM_EXPRESSIO,
    CRUSTA_IUDICIUM_TOK_CLAUSURA, CRUSTA_IUDICIUM_REDIRECTIONES }
    CrustaIudiciumLocus;
nomen enumeratio { CRUSTA_SOCIUS_TOK_VERBUM = 0, CRUSTA_SOCIUS_TOK_TITULUS,
    CRUSTA_SOCIUS_IMPERIUM } CrustaSociusLocus;
nomen enumeratio { CRUSTA_VERBUM_PARTES = 0 } CrustaVerbumLocus;
nomen enumeratio { CRUSTA_PARS_TOK = 0 } CrustaParsLocus;
    /* pars-litteralis, -effugium, -continuatio, -simplex, -effugia,
     * -domus: locus unus 'tok' */
nomen enumeratio { CRUSTA_GEMINA_TOK_APERTURA = 0, CRUSTA_GEMINA_PARTES,
    CRUSTA_GEMINA_TOK_CLAUSURA } CrustaGeminaLocus;      /* et pars-versa */
nomen enumeratio { CRUSTA_PARAMETRUM_TOK_SIGILLUM = 0, CRUSTA_PARAMETRUM_TOK_TITULUS }
    CrustaParametrumLocus;
nomen enumeratio { CRUSTA_EXPANSIO_TOK_APERTURA = 0, CRUSTA_EXPANSIO_TOK_PRAEFIXUM,
    CRUSTA_EXPANSIO_TOK_TITULUS, CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM,
    CRUSTA_EXPANSIO_TOK_OPERATOR, CRUSTA_EXPANSIO_ARGUMENTA,
    CRUSTA_EXPANSIO_TOK_CLAUSURA } CrustaExpansioLocus;
nomen enumeratio { CRUSTA_SUBSTITUTIO_TOK_APERTURA = 0, CRUSTA_SUBSTITUTIO_LIBERI,
    CRUSTA_SUBSTITUTIO_TOK_CLAUSURA } CrustaSubstitutioLocus;  /* et pars-processus */
nomen enumeratio { CRUSTA_PARS_ARITHMETICA_TOK_APERTURA = 0,
    CRUSTA_PARS_ARITHMETICA_EXPRESSIO, CRUSTA_PARS_ARITHMETICA_TOK_CLAUSURA }
    CrustaParsArithmeticaLocus;
nomen enumeratio { CRUSTA_NUMERUS_TOK = 0 } CrustaNumerusLocus;
nomen enumeratio { CRUSTA_VARIABILIS_TOK_TITULUS = 0, CRUSTA_VARIABILIS_TOK_SUBSCRIPTUM }
    CrustaVariabilisLocus;
nomen enumeratio { CRUSTA_PRAEPOSITA_TOK_OPERATOR = 0, CRUSTA_PRAEPOSITA_OPERANDUM }
    CrustaPraepositaLocus;      /* et iudicium-praeposita */
nomen enumeratio { CRUSTA_POSTPOSITA_OPERANDUM = 0, CRUSTA_POSTPOSITA_TOK_OPERATOR }
    CrustaPostpositaLocus;
nomen enumeratio { CRUSTA_BINARIA_SINISTER = 0, CRUSTA_BINARIA_TOK_OPERATOR,
    CRUSTA_BINARIA_DEXTER } CrustaBinariaLocus;   /* et iudicium-binaria, -coniuncta */
nomen enumeratio { CRUSTA_TERNARIA_PROBATIO = 0, CRUSTA_TERNARIA_TOK_QUAESTIO,
    CRUSTA_TERNARIA_SINISTER, CRUSTA_TERNARIA_TOK_COLON, CRUSTA_TERNARIA_DEXTER }
    CrustaTernariaLocus;
nomen enumeratio { CRUSTA_INCLUSA_TOK_APERTURA = 0, CRUSTA_INCLUSA_EXPRESSIO,
    CRUSTA_INCLUSA_TOK_CLAUSURA } CrustaInclusaLocus;   /* et iudicium-inclusa */
```

  Genera that share a locus shape share the enum (a comment names the
  sharers); the registrum gate's named-slot table still lists every genus
  separately, so a declaration drift in any of them is caught.

  The dialect, same header (M7: tables, NIHIL-terminated):

```c
nomen enumeratio { CRUSTA_OPERATOR_BINARIUS = 0, CRUSTA_OPERATOR_PRAEPOSITUS,
    CRUSTA_OPERATOR_POSTPOSITUS, CRUSTA_OPERATOR_TERNARIUS } CrustaOperatorSpecies;
nomen structura {
    constans character* titulus;
                   i32  praecedentia;   /* maior = artius ligat */
                   b32  dextra;         /* associatio dextra */
    CrustaOperatorSpecies species;
} CrustaOperator;
nomen structura {
    constans character* constans* reservata;
    constans character* constans* aedificatores;
    constans character* constans* redirectiones;
    constans character* constans* optionum_termini;
    constans CrustaOperator*      arithmetici;        /* titulus NIHIL terminat */
    constans character* constans* iudicii_praeposita;
    constans character* constans* iudicii_binaria;
} CrustaDialectus;
externus constans CrustaDialectus CRUSTA_BASH;
```

  `CRUSTA_BASH` lives in `crusta_lexicon.c` (the one hand-written table
  file): reservata `if then else elif fi case esac for select while until
  do done in function time coproc { } [[ ]] !`; aedificatores `alias
  declare export local readonly typeset`; redirectiones `< > >> <& >& <>
  >| &> &>> << <<- <<<`; optionum_termini `;; ;& ;;&`; iudicii_praeposita
  `-a -b -c -d -e -f -g -h -k -p -r -s -t -u -w -x -G -L -N -O -S -z -n -o
  -v -R`; iudicii_binaria `-nt -ot -ef -eq -ne -lt -le -gt -ge == = != =~
  < >`; arithmetici in P4 (the table is declared here, filled there — an
  empty NIHIL-terminated table until then).

- [x] **Step 5: The lexicon (`crusta_lexicon.h` + `.c`)** — the enum
  `CrustaLexGenus` in the header, 50 members + sentinel, in THIS order
  (the rows follow it; the gate asserts by title):

```c
nomen enumeratio {
    CRUSTA_LEX_FINIS = 0,
    /* structura */
    CRUSTA_LEX_RESERVATUM, CRUSTA_LEX_ASSIGNATIO_TITULUS,
    CRUSTA_LEX_ASSIGNATIO_SUBSCRIPTUM, CRUSTA_LEX_ASSIGNATIO_OPERATOR,
    CRUSTA_LEX_REDIRECTIO_FD, CRUSTA_LEX_REDIRECTIO_OPERATOR,
    CRUSTA_LEX_PIPA, CRUSTA_LEX_PIPA_ERRORIS, CRUSTA_LEX_ET, CRUSTA_LEX_AUT,
    CRUSTA_LEX_SEPARATOR, CRUSTA_LEX_SEPARATOR_LINEAE,
    CRUSTA_LEX_TERMINATOR_OPTIONIS,
    CRUSTA_LEX_PARENTHESIS, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
    CRUSTA_LEX_ARITHMETICA_APERTURA, CRUSTA_LEX_ARITHMETICA_CLAUSURA,
    CRUSTA_LEX_IUDICIUM_APERTURA, CRUSTA_LEX_IUDICIUM_CLAUSURA,
    /* partes verbi */
    CRUSTA_LEX_LITTERALIS, CRUSTA_LEX_EFFUGIUM, CRUSTA_LEX_CONTINUATIO,
    CRUSTA_LEX_SIMPLEX, CRUSTA_LEX_GEMINA_APERTURA, CRUSTA_LEX_GEMINA_CLAUSURA,
    CRUSTA_LEX_EFFUGIA, CRUSTA_LEX_VERSA_APERTURA,
    CRUSTA_LEX_PARAMETRUM_SIGILLUM, CRUSTA_LEX_PARAMETRUM_TITULUS,
    CRUSTA_LEX_EXPANSIO_APERTURA, CRUSTA_LEX_EXPANSIO_PRAEFIXUM,
    CRUSTA_LEX_EXPANSIO_OPERATOR, CRUSTA_LEX_EXPANSIO_CLAUSURA,
    CRUSTA_LEX_SUBSTITUTIO_APERTURA, CRUSTA_LEX_GRAVIS,
    CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA, CRUSTA_LEX_PROCESSUS_APERTURA,
    CRUSTA_LEX_DOMUS,
    /* heredoc */
    CRUSTA_LEX_HEREDOC_DELIMITATOR,
    /* arithmetica */
    CRUSTA_LEX_NUMERUS, CRUSTA_LEX_VARIABILIS, CRUSTA_LEX_ARITHMETICA_OPERATOR,
    CRUSTA_LEX_ARITHMETICA_SEPARATOR,
    /* iudicium */
    CRUSTA_LEX_IUDICIUM_OPERATOR, CRUSTA_LEX_REGULA,
    /* trivia */
    CRUSTA_LEX_SPATIUM, CRUSTA_LEX_COMMENTUM, CRUSTA_LEX_LINEA, CRUSTA_LEX_LAMINA,
    CRUSTA_LEX_NUMERUS_GENERUM
} CrustaLexGenus;
```

  Rows (`GENERA_CRUSTA[]`, then `CRUSTA_LEXICON = { GENERA_CRUSTA, count,
  "crusta-", (s32)-I }` — no canonical space, SPATIUM is VERBATIM):
  FIXUM with orthography for `FINIS ""`, `PIPA "|"`, `PIPA_ERRORIS "|&"`,
  `ET "&&"`, `AUT "||"`, `PARENTHESIS "("`, `PARENTHESIS_CLAUSURA ")"`,
  `ARITHMETICA_APERTURA "(("`, `ARITHMETICA_CLAUSURA "))"`,
  `IUDICIUM_APERTURA "[["`, `IUDICIUM_CLAUSURA "]]"`,
  `GEMINA_APERTURA "\""`, `GEMINA_CLAUSURA "\""`, `VERSA_APERTURA "$\""`,
  `EXPANSIO_APERTURA "${"`, `EXPANSIO_CLAUSURA "}"`,
  `SUBSTITUTIO_APERTURA "$("`, `GRAVIS "`"`,
  `ARITHMETICA_PARTIS_APERTURA "$(("`, `ARITHMETICA_SEPARATOR ";"`;
  TERMINATOR `"\n"` for `SEPARATOR_LINEAE` (SUBSTANTIVUM) and `LINEA`
  (munus LINEA); VERBATIM for everything else — SUBSTANTIVUM except
  `SPATIUM` (munus SPATIUM), `COMMENTUM` (COMMENTUM), `LAMINA` (LAMINA).
  `SEPARATOR` (`;` or `&`), `REDIRECTIO_OPERATOR`, `TERMINATOR_OPTIONIS`,
  `PROCESSUS_APERTURA` (`<(` or `>(`) are VERBATIM because their bytes vary.
  Header comment: the C6 fact (two newline genera by mode), the C7 rule,
  the LINEA munus PRESENT (md's answer, not html's) and the FIDELITAS
  question left to the stml gate.

- [x] **Step 6: The runner** — copy `html/compile_probationes.sh` to
  `crusta/compile_probationes.sh`; rename every `html`/`HTML` (dir, log
  `build/test_logs/crusta.log`, banner `CRUSTA PROBATIONES: N/M
  praeteritae`, mensor prefix `crusta.`, env `CRUSTA_PROBATIONES_EFFUSIO`);
  `RADIX_FONTES` = `piscina chorda chorda_aedificator xar friatio
  tabula_dispersa internamentum stml stml_macros selectio similitudo canon
  credo processus_posix` (the processus library is `lib/processus_posix.c`,
  a POSIX file: it needs `postulata_posix.h` first, which it already has);
  DROP the md sub-fontes loop and the `-I…/md/fontes`
  include (crusta consumes materia only); keep the materia loop, the header
  guard, the sera lock, the adiumenta loop.

- [x] **Step 7: Write the failing registrum probatio** — transcribe
  `html/probationes/probatio_html_registrum.c`: `ORDO_EXSPECTATUS` = the 50
  lexicon titles in enum order; `GENERA_EXSPECTATA` = the 50 genus titles
  in declaration order; the recens gate over
  `"crusta/grammatica/crusta.registrum.stml"`; contiguity of `loci_offset`;
  `LOCI_NOMINATI[]` = 150 rows, every slot enum member above against the
  table's titles, count asserted equal to `numerus_locorum`; the munus
  facts: `SPATIUM`/`COMMENTUM`/`LINEA`/`LAMINA` are trivia,
  `SEPARATOR_LINEAE` is NOT, `materia_lexicon_munus_habet(LINEA)` VERUM;
  the dialect: every table NIHIL-terminated, no duplicate, `reservata`
  contains `"if"` and `"]]"`, `redirectiones` contains `"<<-"` and `"&>>"`,
  the arithmetici table is empty (P4 fills it and moves this assertion to
  a count); one minimal tree — `programma` with one `imperium` holding one
  `verbum` of one `pars-litteralis` `echo` — sized from the registry,
  through writer → reader → writer, byte-equal, `grammatica="crusta"` and
  `<lex-litteralis` present in the text.

- [x] **Step 8: Register in pythonica NOW** — `PORTAE['crusta'] =
  (['./crusta/compile_probationes.sh'], r'CRUSTA PROBATIONES: \d+/\d+')`,
  `FORMAE['crusta'] = 'suita'`, `SUITAE['crusta'] = ('crusta/probationes',
  'crusta/build/%s')` (the mensor `praef` dict and `metiri` wait for P10).
  A runner outside the gate table is a dead gate, and this commit gates on
  `crusta`.

- [x] **Step 9: Run and verify it fails, then passes** —
  `./crusta/compile_probationes.sh registrum`. Exit 2 = a path is wrong.

- [x] **Step 10: Plant** — `silva.planta('crusta/fontes/crusta_lexicon.c',
  <the SPATIUM row>, <the COMMENTUM row swapped into its place>, 'crusta',
  'registrum')` → red on the order-by-title assertion; reverted green.
  Second plant by hand: one byte of the generated `.c` → the recens gate
  names the line; revert.

- [x] **Step 11: `crusta/CLAUDE.md` stub** — what it is (one paragraph),
  spec and plan paths, the runner and its exit contract, "generated
  registry" paragraph (html's, retargeted), the two decrees by id.

- [x] **Step 12: Words, format, commit** — `./oratio/quaere.sh` on every
  new identifier word (`crusta lector situs regio petitio gradus …` as they
  appear); `./silva/formator.sh crusta/fontes/*.c crusta/fontes/*.h
  crusta/probationes/*.c -scribere` then `-vitia`; then

```python
silva.commissio("crusta: cliens materiae sextus natus - declaratio registri (L genera, CL loci), tabulae coctae, dialectus CRUSTA_BASH, lexicon (L genera lexematum, linea nova bis: separator et trivium), cursor, porta registri (rubra nata: ordo lexici)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/grammatica/crusta.registrum.stml",
     "crusta/fontes/crusta_registrum_coctum.h", "crusta/fontes/crusta_registrum_coctum.c",
     "crusta/fontes/crusta_registrum.h", "crusta/fontes/crusta_lexicon.h",
     "crusta/fontes/crusta_lexicon.c", "crusta/compile_probationes.sh",
     "crusta/probationes/probatio_crusta_registrum.c", "crusta/CLAUDE.md",
     "oratio/glossarium.stml", "pythonica/silva.py"], ["crusta", "pythonica"])
```

---

## Task P2: The lector — modes, regions, heredoc queue, positions

**Files:**
- Create: `crusta/fontes/crusta_lector.{h,c}`
- Create: `crusta/probationes/probatio_crusta_lector.c`

**Interfaces:**
- Consumes: `materia_token_creare(piscina, &forma, genus, valor, offset,
  linea, columna, fons_index)` (8 args, `md_lexema.c:1018`), `CrustaLexGenus`,
  `CrustaDialectus`.
- Produces (`crusta_lector.h`):

```c
nomen enumeratio {
    CRUSTA_MODUS_INITIUM = 0, CRUSTA_MODUS_VERBA, CRUSTA_MODUS_ASSIGNATIONES,
    CRUSTA_MODUS_POST_TITULUM, CRUSTA_MODUS_IN_VERBIS, CRUSTA_MODUS_EXEMPLAR,
    CRUSTA_MODUS_GEMINA, CRUSTA_MODUS_EXPANSIO, CRUSTA_MODUS_EXPANSIO_VERBUM,
    CRUSTA_MODUS_ARITHMETICA_SUMMA, CRUSTA_MODUS_ARITHMETICA_INTRA,
    CRUSTA_MODUS_IUDICIUM, CRUSTA_MODUS_REGULA, CRUSTA_MODUS_HEREDOC,
    CRUSTA_MODUS_HEREDOC_LITTERALE, CRUSTA_MODUS_NUMERUS_MODORUM
} CrustaModus;
nomen structura { s32 cursor; i32 linea; s32 linea_initium; } CrustaSitus;
nomen structura { s32 finis; i32 gravis; } CrustaRegio;
nomen structura { chorda delimitator; b32 citatus; b32 tabulae; }
    CrustaHeredocPetitio;
nomen structura {
                  Piscina* piscina;
       constans character* fons;
                      i32  mensura;
              CrustaSitus  situs;
                      Xar* regiones;        /* CrustaRegio per valorem */
                      Xar* heredoca;        /* CrustaHeredocPetitio, queue */
                      i32  heredoca_caput;  /* index proximae */
        MateriaTokenForma  forma;           /* cauda ZEPHYRUM */
    constans CrustaDialectus* dialectus;
} CrustaLector;

b32           crusta_lector_incipere (CrustaLector*, Piscina*, constans character* fons,
                                      i32 mensura, constans CrustaDialectus*);
MateriaToken* crusta_lector_proximum (CrustaLector*, CrustaModus);
              /* FINIS at the end of the source OR of the innermost region;
               * NIHIL = memoria deficit */
CrustaSitus   crusta_lector_situs (constans CrustaLector*);
vacuum        crusta_lector_situm_reponere (CrustaLector*, CrustaSitus);
b32           crusta_lector_regionem_aperire (CrustaLector*, s32 finis, i32 gravis);
vacuum        crusta_lector_regionem_claudere (CrustaLector*);
s32           crusta_lector_gravem_quaerere (constans CrustaLector*, s32 ab, i32 gravis);
              /* offset of the closing backtick at depth 'gravis' from 'ab';
               * -I when none (the region then runs to the source end) */
b32           crusta_lector_heredoc_petere (CrustaLector*, chorda delimitator,
                                            b32 citatus, b32 tabulae);
b32           crusta_lector_heredoca_pendent (constans CrustaLector*);
b32           crusta_lector_heredoc_aperire (CrustaLector*, CrustaHeredocPetitio* exitus);
              /* pops the queue head, finds the delimiter line, opens the
               * region [cursor, line start of the delimiter); FALSUM = empty */
MateriaToken* crusta_lector_heredoc_delimitator (CrustaLector*);
              /* after the region's FINIS: closes it; the delimiter line
               * token (tabs kept), NIHIL at EOF */
MateriaToken* crusta_lector_heredoc_finis (CrustaLector*);
              /* the newline after it, SEPARATOR_LINEAE; NIHIL at EOF */
```

- [x] **Step 1: Write the failing lector gate** — cases are `{fons, modi[],
  exspectata[]}`: the test pulls in the listed mode sequence and asserts
  each token's genus and bytes, then BYTE COVERAGE: the concatenation of
  every token's value (trivia are tokens here, nothing is bound yet) equals
  the source, FINIS last. Cases, at least:
  - `INITIUM`: `if true; then :; fi` → RESERVATUM, SPATIUM, LITTERALIS,
    SEPARATOR, SPATIUM, RESERVATUM, …; `echo if` in `INITIUM` then `VERBA`
    → LITTERALIS `if`; `x=if` → ASSIGNATIO_TITULUS `x`, ASSIGNATIO_OPERATOR
    `=`, LITTERALIS `if`; `a[1]+=v` → TITULUS, SUBSCRIPTUM `[1]`, OPERATOR
    `+=`; `A=1 if` in `INITIUM`,`INITIUM` → assignment then RESERVATUM.
  - newline by mode: `a\n` in `VERBA` → SEPARATOR_LINEAE; in `INITIUM` →
    LINEA; `\r\n` both ways (the TERMINATOR species takes the `cr` flag —
    assert the value is two bytes).
  - fd gluing: `2>f` → REDIRECTIO_FD `2`, REDIRECTIO_OPERATOR `>`; `2 >f` →
    LITTERALIS `2`, SPATIUM, OPERATOR; `x2>f` → LITTERALIS `x2`, OPERATOR;
    `{fd}>f` → REDIRECTIO_FD `{fd}`; `&>>f`, `<<<w`, `<<-E`, `>|f`, `<&-`.
  - parts: `"a$b${c}$(d)\$"` in `INITIUM`,`GEMINA`×n → GEMINA_APERTURA,
    LITTERALIS, PARAMETRUM_SIGILLUM+TITULUS, EXPANSIO_APERTURA…,
    SUBSTITUTIO_APERTURA…, EFFUGIUM `\$`, GEMINA_CLAUSURA; `'a b'` one
    SIMPLEX; `$'\t'` one EFFUGIA; `$"x"` VERSA_APERTURA; `~/x` DOMUS `~`
    then LITTERALIS `/x`; `a~b` one LITTERALIS; `$10` SIGILLUM+TITULUS `1`
    then LITTERALIS `0`; `$@ $* $# $? $- $$ $! $0`; a lone `$ ` LITTERALIS.
  - expansion operators in `EXPANSIO`: `${#x}` PRAEFIXUM `#`; `${#}` TITULUS
    `#`; `${!x[@]}`; `${x:-d}` OPERATOR `:-`; `${x//a/b}` OPERATOR `//`;
    `${x:1:2}` OPERATOR `:`; `${x@Q}` OPERATOR `@`; `${x^^}`.
  - continuation: `ab\<nl>cd` → LITTERALIS `ab`, CONTINUATIO, LITTERALIS
    `cd`; `a \<nl> b` → LITTERALIS, SPATIUM, LAMINA, SPATIUM, LITTERALIS.
  - `EXEMPLAR`: `(a|b) x` → PARENTHESIS, LITTERALIS, PIPA, LITTERALIS,
    PARENTHESIS_CLAUSURA; `IN_VERBIS`: `a b\n` → the newline is
    SEPARATOR_LINEAE; `POST_TITULUM`: `x\nin` → LITTERALIS, LINEA,
    RESERVATUM `in`.
  - arithmetic: `1 + 0x1f*(2)` in `ARITHMETICA_SUMMA` then `_INTRA` → NUMERUS,
    ARITHMETICA_OPERATOR `+`, NUMERUS `0x1f`, OPERATOR `*`, PARENTHESIS,
    NUMERUS, PARENTHESIS_CLAUSURA; `))` in SUMMA → one ARITHMETICA_CLAUSURA,
    in INTRA → two PARENTHESIS_CLAUSURA; `16#ff`, `a[i]++`, `<<=`, `**`,
    `? :`, `,`; `;` → ARITHMETICA_SEPARATOR.
  - `IUDICIUM`: `-f $x && ( a == b* ) ]]` → IUDICIUM_OPERATOR `-f`, parts,
    IUDICIUM_OPERATOR `&&`, PARENTHESIS, …, IUDICIUM_CLAUSURA; `REGULA`:
    `^a(b|c)+$ ]]` → REGULA `^a(b|c)+$` then SPATIUM, IUDICIUM_CLAUSURA.
  - regions: `crusta_lector_gravem_quaerere` on `` `a \`b\` c` `` at depth 0
    → the last backtick; at depth 1 from inside → the `` \` `` before `c`;
    pulling inside a depth-1 region reads `` \$x `` as SIGILLUM (2 bytes)
    + TITULUS; FINIS at the region end, then `regionem_claudere` and the
    next pull continues after it.
  - heredocs: `petere("A", FALSUM, FALSUM)` then source `x\nl1\n$v\nA\nrest`:
    `heredoc_aperire` → region; pulls in `HEREDOC` → LITTERALIS `l1\n`,
    SIGILLUM, TITULUS, LITTERALIS `\n`, FINIS; `heredoc_delimitator` → `A`;
    `heredoc_finis` → `\n`; next pull `rest`. Quoted delimiter → one
    LITTERALIS `l1\n$v\n` in `HEREDOC_LITTERALE`. `tabulae` VERUM matches
    `\t\tA` as the delimiter line, token keeps the tabs. Missing delimiter
    → body to EOF, `heredoc_delimitator` NIHIL, `heredoc_finis` NIHIL. Two
    petitions queue in order.
  - positions: every token's `linea`/`columna` against a hand table for a
    three-line source; `situs` / `situm_reponere` round trip re-lexes the
    same tokens.
  Self-measure: cases run ≥ 40, bytes covered printed.

- [x] **Step 2: Run and verify it fails** — `./crusta/compile_probationes.sh lector`.

- [x] **Step 3: Implement `crusta_lector.c`** — one `commutatio` on the mode
  choosing a scanner; every scanner returns a token from `_facere(lector,
  genus, ab, ad)` = `materia_token_creare(piscina, &forma, genus,
  {fons + ab, ad − ab}, ab, linea, columna, ZEPHYRUM)` and advances
  `situs` (counting newlines to keep `linea`/`linea_initium`). Shape:
  - `_trivium(l)`: at any word-mode entry, blanks → SPATIUM; `#` at a word
    start → COMMENTUM to line end (newline excluded); `\`+newline at a word
    boundary → LAMINA; newline → LINEA or SEPARATOR_LINEAE by mode table
    `MODI_LINEAM_TERMINANT[]` (VERBA, ASSIGNATIONES, IN_VERBIS: separator;
    the rest: trivium; HEREDOC modes: part of the literal).
  - `_reservatum(l)`: in INITIUM/POST_TITULUM/EXEMPLAR(`esac` only)/
    IUDICIUM(`]]`): scan the word; if it is exactly one unquoted run and
    matches the dialect table → RESERVATUM.
  - `_assignatio(l)`: INITIUM/ASSIGNATIONES: `name` `[…]`? `+`? `=` at word
    start → the three tokens (the operator ends the recognition; the value
    follows as ordinary parts).
  - `_redirectio(l)`: digits/`{name}` glued to `<`/`>` → FD; the operator
    set from the dialect (longest match: `<<<` before `<<-` before `<<`).
  - `_pars(l, modus)`: the part scanners — literal run (stops at blank,
    newline, `| & ; ( ) < >` in word modes; at `"` `'` `` ` `` `$` `\`
    `~`-at-start everywhere; at `}` in EXPANSIO_VERBUM; at `)`/`|` in
    EXEMPLAR), `\x` EFFUGIUM (in GEMINA only for `$ \` " \ newline`),
    `'…'` SIMPLEX (to the next `'`, or EOF: the token is then imperfect
    but total), `"` GEMINA_APERTURA/CLAUSURA, `$'…'` EFFUGIA (ANSI-C
    escapes skip the closing quote correctly: `$'\''`), `$"` VERSA, `$name`
    / `$digit` / `$special` SIGILLUM+TITULUS, `${` EXPANSIO_APERTURA, `$((`
    ARITHMETICA_PARTIS_APERTURA (before `$(`), `$(` SUBSTITUTIO_APERTURA,
    `` ` `` GRAVIS, `<(`/`>(` PROCESSUS_APERTURA, `~` DOMUS at word start
    (through the next `/` or blank; `~user` included).
  - `_expansio(l)`: after `${`: `#`/`!` PRAEFIXUM when a name follows else
    TITULUS; the name/digits/special; `[…]` SUBSCRIPTUM (to the matching
    `]`, raw); the operator (longest match from the table in §3; `*` and
    `@` after `!name`); `}` EXPANSIO_CLAUSURA.
  - `_arithmetica(l, intra)`: numbers (`0x…`, `0…`, `base#…`, decimal),
    names with optional raw `[…]`, the operator set (longest match: `<<=`
    `>>=` `**` `++` `--` `&&` `||` `<=` `>=` `==` `!=` `+=` … then
    singles), `(` `)`, `))` as ONE token only in SUMMA, `;` SEPARATOR, `?`
    `:` `,`, and the `$` parts as in words.
  - `_iudicium(l)`: `&&` `||` `!` `(` `)` `<` `>` `=` `==` `!=` `=~` and the
    `-x` tables → IUDICIUM_OPERATOR; `]]` IUDICIUM_CLAUSURA; else parts.
  - `_regula(l)`: one REGULA token: bytes until an unquoted blank, `&&`,
    `||` or `]]`, quotes and parens tracked (quoted portions are separate
    parts: SIMPLEX/GEMINA tokens interleaved — the builder makes them one
    `verbum`).
  - regions: `_finis(l)` = the innermost region's `finis` or `mensura`;
    every scanner stops there; `gravis` depth: `_gravis_octetus(l, i)` reads
    `\`×d + (`` ` `` | `\` | `$`) as that byte and reports the consumed
    width so tokens keep the real bytes.
  - the heredoc reader: `heredoc_aperire` scans line by line from the
    cursor comparing each line (tabs stripped when `tabulae`) with the
    delimiter; the region ends at that line's start; the modes HEREDOC /
    HEREDOC_LITTERALE lex inside it (a literal never crosses a line end).

- [x] **Step 4: Run and verify** — green, coverage printed.

- [x] **Step 5: Plant** — in `_trivium`, the LAMINA branch consumes the
  backslash but not the newline → coverage red by one byte; revert.

- [x] **Step 6: Words, format, commit**

```python
silva.commissio("crusta: lector - functio (modus, positio), modi XV, regiones (heredoc, backtick cum profunditate), queue heredoc, glutinatio fd, partes verbi; porta lectoris (tegumentum octetorum per modos; rubra nata: lamina dimidiata)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_lector.h", "crusta/fontes/crusta_lector.c",
     "crusta/probationes/probatio_crusta_lector.c"], ["crusta"])
```

---

## Task P3: The builder, part one — program, simple commands, words and parts, redirections, lists, mala

**Files:**
- Create: `crusta/fontes/crusta_arbor.{h,c}`
- Create: `crusta/probationes/probatio_crusta_arbor.c`

**Interfaces:**
- Consumes: P2's lector; `materia_nodus_creare/ponere/appendere`,
  `materia_valor_token/nodus/lista_nova`, `materia_scribere_nodum`,
  `materia_token_trivia_ante_ponere/_post_ponere`.
- Produces (`crusta_arbor.h`):

```c
nomen structura {
    i32 mala;                 /* nodi malum */
    i32 clausurae_absentes;   /* constructa ab EOF clausa */
    i32 profunditas_maxima;   /* acervus framarum */
    i32 heredoca;             /* corpora lecta */
    b32 sana;                 /* mala == 0 && clausurae_absentes == 0 */
} CrustaParsura;

MateriaNodus* crusta_arbor_parsare (Piscina*, constans character* fons, i32 mensura,
                                    constans CrustaDialectus*, CrustaParsura* relatio);
                                    /* relatio NIHIL licet */
b32    crusta_verbum_staticum (Piscina*, constans MateriaNodus* verbum, chorda* valor);
       /* decoded value when no parametrum/expansio/substitutio/arithmetica/
        * processus part is present; FALSUM = not static */
b32    crusta_verbum_citatum (constans MateriaNodus* verbum);   /* any quoted or escaped part */
constans MateriaNodus* crusta_imperium_titulus (constans MateriaNodus* imperium);  /* first verbum, NIHIL if none */
Xar*   crusta_imperium_argumenta (Piscina*, constans MateriaNodus* imperium);      /* verba after it */
```

Internal shape (in `crusta_arbor.c`):

```c
nomen structura {
    MateriaNodus* nodus;
             s32  genus;
             i32  locus;        /* locus liberos recipiens */
      CrustaModus modus;        /* modus petendi proximi */
             i32  status;       /* per genus: verbum visum, inclusa profunditas … */
             s32  verbi_finis;  /* offset post partem ultimam (adiacentia) */
} CrustaGradus;
nomen structura { Xar* pendentia; MateriaToken* prior; } CrustaLigator;
nomen structura {
    Piscina* piscina; CrustaLector lector; Xar* gradus; CrustaLigator ligator;
    constans CrustaDialectus* dialectus; CrustaParsura* relatio;
    Xar* redirectiones_heredoc;   /* redirectio nodes waiting for a body */
} CrustaParsatura;
```

- [ ] **Step 1: Write the failing arbor gate** — every case is a source
  string allocated from the piscina, parsed, emitted through
  `materia_scribere_nodum`, `memcmp` against the source, plus structural
  assertions through helpers `_liberi(nodus, locus)`, `_genus(nodus)`,
  `_tok(nodus, locus)`. Cases for this task:
  - `echo hi\n` → programma.liberi = [imperium(verbum echo, verbum hi),
    separator `\n`]; cauda FINIS; `crusta_imperium_titulus` = `echo`,
    argumenta = [hi]. Empty source → liberi empty, cauda present.
  - words: `"a"b'c'` one verbum of three parts; `a b` two verba; `x=1 y=2 cmd
    a=b` → assignatio, assignatio, verbum, verbum (positional: `a=b` is a
    word); `A=1 if` → assignatio then a verbum `if`?? NO: `if` in INITIUM
    after an assignment is a RESERVATUM the grammar cannot place → `malum`
    (bash: syntax error) — assert one malum, `relatio.sana` FALSUM;
    `declare -a x=(1 2)` → aedificator: `x=(1 2)` is an assignatio with a
    tabulatum valor; `a=` valor absent; `a+=b`; `a[i]=v` subscriptum.
  - parts: `$x ${y:-d} $(echo z) \$ ~/p $'\t' $"s"` each genus; `${x/a/b}`
    argumenta = [verbum a, operator /, verbum b]; `${x:1:2}`; `${#x}`;
    nested `"$(echo "in")"`; backticks `` `echo \`echo d\`` `` → substitutio
    (GRAVIS open/close) containing a substitutio whose open token is the two
    bytes `` \` ``; `a$(b)c` ONE verbum of three parts; `a $(b)` two verba.
  - redirections: `cmd 2>&1 >/dev/null <in &>all {fd}>x` → imperium.liberi
    in byte order, fd tokens where present; `> f` alone → imperium with no
    verbum and one redirectio; `<<<w` scopus verbum; `<<A` here: the
    redirectio is built and the petition queued, the body handled in P5 —
    this task's case ends at the operator's line: `cat <<A` + EOF → body
    empty, heredoc node with no delimiter (P5 asserts it; here only that
    the redirectio exists and `relatio.heredoca == 1`).
  - lists: `a; b & c\n` → imperium, separator `;`, imperium, separator `&`,
    imperium, separator `\n`; `a && b || c` → catena[pipa? no: imperium,
    operator &&, imperium, operator ||, imperium]; `a | b |& c` → pipa[…];
    `! a | b` → pipa.praefixa [!]; `time -p a` → praefixa [time, -p];
    `a &&\n  b` → the newline is trivia POST of `&&` (assert
    `numerus_post == 2`: blank, LINEA) and `  ` ANTE of `b`.
  - trivia: `a # c\n` → separator with ante [` `, `# c`]; `#!/bin/bash\n\na`
    → all ante of `a`; `a # c` + EOF → cauda's FINIS carries them; `a \<nl>
    b` → LAMINA post of `a`.
  - mala: `fi`, `)`, `;;`, `}`, `; ;`, `then` alone → one malum each, bytes
    kept (emission equal), `relatio.mala` counted; `if` alone → NOT this
    task (P5), omit.
  - EOF: `"abc` → gemina with tok_clausura absent; `$(a` → substitutio with
    tok_clausura absent; `relatio.clausurae_absentes == 1` each.
  - `crusta_verbum_staticum`: `'a b'` → `a b`; `"a\"b"` → `a"b`; `a\ b` →
    `a b`; `$'\t'` → TAB; `$x` → FALSUM; `crusta_verbum_citatum` on each.
  - the absent-slot rows of spec §4 that belong to these genera (`a=`,
    `>f`, `${x}`, `${x:-}`).
  `ARITHMETICA_PARTIS_APERTURA` (`$((`) is routed to a `malum` UNTIL P4 (a
  named intermediate; no case here uses it). `[[`, `((`, compound keywords
  and heredoc bodies are P4/P5.

- [ ] **Step 2: Run and verify it fails** — `./crusta/compile_probationes.sh arbor`.

- [ ] **Step 3: Implement** — the loop:

```c
    dum (VERUM)
    {
        CrustaGradus* g = _vertex(p);
        MateriaToken* t = crusta_lector_proximum(&p->lector, g->modus);
        si (t == NIHIL) redde NIHIL;
        si (materia_lexicon_trivium_est(p->lexicon, t->genus))
        {   _ligator_cumulare(p, t);  perge;  }
        _ligator_solvere(p, t);                /* C7 division */
        si (t->genus == CRUSTA_LEX_FINIS) { si (!_finem_tractare(p, t)) frange; perge; }
        _tractare(p, g, t);                     /* (frame genus, token genus) */
    }
```

  `_ligator_solvere`: pending trivia divided AFTER the last one whose
  genus is LINEA or LAMINA: `[0, k]` → `post` of `prior` (when `prior` is
  NIHIL — the file start — everything goes `ante`), `(k, n)` → `ante` of
  `sequens`; no newline → all `ante`. `_finem_tractare`: inside a region
  (heredoc/backtick) pops to the region's frame and returns VERUM; at the
  source end closes every frame with absent tokens (counting
  `clausurae_absentes` for genera with a `tok_clausura`), puts FINIS in
  `programma.cauda`, returns FALSUM. `_tractare` dispatches: word parts →
  `_partem_recipere` (a new `verbum` unless the top frame is a verbum whose
  `verbi_finis == t->byte_offset`; `pars-gemina`/`-versa`/`-expansio`/
  `-substitutio`/`-processus` push frames with their modes; `EXPANSIO`
  after the name pushes an `EXPANSIO_VERBUM` frame for each argument word);
  `RESERVATUM` in a list frame → this task: `malum` (P5 replaces with the
  constructs; keep the dispatch table `RESERVATA_TRACTATIO[]` keyed by the
  reserved word so P5 fills rows, not branches); separators → `_sententiam_claudere`
  + `separator` node; `ET`/`AUT` → `_catenam_curare`; `PIPA`/`PIPA_ERRORIS`
  → `_pipam_curare`; `RESERVATUM` `!`/`time` in INITIUM → open a `pipa`,
  praefixa; redirect fd/operator → `redirectio` frame (scopus = the next
  verbum; for `<<`/`<<-`, when the scopus verbum closes, `crusta_verbum_staticum`
  + `crusta_verbum_citatum` → `crusta_lector_heredoc_petere`, and the
  redirectio is remembered in `redirectiones_heredoc`); an `imperium` is
  opened by the first assignment/word/redirect in a list frame; its mode is
  INITIUM until a verbum is complete, then VERBA or ASSIGNATIONES.
  The mala: `_malum(p, t)` appends a `malum` with the token to the innermost
  list frame (or extends the previous malum when adjacent).

- [ ] **Step 4: Run and verify** — green.

- [ ] **Step 5: Plant** — in `_ligator_solvere`, divide after the FIRST
  newline instead of the last → the `a &&\n\n  b` case red (post count);
  revert.

- [ ] **Step 6: Words, format, commit**

```python
silva.commissio("crusta: aedificator I - programma, imperia simplicia (assignationes positionales, verba ut partes, redirectiones), catena/pipa cum operatore solo, separatores, mala totalitatis, ligator regulae inversae (trivia retro per lineam novam ultimam); porta arboris (rubra nata: divisio post lineam primam)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_arbor.h", "crusta/fontes/crusta_arbor.c",
     "crusta/probationes/probatio_crusta_arbor.c"], ["crusta"])
```

---

## Task P4: Arithmetic — the precedence machine, the evaluator, `$(( ))` and `(( ))`

**Files:**
- Create: `crusta/fontes/crusta_arithmetica.{h,c}`
- Create: `crusta/probationes/probatio_crusta_arithmetica.c`
- Modify: `crusta/fontes/crusta_lexicon.c` (fill `arithmetici`), `crusta/fontes/crusta_arbor.c` (arithmetic frames), `crusta/probationes/probatio_crusta_registrum.c` (the arithmetici count: 41 rows)

**Interfaces:**
- Produces (`crusta_arithmetica.h`):

```c
nomen structura {
    Piscina* piscina; constans CrustaDialectus* dialectus;
    Xar* operanda;      /* MateriaNodus* */
    Xar* operatores;    /* MateriaToken* with their CrustaOperator */
    b32  operandum_exspectatur;
    i32  mala;
} CrustaArithmetica;
vacuum        crusta_arithmetica_incipere (CrustaArithmetica*, Piscina*, constans CrustaDialectus*);
b32           crusta_arithmetica_operandum (CrustaArithmetica*, MateriaNodus* nodus);
b32           crusta_arithmetica_operator (CrustaArithmetica*, MateriaToken* tok);
MateriaNodus* crusta_arithmetica_finire (CrustaArithmetica*);   /* NIHIL = expressio vacua */
b32           crusta_arithmetica_aestimare (constans MateriaNodus* expressio, s64* valor);
constans CrustaOperator* crusta_arithmetica_quaerere (constans CrustaDialectus*, chorda titulus,
                                                      CrustaOperatorSpecies species);
```

- [ ] **Step 1: The operator table** (`CRUSTA_BASH.arithmetici`, measured on
  bash 5.2 — unary binds TIGHTER than `**`: `-2**2` is 4; `**` is right):

| praecedentia | operators | species | dextra |
|---|---|---|---|
| 16 | `++` `--` (after an operand) | POSTPOSITUS | — |
| 15 | `++` `--` `!` `~` `+` `-` (before an operand) | PRAEPOSITUS | VERUM |
| 14 | `**` | BINARIUS | VERUM |
| 13 | `*` `/` `%` | BINARIUS | |
| 12 | `+` `-` | BINARIUS | |
| 11 | `<<` `>>` | BINARIUS | |
| 10 | `<` `<=` `>` `>=` | BINARIUS | |
| 9 | `==` `!=` | BINARIUS | |
| 8 | `&` | | |
| 7 | `^` | | |
| 6 | `\|` | | |
| 5 | `&&` | | |
| 4 | `\|\|` | | |
| 3 | `?` `:` | TERNARIUS | VERUM |
| 2 | `=` `*=` `/=` `%=` `+=` `-=` `<<=` `>>=` `&=` `^=` `\|=` | BINARIUS | VERUM |
| 1 | `,` | BINARIUS | |

- [ ] **Step 2: Write the failing arithmetica gate** — (a) structure through
  `crusta_arbor_parsare` on `$(( … ))` sources: `1+2*3` → binaria(+, 1,
  binaria(*, 2, 3)); `2**3**2` right-nested; `-2**2` → binaria(**,
  praeposita(-, 2), 2); `a++ + ++b`; `x = y += 2` right; `1,2`; `c ? a :
  b`; `(1+2)*3` inclusa; `$x + ${y} + $(cmd)` parts as leaves; `a[i+1]`
  variabilis with subscript; empty `$(( ))` → expressio absent; `(( i +=
  1 ))` as a command → `arithmetica` genus; `$((1+1))` versus
  `$((echo a); b)` → the second is a substitutio whose first child is a
  crustula (the backtrack); every case byte-exact through the emitter;
  (b) the constant evaluator against bash 5.2's values (computed 2026-09-16
  with `/opt/homebrew/bin/bash -c 'echo $((e))'`, embedded as a table):

```c
hic_manens constans struct { constans character* fons; s64 valor; } AESTIMANDA[] = {
    { "1+2*3", 7 }, { "(1+2)*3", 9 }, { "2**3**2", 512 }, { "-2**2", 4 },
    { "10/3", 3 }, { "10%3", 1 }, { "-7/2", -3 }, { "-7%2", -1 },
    { "1<<4", 16 }, { "256>>2", 64 }, { "5&3", 1 }, { "5|3", 7 }, { "5^3", 6 },
    { "~5", -6 }, { "!0", 1 }, { "!5", 0 }, { "1&&0", 0 }, { "1||0", 1 },
    { "0||0&&1", 0 }, { "1?2:3", 2 }, { "0?2:3", 3 }, { "1?0?4:5:6", 5 },
    { "0x1f", 31 }, { "010", 8 }, { "2#1010", 10 }, { "16#ff", 255 },
    { "36#zz", 1295 }, { "1,2,3", 3 }, { "3>2", 1 }, { "2>=3", 0 },
    { "1==1", 1 }, { "1!=1", 0 }, { "-(3)", -3 }, { "+4", 4 },
    { "9223372036854775807+1", (s64)-9223372036854775807LL - 1 },
    { "-9223372036854775808-1", 9223372036854775807LL },
    { "1+2<<1", 6 }, { "6/2*3", 9 }, { "2*3%4", 2 }, { "7-3-2", 2 },
    { "1<2==1", 1 }, { "(2)", 2 }, { "- - 3", 3 }
};
```

  (`struct` is `structura` in the house — write it so.) Plus refusals:
  `1/0`, `1%0`, `2**-1` → `aestimare` FALSUM; `$x+1` → FALSUM (not
  constant). Self-measure: rows evaluated == 43.

- [ ] **Step 3: Run and verify it fails.**

- [ ] **Step 4: Implement** — the machine: `operandum` pushes (and, when
  the previous entry was a POSTPOSITUS candidate, nothing special: postfix
  is decided at `operator` time by `operandum_exspectatur == FALSUM`);
  `operator` looks the token up by species (`operandum_exspectatur` ? PRAEPOSITUS
  : (`++`/`--` → POSTPOSITUS, else BINARIUS/TERNARIUS)), reduces the stack
  while the top has higher precedence (or equal and not right-assoc), then
  pushes; `?` pushes a marker; `:` reduces to the marker and replaces it
  with a `:` entry; reducing `:` pops dexter, sinister and probatio into a
  `ternaria`; `finire` reduces everything; a dangling operator (`1 +` then
  `))`) yields a `binaria` with `dexter` absent and `mala++` (totality).
  In the builder: `$((` and `((` push an `expressio` frame carrying a
  `CrustaArithmetica`; NUMERUS → `numerus`, VARIABILIS → `variabilis`
  (subscript token when present), parts → their frames (their node becomes
  an operand when they pop), `(` → an `inclusa` frame (mode INTRA), `)` in
  INTRA closes it (its node is an operand of the parent machine),
  `ARITHMETICA_CLAUSURA` in SUMMA → `finire`, place the expression, close
  the frame; `$((` that reaches FINIS or a non-arithmetic token without
  `))` → `crusta_lector_situm_reponere` to the `$(` and re-dispatch as
  SUBSTITUTIO_APERTURA + PARENTHESIS (the `_situs` was saved at the `$((`).
  The evaluator: a recursive walk over the seven arithmetic genera (depth is
  the expression's nesting, bounded by the totality gate's measurement in
  P9), 64-bit wrapping arithmetic via `i64`/`s64` with explicit casts,
  division/modulo by zero and negative exponents refused.

- [ ] **Step 5: Run and verify** — green; then the whole suite.

- [ ] **Step 6: Plant** — swap the precedence of `*` and `+` in the table →
  `1+2*3` red; revert.

- [ ] **Step 7: Words, format, commit**

```python
silva.commissio("crusta: arithmetica - machina praecedentiae (shunting-yard, XVI gradus bash: praeposita artius quam **), aestimator constans contra bash 5.2 (XLIII valores), $(( )) et (( )) in aedificatore, ambiguitas $(( per reversionem cursoris; porta arithmeticae (rubra nata: praecedentia * et + permutata)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_arithmetica.h", "crusta/fontes/crusta_arithmetica.c",
     "crusta/fontes/crusta_lexicon.c", "crusta/fontes/crusta_arbor.c",
     "crusta/probationes/probatio_crusta_arithmetica.c",
     "crusta/probationes/probatio_crusta_registrum.c"], ["crusta"])
```

---

## Task P5: The builder, part two — compound commands, functions, `[[ ]]`, heredocs

**Files:**
- Modify: `crusta/fontes/crusta_arbor.c`, `crusta/probationes/probatio_crusta_arbor.c`

**Interfaces:** none new; `RESERVATA_TRACTATIO[]` rows filled; the heredoc
placement (decree 01M2NJ16RG) implemented.

- [ ] **Step 1: Add the failing cases** to the arbor gate:
  - `if a; then b; elif c; then d; else e; fi` → conditio(probatio [imperium,
    separator], tok_deinde, liberi, rami [ramus elif, ramus else], tok_clausura);
    `if a\nthen\n b\nfi` (newlines: the one after `a` is a separator in
    probatio, the one after `then` is trivia); `if a; then { b; fi` → grex
    with tok_clausura absent, conditio closed by `fi`, `clausurae_absentes
    == 1`; `if` at EOF → everything absent.
  - `for i in a b; do c; done`, `for i in a b\ndo c; done` (tok_separator
    = the newline), `for i; do :; done`, `for i do :; done` (tok_in and
    tok_separator absent), `for ((i=0;i<3;i++)); do :; done` → cyclus with
    liberi [expressio, operator ;, expressio, operator ;, expressio],
    `for ((;;))` three absent; `select x in a; do :; done` → iteratio with
    tok_apertura `select`; `while :; do break; done`, `until a; do b; done`.
  - `case $x in a|b) echo 1 ;; (c) ;; d) esac` → electio with optio ×3:
    exemplaria [verbum, operator |, verbum]; the second with tok_parenthesis;
    the third with tok_terminator absent; `;&` and `;;&` terminators; a
    pattern with `$v` parts; `case x in\n  a)\n b\n ;;\nesac` newlines as
    trivia after `in` and `)`.
  - `{ a; b; } > f` → grex with redirectiones [redirectio]; `( a ) 2>&1`;
    `{ a }` (no separator before `}`: bash error → `}` is a word here, the
    grex closes at EOF with tok_clausura absent — assert exactly that,
    `bash -n` will agree it is invalid).
  - `f() { :; }`, `function g { :; }`, `function h () ( : )`, `f() > x`
    (no body: corpus absent, malum? bash: syntax error near `>` — the
    redirectio has no command: corpus absent, the redirectio becomes an
    imperium after the functio; assert bytes only and `sana` FALSUM).
  - `[[ $a == b* && -f $c || ! ( x =~ ^a(b|c)$ ) ]]` → iudicium whose
    expressio is iudicium-coniuncta(||, iudicium-coniuncta(&&, binaria(==),
    praeposita(-f)), praeposita(!, inclusa(binaria(=~, verbum x, verbum
    [REGULA part])))); `[[ -n $x ]] && y`; `[[ a ]]` bare verbum; `[[` at
    EOF.
  - `coproc c { :; }`, `coproc cmd`, `time ls |& cat`.
  - heredocs (decree I): `cat <<A\nl1\n$v\nA\nrest\n` → programma.liberi =
    [imperium(cat, redirectio(<<, scopus A, corpus → #heredoc)),
    separator \n, heredoc(partes [litteralis, parametrum, litteralis],
    tok_delimitator A, tok_finis \n), imperium(rest), separator];
    `cat <<'A'` one literal part; `cat <<-A\n\tx\n\tA\n` tabs kept;
    `cat <<A <<B | wc\n1\nA\n2\nB\n` → pipa.liberi = [imperium, operator |
    (post: ` `, `\n`), heredoc A, heredoc B, imperium wc] — the two bodies
    after the OPERATOR node; `cat <<A &&\nx\nA\ny` → catena.liberi order;
    `echo $(cat <<A\nx\nA\n)` → heredoc inside the substitutio's liberi;
    `echo $(cat <<A)\nx\nA\n` → heredoc in the OUTER list; `cat <<A` + EOF
    → heredoc with partes empty, both tokens absent; a `<<` whose line ends
    inside `"…"` → the body starts after the closing quote's line; every
    case byte-exact and `relatio.heredoca` counted.
  - the remaining absent-slot rows of spec §4.

- [ ] **Step 2: Run and verify it fails.**

- [ ] **Step 3: Implement** — `RESERVATA_TRACTATIO[]` rows: `if` → conditio
  frame (locus probatio, mode INITIUM), `then` → the nearest conditio/ramus
  frame with probatio open: tok_deinde, locus liberi (a `then` elsewhere →
  malum), `elif`/`else` → close the conditio's liberi, push a ramus into
  rami, `fi` → pop to the nearest conditio (frames above close absent),
  `for`/`select` → iteratio frame (mode POST_TITULUM; a `((` there converts
  the frame to a cyclus), `in` → tok_in + mode IN_VERBIS (iteratio) or
  liberi + mode EXEMPLAR (electio), `do` → cursus under the nearest
  iteratio/cyclus/repetitio, `done` → pop to the cursus, then the loop
  frame takes redirections, `while`/`until` → repetitio, `case` → electio
  (mode POST_TITULUM for the word, then `in`), `esac` → pop to the electio,
  `{` → grex, `}` → pop to the nearest grex (only when `}` is in command
  position — the lexer already decided), `function` → functio frame,
  `time`/`!` → pipa praefixa, `coproc` → socius, `[[` → iudicium frame
  (mode IUDICIUM, its own small precedence: `!` > `&&` > `||`, parens as
  frames; after `=~` one pull in REGULA), `((` → arithmetica frame (P4's
  machine). In EXEMPLAR mode: PARENTHESIS opens an optio, parts make
  exemplaria verba, PIPA between them is an operator node,
  PARENTHESIS_CLAUSURA closes the pattern and the optio takes liberi in
  INITIUM; TERMINATOR_OPTIONIS closes the optio. A `name()` in INITIUM
  (a verbum immediately followed by `(` `)`) converts the imperium into a
  functio (`_imperium_in_functionem`): allowed because the imperium has
  exactly one verbum of one literal part and no assignments. After a
  compound's closing token the frame stays for redirections until the
  next non-redirect token. Heredoc placement: in the loop, when the token
  is SEPARATOR_LINEAE or a LINEA trivium and `crusta_lector_heredoca_pendent`,
  first solve the ligator (so the newline lands `post`/on the separator),
  then for each pending redirectio: `heredoc_aperire`, push a `heredoc`
  frame (mode HEREDOC or HEREDOC_LITTERALE by `citatus`) whose node is
  appended to the innermost LIST frame's locus immediately (the list's
  last child is the separator or the operator node that holds the
  newline), pull parts until FINIS, `heredoc_delimitator` + `heredoc_finis`
  into the node, `materia_nodus_ponere(redirectio, CORPUS,
  materia_valor_referentia(heredoc), MATERIA_LOCUS_REFERENTIA)`.

- [ ] **Step 4: Run and verify** — green; whole suite.

- [ ] **Step 5: Plant** — append the heredoc node to the imperium frame
  instead of the list frame → `cat <<A <<B | wc` emits the bodies before
  ` | wc` → byte red; revert.

- [ ] **Step 6: Words, format, commit**

```python
silva.commissio("crusta: aedificator II - imperia composita (conditio/rami, iteratio/cyclus/repetitio/cursus, electio/optiones, grex, crustula, functio, socius), iudicium [[ ]] cum regula, praefixa pipae, corpora heredoc ubi octeti iacent cum referentia (decretum 01M2NJ16RG); porta arboris aucta (rubra nata: corpus sub imperio)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_arbor.c", "crusta/probationes/probatio_crusta_arbor.c"], ["crusta"])
```

---

## Task P6: Cases and corpus — the reader, the pathology fixture, the adversarial fixture, FreeBSD (ASK), the corpus gate

**Files:**
- Create: `crusta/fontes/crusta_exempla.{h,c}`
- Create: `probationes/fixa/crusta/pathologiae.sh`, `probationes/fixa/crusta/adversarius.sh`
- Create (after Fran's yes): `probationes/fixa/crusta/freebsd/*` + `probationes/fixa/crusta/FONTES.md`
- Create: `crusta/probationes/probatio_crusta_exempla.c`, `crusta/probationes/probatio_crusta_corpus.c`

**Interfaces:**

```c
nomen structura { chorda titulus; chorda textus; } CrustaSectio;
nomen structura {
    i32 numerus;      /* I-basatus */
    i32 linea;        /* linea '####' */
    chorda titulus;   /* post '#### ', detonsa */
    chorda datum;     /* octeti inter lineam tituli et sectionem primam / casum proximum */
    Xar* sectiones;   /* CrustaSectio per valorem */
} CrustaExemplum;
Xar*   crusta_exempla_legere (Piscina*, constans character* fons, i32 mensura);
chorda crusta_exemplum_sectio (constans CrustaExemplum*, constans character* titulus);
       /* datum NIHIL si absens */
```

Format (the Oils `spec/` shape, so an Oils file reads unchanged): a line
`#### <titulus>` opens a case; lines before the first `####` are preamble;
the datum is every byte from the line after the title up to the line
start of the first `## ` line or the next `####`; a `## NOMEN: textus` line
with non-blank text after the colon is a one-line section; `## NOMEN:`
alone opens a multi-line section that ends at `## END` (exclusive). The
reader works on bytes (CRLF cases keep their `\r`).

- [ ] **Step 1: ASK Fran** before any fetch: "vendor FreeBSD
  `bin/sh/tests/parser/` (BSD-2) into `probationes/fixa/crusta/freebsd/`?"
  On yes, in the scratchpad: `git clone --depth 1 --filter=blob:none
  --sparse https://github.com/freebsd/freebsd-src freebsd && cd freebsd &&
  git sparse-checkout set bin/sh/tests/parser`; copy `bin/sh/tests/parser/*`
  (the numbered scripts `name.0`, `name.1`…; skip the `Makefile`); record
  the commit hash, file count and byte count in `FONTES.md` with the BSD-2
  text from the repo's `COPYRIGHT`. On no, the corpus is the house scripts
  plus the two fixtures and this task's pins change accordingly.

- [ ] **Step 2: Write `pathologiae.sh`** — one `#### title` block per
  snippet of spec Appendix A.9 (each with its own title: `positio-verba`,
  `heredoc-bini`, `substitutio-commentum`, `arith-subcrusta`,
  `sublinguae`, `fd-glutinatio`, `gravis-nidus`, `extglob`, `alias`), plus:
  `processus-substitutio` (`diff <(a) >(b)`), `pipa-erroris` (`a |& b`),
  `redirectio-ambo` (`a &> f; b &>> g`), `fd-variabilis` (`exec {fd}>f`),
  `optio-termini` (`case x in a) ;& b) ;;& c) ;; esac`), `heredoc-tabulae`
  (`cat <<-E` with tabs), `herestring`, `regula` (`[[ a =~ ^a(b|c)$ ]]`),
  `socius`, `electio-select`, `tempus`, `cyclus`, `tabulata` (`a=(1 [2]=x)
  b+=(y)`), `versa` (`$"x"`), `crlf` (written with real `\r\n`),
  `arith-ambiguum` (`$((1+1))` and `$((echo a); b)`), `heredoc-in-
  substitutione` (both placements), `continuatio` (`ab\`newline`cd`),
  `functio-formae` (the three), `grex-sine-separatore` (`{ a }`),
  `si-apertum` (`if true; then` — invalid), `esac-solum`, `parenthesis-
  vaga`. Every block's datum ends with `\n`.

- [ ] **Step 3: Write `adversarius.sh`** — CRLF THROUGHOUT (written with
  Python so the `\r\n` are real), unclosed `"`, `'`, `$(`, `${`, `((`,
  `[[`, `{`, `if`, `case`, a heredoc without delimiter at the end, `\` at
  EOF, `#` comment at EOF without newline, `$` at EOF, nesting `$( … )`
  200 deep, `a=(` unclosed, `;;` outside case, the literal strings
  `</lex-litteralis>` and `</crusta-litteralis>` inside a word (the raw-form
  refusal case). No NUL (rides inline in P9).

- [ ] **Step 4: Write the failing exempla gate** — read `pathologiae.sh`
  (count pinned: the number of blocks written), every FreeBSD file (count
  and total bytes pinned from `FONTES.md`), a synthetic file with the
  one-line and multi-line section forms, a CRLF case; assert titles, data
  bytes, section lookup, `crusta_exemplum_sectio` NIHIL for an absent name.

- [ ] **Step 5: Write the failing corpus gate** — `CORPUS[]` = every tracked
  `.sh` of the repo (the list is built at test time: the gate reads
  `RHUBARB_RADIX/build/crusta_corpus.lst`, which the RUNNER writes with
  `git ls-files '*.sh'` before the loop — add that line to
  `compile_probationes.sh`; a missing list = `CREDO_CULPA`), the two
  fixtures, and every FreeBSD file: parse, `materia_scribere_nodum`,
  `memcmp`; tally causes (`idem / NIHIL / emissio fracta / dispar`), print
  the first divergent offset and the file; per file also the lector byte
  coverage through the builder's own pulls (the parser records every
  token it received; the gate concatenates values + trivia and compares —
  this is the corpus-wide coverage P2 could not do). Self-measure:
  `plagulae_lectae ≥ 230`, `octeti_probati > 800 000`; house-corpus mala
  printed per file (pinned in P11, reported here).

- [ ] **Step 6: Run, read failure #1 first** — one cause per fix; a
  substrate defect gets its own materia gate and commit first.

- [ ] **Step 7: Fix until green.**

- [ ] **Step 8: Plant** — corpus: drop every `separator` node in the builder
  → every file with `;` short → red; exempla: the section reader keeps the
  `## END` line → bytes red; revert both.

- [ ] **Step 9: Words, format, commit**

```python
silva.commissio("crusta: lector casuum (forma Oils '#### titulus' / '## SECTIO:'), fixa pathologiarum (appendix A.IX + additamenta) et adversarii (CRLF, aperta omnia, nidus CC), casus parser FreeBSD vendati (BSD-2, FONTES.md), porta exemplorum et porta corporis (plagulae domus CCXXX + fixa + FreeBSD octetim exactae, tegumentum lectoris per corpus; rubrae natae)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_exempla.h", "crusta/fontes/crusta_exempla.c",
     "probationes/fixa/crusta/pathologiae.sh", "probationes/fixa/crusta/adversarius.sh",
     "probationes/fixa/crusta/FONTES.md"]
    + sorted("probationes/fixa/crusta/freebsd/" + n
             for n in os.listdir("probationes/fixa/crusta/freebsd"))
    + ["crusta/compile_probationes.sh",
     "crusta/probationes/probatio_crusta_exempla.c",
     "crusta/probationes/probatio_crusta_corpus.c"], ["crusta"])
```

---

## Task P7: STML projection — two cycles, the comparator, references carried

**Files:**
- Create: `crusta/probationes/probatio_crusta_stml.c`

**Interfaces (verified against `materia_arbor.h` 2026-09-15):**

```c
    MateriaLexiconRatum ratum; MateriaLexIudicium iudicium;
    MateriaArborConsilium consilium; MateriaArborVitium vitium;
    materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON, &iudicium);
    materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM, &ratum, "crusta");
    MateriaArborScriptura s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    MateriaNodus* r = materia_arbor_legere(piscina, intern, s.textus, &consilium, &vitium);
    b32 materia_arbor_aequalis(a, b, MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d);
```

- [ ] **Step 1: Write the failing gate** — inline cases (every P3/P4/P5
  case string) AND the whole P6 corpus: `s1 = scribere(radix)`; `r1 =
  legere(s1)`; `s2 = scribere(r1)`; `CREDO_CHORDAE_AEQUALES(s1, s2)`;
  `CREDO_VERUM(aequalis(radix, r1, STRUCTURALIS))`; then the re-read
  tree's direct emission equals the source; then FIDELITAS: try
  `MATERIA_ARBOR_COMPARATIO_FIDELIS` on one case — if the comparator
  accepts it (the LINEA munus exists), assert it on every case; if it
  refuses, pin the refusal by its named cause and say so in the header
  comment (spec §10's question, answered by measurement). References: for
  every heredoc case assert the projected text contains `<corpus(> #nod`
  and `id="nod`, and that the re-read `redirectio`'s `corpus` value is a
  REFERENTIA whose target is a `heredoc` node. Refusals: the two
  `</lex-…>` strings of `adversarius.sh` — print `s.causa`; a refusal for
  any cause other than the raw-form limit (01M2KPJ0HW) is red. Self-measure:
  documents round-tripped == corpus files + inline count.

- [ ] **Step 2: Run and verify it fails** — `./crusta/compile_probationes.sh stml`.

- [ ] **Step 3: Fix what the refusals name** — causes, never assertions.

- [ ] **Step 4: Run the whole suite** — exit 0.

- [ ] **Step 5: Plant** — before the comparator,
  `materia_nodus_ponere(r1, CRUSTA_PROGRAMMA_CAUDA, VALOR_NIHIL, TOKEN)` on
  the re-read tree → tree oracle red while `s1`/`s2` stay equal (the
  oracle-separation pin); revert.

- [ ] **Step 6: Commit**

```python
silva.commissio("crusta: circuitus STML - duo cycli octetim idem, comparator STRUCTURALIS (FIDELITAS mensurata), referentiae heredoc relatae, emissio relectae idem, corpus totum (rubra nata: cauda relectae sublata)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/probationes/probatio_crusta_stml.c"], ["crusta"])
```

---

## Task P8: `crusta.canon` — hand-written, drift-guarded, seal pinned

**Files:**
- Create: `crusta/grammatica/crusta.canon`
- Create: `crusta/probationes/probatio_crusta_canon.c`

Model on `html/grammatica/html.canon` (read its header first): the
`<arbor>` envelope with `grammatica="crusta"` + `registrum-sigillum`
pinned; fragments `<#lexema-unum>` / `<#lexemata-multa>` over the 50
`crusta-*` tags (tag mangling: `_` → `-`, lower case — check one against
`s1.textus` from P7); one rule per genus constraining loci at SPECIES level
(G3); the list union `<#sententia>` (the seventeen list-item genera) used by
every `liberi`/`probatio` rule; `<ante>`/`<post>` allow `crusta-spatium`,
`crusta-commentum`, `crusta-linea`, `crusta-lamina` only; `id` declared on
`heredoc` (the `corpus` target) — and on nothing else, so a stray `id` is
a refusal. NOT in `canones.registrum`.

- [ ] **Step 1: Write the failing gate** — `canon_legere(chorda fons,
  piscina, intern, &causa)`; for every P7 document (inline + corpus):
  `stml_legere` the projected text, `canon_iudicare(canon, radix,
  piscina)`, assert non-NIHIL and `xar_numerus(vitia) == 0`; self-measure
  `documenta_iudicata > 0`. The drift guard: every `CrustaGenus` title has
  exactly one rule and the count is `CRUSTA_GENUS_NUMERUS_GENERUM`; every
  locus title appears under its genus's rule; every lexicon title has its
  `crusta-*` in both fragments; every genus rule in the canon names a live
  genus (reverse); the pinned `registrum-sigillum` equals
  `materia_arbor_sigillum(piscina, &CRUSTA_REGISTRUM)` live (the gate
  prints the live seal so the pin can be written).

- [ ] **Step 2: Run and verify it fails** — no canon yet.

- [ ] **Step 3: Write the canon**; `./tools/natura_struere.sh`.

- [ ] **Step 4: Run the whole suite** — exit 0.

- [ ] **Step 5: Plant** — delete the `verbum` genus rule → the guard AND the
  judgment must both fire; revert. Second plant: a false sigillum → guard red.

- [ ] **Step 6: Commit**

```python
silva.commissio("crusta: canon manu scriptum (L genera, loci ad gradum speciei, lexemata L, unio sententiae, id in heredoc solo) - custos derivae utrimque, sigillum pinnatum, corpus iudicatum (rubra nata: regula verbi deleta)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/grammatica/crusta.canon", "crusta/probationes/probatio_crusta_canon.c"], ["crusta"])
```

---

## Task P9: The reservation gate and the totality gate

**Files:**
- Create: `crusta/probationes/probatio_crusta_reservatio.c`
- Create: `crusta/probationes/probatio_crusta_totalitas.c`

- [ ] **Step 1: Reservatio — write the failing gate** — the deviation table
  of spec §5, each row measured: `shopt -s extglob; echo @(a|b)` → exactly
  the mala the parser produces today (count pinned, printed; a verbum `@`,
  a `crustula` holding `a|b`, and the mala for what does not fit);
  `alias x='if true; then'; x echo hi; fi` → `x` parses as a command name,
  `fi` is a malum (count 1); `eval 'if'` → the argument is one verbum, no
  malum; `set -o posix` and `shopt -s anything` → plain commands; `$"x"` →
  a `pars-versa` whose static value is `x` (never translated); `$'\x41'` →
  `crusta_verbum_staticum` gives `A`. Also the reader side (html §11.6's
  measured fact): a projected document with `<derivatum/>` under `<verbum>`
  → `materia_arbor_legere` NIHIL, `vitium.causa` `"locus generi ignotus"`;
  `derivatum="x"` as an attribute on `<verbum>` → the reader accepts, the
  canon refuses with ≥ 1 vitium naming it. Every message by `strcmp`.

- [ ] **Step 2: Totalitas — write the failing gate** — transcribe
  `html/probationes/probatio_html_totalitas.c` (LCG random bytes, mutation
  1/40, 23 truncation steps, nesting forms) over the crusta corpus with
  `crusta_arbor_parsare` + `materia_scribere_nodum` under `CREDO_NON_RUIT`;
  failing source written to `crusta/build/totalitas_fractum.sh`. Depth:
  parse ALONE of `$(` × 100 000 (open, and open+closed), `((` × 100 000,
  `"` unclosed after `$(` × 100 000, `{ ` × 100 000, `if a; then ` × 100 000
  → `CREDO_NON_RUIT` (the builder is iterative); then parse + emit at
  100 000 → measure: if `materia_scribere_nodum` survives, `CREDO_NON_RUIT`;
  if it dies, `CREDO_RUIT_CUM(…, SIGSEGV)` with the depth named and
  `01M1FAD8` cited; the STML write at the same depth, same rule; the
  arithmetic evaluator on `((((` × 100 000 (P4's recursive walker),
  same rule. NUL: an inline source with a NUL byte inside a word and one
  inside a heredoc body parses and emits byte-exact; the STML writer's
  refusal is a NAMED refusal, not a crash. CRLF: `adversarius.sh` whole
  and every pathology case with `\n` → `\r\n`.

- [ ] **Step 3: Run both and verify they fail.**

- [ ] **Step 4: Implement nothing unless red names a defect** — a builder
  crash found here is fixed in `crusta_arbor.c` with the failing source
  added as a case in the arbor gate.

- [ ] **Step 5: Plant** — reservatio: the extglob malum count edited → red.
  Totalitas: replace the `inclusa` frame push with a recursive call for
  one construct → the `((((` pin red; revert both.

- [ ] **Step 6: Commit**

```python
silva.commissio("crusta: porta reservationis (deviationes nominatae mensuratae: extglob, alias, eval, posix, $\"\", lector elementa ignota RECUSAT attributa IGNORAT) + porta totalitatis (fortuiti, mutati, truncati, CRLF, NUL, nidificatio C milium sine recursione; scriptor/proiectio/aestimator ad profunditatem mensurati, 01M1FAD8)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/probationes/probatio_crusta_reservatio.c",
     "crusta/probationes/probatio_crusta_totalitas.c"], ["crusta"])
```

---

## Task P10: Computus twin, instruments, pythonica

**Files:**
- Create: `crusta/fontes/crusta_computus.{h,c}`, `crusta/instrumenta/computus.c`,
  `crusta/instrumenta/arbor.c`, `crusta/computus.sh`, `crusta/arbor.sh`
- Create: `crusta/probationes/probatio_crusta_computus.c`,
  `crusta/probationes/fixa/computus/basis.tsv`
- Modify: `pythonica/silva.py` (the mensor `praef` dict, `metiri`'s
  client-by-suffix `.sh` → `crusta/computus.sh`), `crusta/CLAUDE.md`

- [ ] **Step 1: Write the failing computus gate** — transcribe
  `html/probationes/probatio_html_computus.c`: corpus = `pathologiae.sh`,
  `adversarius.sh`, and three house scripts by path (`silva/compile_probationes.sh`,
  `tools/vexilla.sh`, `html/compile_probationes.sh`), the same deterministic
  columns, golden `basis.tsv`, `COMPUTUS_SCRIBERE=1` regenerates (a named
  cause in the commit).

- [ ] **Step 2: Run and verify it fails** — no `crusta_computus` yet.

- [ ] **Step 3: Implement** — `crusta_computus_metiri(fons, mensura,
  &exitus)` = html's twin (`CrustaComputus` with the same fields; parse in
  its own piscina; emit; STML write / read / compare in a second piscina;
  `clock()` ms). `crusta/computus.sh`, `crusta/arbor.sh <x.sh> [-tacitus]`
  from html's scripts (`html` → `crusta`, `.html` → `.sh` in the usage
  lines); `crusta/instrumenta/arbor.c` = html's with `crusta_arbor_parsare(piscina,
  textus, mensura, &CRUSTA_BASH, NIHIL)`. Generate the golden once, read
  it, commit it.

- [ ] **Step 4: Register in pythonica** — the mensor `praef` dict gains
  `'crusta': 'crusta.'`; `metiri`: `.sh` → `crusta/computus.sh`. Then
  `silva.porta('crusta')` from Python runs the suite and parses the banner;
  `./pythonica/probare.sh` stays green.

- [ ] **Step 5: Plant** — edit one `nodi` count in `basis.tsv` → red; revert.

- [ ] **Step 6: `crusta/CLAUDE.md`** — the "Currere" section with the three
  scripts, the gate list so far.

- [ ] **Step 7: Commit**

```python
silva.commissio("crusta: computus gemellus (aurum fixa/computus/basis.tsv), instrumenta computus.sh/arbor.sh, registratio pythonica (praef mensoris, metiri .sh) (rubra nata: aurum mutatum)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_computus.h", "crusta/fontes/crusta_computus.c",
     "crusta/instrumenta/computus.c", "crusta/instrumenta/arbor.c",
     "crusta/computus.sh", "crusta/arbor.sh",
     "crusta/probationes/probatio_crusta_computus.c",
     "crusta/probationes/fixa/computus/basis.tsv", "pythonica/silva.py",
     "crusta/CLAUDE.md"], ["crusta", "pythonica"])
```

---

## Task P11: The cooked view, the oracle instrument, the goldens, the oraculum and differentia gates

**Files:**
- Create: `crusta/fontes/crusta_coctum.{h,c}`, `crusta/instrumenta/oraculum.c`, `crusta/oraculum.sh`, `crusta/coctum.sh`, `crusta/instrumenta/coctum.c`
- Create: `probationes/fixa/crusta/oraculum/expectata.txt`, `probationes/fixa/crusta/oraculum/sanitas.txt`
- Create: `crusta/probationes/probatio_crusta_coctum.c`, `probatio_crusta_oraculum.c`, `probatio_crusta_differentia.c`

**Interfaces:**

```c
chorda crusta_coctum_scribere (Piscina*, constans MateriaNodus* radix);
       /* bash's declare -f normal form of the whole program; datum NIHIL
        * = memoria; ends with '\n' when non-empty */
```

The instrument (`oraculum.c`, driven by `crusta/oraculum.sh`):
`-scribere <casus.sh>…` runs `/opt/homebrew/bin/bash` per case via
`processus_exsequi` with argv `{"/opt/homebrew/bin/bash", "-c", <script>,
NIHIL}`: for `expectata.txt` the script is `f() {\n` + datum + `}\ndeclare
-f f\n` and the golden block is `#### <fons>:<titulus>\n## EXSPECTATUM:\n`
+ stdout + `## END\n` (a bash failure = no block, the case counts as
"bash rejects"); for `sanitas.txt` the argv is `{"bash", "-n", "-c",
datum, NIHIL}` and the block is `#### key\n## status: N\n## ERRATUM: <first
stderr line>\n`. The header of both goldens records the `bash --version`
first line and the date; `-scribere` refuses a bash whose version line
lacks `5.2`. `-probare` reruns and diffs. `-domus <plagulae>…` runs the
house scripts live: parse, cook, compare with `declare -f` (wrapped the
same way); `bash -n` verdict versus `relatio.sana`; prints counts per
class and the first ten disagreements. `crusta/coctum.sh <x.sh>` prints
the normal form of a file.

- [ ] **Step 1: Write the failing coctum gate** — inline cases with the
  expected text embedded (measured on 5.2.15, spec §7): `if true; then :;
  fi; a | b && c || d; ! x` inside the `f` wrapper → the exact block of
  spec §7's first probe; the redirect reorder (`2>&1 A=1 cmd x >o B=2 y` →
  `A=1 cmd x B=2 y 2>&1 > o`); `|&` → `2>&1 |`; `h() ( : )` → the
  `function h () \n{ \n    ( : )\n}` form; the `for … in …;\ndo` and
  `case … in \n` forms with an empty item; the heredoc inline form; `[[ ]]`
  with single spaces; arithmetic verbatim; comments and blank lines gone.
  Self-measure: cases ≥ 12.

- [ ] **Step 2: Run and verify it fails; implement `crusta_coctum.c`** — a
  printer over the tree with an indent level and a `chorda_aedificator`,
  one function per genus, the rules of spec §7 as the first cut; every
  later discrepancy is a rule here.

- [ ] **Step 3: The instrument** — `oraculum.c` as above; `crusta/oraculum.sh`
  in `arbor.sh`'s shape passing `-scribere`/`-probare`/`-domus` through
  (`-domus` with no files = `$(git ls-files '*.sh')`). Generate both
  goldens over `pathologiae.sh` and the FreeBSD files; read the headers.

- [ ] **Step 4: Write the failing oraculum and differentia gates** —
  oraculum: for every case of `pathologiae.sh` + FreeBSD with a golden
  block: parse the wrapped text, cook, compare; count `pares / dispares /
  sine auro`; `PINNA_PRAETERITA` = the count at birth (printed first, then
  pinned), rising only; `ORACULUM_OMNIA=1` prints every failure's diff
  (first differing line), `ORACULUM_EXEMPLUM=<key>` one case in full.
  differentia: for every case with a `sanitas.txt` block: bash's `status
  == 0` versus `relatio.sana`; agreement pinned rising; the disagreements
  listed by key with both verdicts; plus the house-corpus pin: parse every
  file of `build/crusta_corpus.lst`, sum `relatio.mala` — pinned at the
  birth value, FALLING only, and zero is the goal.

- [ ] **Step 5: Run, read the failure classes, fix the cooked view** (never
  the parser unless the parser is wrong — a parser fix goes to the arbor
  gate first) until the numbers stop rising cheaply; pin.

- [ ] **Step 6: Plant** — oraculum: the pin raised above the count → red;
  differentia: `sana` inverted for the zero-mala class → red; coctum: the
  redirect reorder disabled → red; revert all.

- [ ] **Step 7: `crusta/CLAUDE.md`** — the oracle numbers and how to move
  them; the instrument's three modes.

- [ ] **Step 8: Commit**

```python
silva.commissio("crusta: visio cocta (forma normalis 'declare -f' bash 5.2), instrumentum oraculi (-scribere/-probare/-domus per processus), aura expectata.txt et sanitas.txt, portae coctum/oraculum/differentia pinnatae ascendentes, mala corporis domus pinnata descendentia (rubrae natae)\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/fontes/crusta_coctum.h", "crusta/fontes/crusta_coctum.c",
     "crusta/instrumenta/oraculum.c", "crusta/instrumenta/coctum.c",
     "crusta/oraculum.sh", "crusta/coctum.sh",
     "probationes/fixa/crusta/oraculum/expectata.txt",
     "probationes/fixa/crusta/oraculum/sanitas.txt",
     "crusta/probationes/probatio_crusta_coctum.c",
     "crusta/probationes/probatio_crusta_oraculum.c",
     "crusta/probationes/probatio_crusta_differentia.c",
     "crusta/CLAUDE.md"], ["crusta"])
```

---

## Task P12: Closure — records, audit, ledger, memory

**Files:**
- Modify: `crusta/CLAUDE.md` (full orientation: built, gates, numbers, laws, the two decrees, what remains), `project-specs/crusta-arbor-spec.md` (§11 "As built", the numbers), `project-specs/crusta-arbor-plan.md` (STATUS: CLOSED), `crusta/fontes/crusta_arbor.worklog.md` (created at the first find-time during P3–P11; closed here), `materia/phase-log.md` (INTENTIO written before P1; RELATIO + instrument debrief here)
- Run: `./materia/shim_probare.sh` (the phase-boundary audit; count in the phase-log)

- [ ] **Step 1: `crusta/CLAUDE.md`** in html's shape: Praesens status
  (built, the 14 gates and what each asserts, the oracle numbers and pins),
  Laws (C5–C8, C12 in one line each, with the decree ids), "The registry is
  generated", Birth lessons (whatever P1–P11 taught), Currere.

- [ ] **Step 2: Spec §11 "As built"** — the deltas from §§2–8 (anything the
  build changed), the measured answers to §10, the oracle tail classes.

- [ ] **Step 3: Phase-log RELATIO** — numbers (files, lines, gates, pins,
  house-corpus mala, oracle agreement, FIDELITAS answer, depth pins), the
  instrument debrief (what the tools did and refused), the substrate
  changes (expected: none) and the shim audit count.

- [ ] **Step 4: Ledger** — desideratum 01M2MC9M29 → status `impletum` with
  a closing note (numbers, commits); notes on the two decrees (as built);
  new items for anything parked (the oracle tail classes as ONE desideratum
  with the class list; any substrate finding as a quaestio).

- [ ] **Step 5: Memory** — `bash-parser-project.md` rewritten as the crusta
  topic file (name, numbers, pins, the reflexes: newline by mode, trivia
  backward, heredoc after the newline's holder, `-domus` for the house
  corpus, the oracle binary), `MEMORY.md` line updated; `materia-fork.md`
  gains the sixth client's line.

- [ ] **Step 6: Commit**

```python
silva.commissio("crusta: clausura plani - CLAUDE.md, spec XI ut aedificatum, phase-log RELATIO + debrief, worklog, planum CLAUSUM; audit shim materiae\n\nCo-Authored-By: Claude Fable 5.1 <noreply@anthropic.com>",
    ["crusta/CLAUDE.md", "project-specs/crusta-arbor-spec.md",
     "project-specs/crusta-arbor-plan.md", "crusta/fontes/crusta_arbor.worklog.md",
     "materia/phase-log.md"], [])
```

---

## Self-Review (2026-09-16, before execution)

**Spec coverage.** §2 files → P1/P2/P3/P4/P6/P10/P11. §3 lexer (modes,
regions, heredoc queue, token genera, trivia rule, fd gluing, parameter
forms) → P2 (the rule's binding is P3's ligator). §4 declaration, absent-slot
table → P1, P3, P5. §5 builder (frames, positional rules, terminators, EOF,
heredoc decree, `$((`, regions, deviations, dialect) → P3/P4/P5/P9. §6
arithmetic + evaluator, `[[ ]]` → P4/P5. §7 cooked view, both oracles, the
case reader, the three case sources → P6/P11. §8 fourteen gates → P1
registrum, P2 lector, P3 arbor, P6 exempla + corpus, P7 stml, P8 canon, P9
reservatio + totalitas, P10 computus, P4 arithmetica, P11 coctum +
oraculum + differentia = 14. §9 futures: nothing to build; the accessors
(`crusta_verbum_staticum`, `crusta_imperium_titulus/argumenta`) are P3.
§10 substrate questions: measured in P7 (references forward, FIDELITAS),
P3 (trivia backward), P9 (depth). Appendix A.9 → P6's fixture.

**Placeholder scan.** No TBD/TODO. Two named intermediates are explicit:
P3 routes `$((` to a malum until P4; P1 declares an empty arithmetici table
until P4 (its gate assertion moves from "empty" to "41 rows" in P4).

**Type consistency.** `CrustaParsura` (P3) used by P5/P9/P11 with the same
fields; `crusta_arbor_parsare` has five parameters everywhere;
`crusta_lector_heredoc_petere(lector, delimitator, citatus, tabulae)` in P2
and P3; `CrustaArithmetica` API in P4 only; `crusta_exempla_legere` /
`crusta_exemplum_sectio` in P6 and P11; `crusta_coctum_scribere` in P11;
slot enums as in P1 throughout (shared enums: `CrustaParsLocus` for the six
single-token parts, `CrustaGeminaLocus` for versa, `CrustaSubstitutioLocus`
for processus, `CrustaPraepositaLocus`/`CrustaBinariaLocus`/`CrustaInclusaLocus`
for the iudicium genera).
