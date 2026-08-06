# censor.c — worklog

## 2026-07-10 — natus (instrumenta prima, tool 1 of 5)

Design paid by instrumenta-simulatio-1.md (C1 + addendum A1/A2) and
-2.md (DECISUS block). Shipped same day as designed; corpus contact
below.

**Mechanics (both "no hand list" pledges kept):**
- Slot inventory is RUNTIME-derived from SILVA_C89_REGISTRUM: any
  genus slot with species TOKEN named "tok_titulus" is a
  name-binding position. New grammar genera are covered
  automatically — the AEDIFICATA lesson (hand count beside a table
  = two sources of truth) applied at design time. "tok_valor"
  (expression leaves) and "tok_destinatio" (goto use) are
  DELIBERATELY excluded: expression-position latina (imprimere,
  NIHIL, roman numerals) is the language's design, not a landmine.
- The latina reserved set is derived per-TU from the expander's
  macro journal (silva_macro_vista): macros whose defining fons
  basename is "latina.h". No word list anywhere in the tool. Files
  that never include latina.h (knotapel English C) get an empty set
  and skip the walk for free.
- Walker is a BFS worklist (grow-only xar + cursor) — no recursion,
  no depth cap, no silent truncation (percursus's recursive
  counters cap at 64/128; a lint cannot afford that).
- Origo chain walked through ALL layers (EXPANSIO/PASTA/CHORDA/API)
  so a user macro that expands to a latina word still fires.

**Two firing classes (ledger audit finding):** of the ~10
historical firings only `exire` (#4, tessera) was class A — a
SILENT rename (expansion is a valid identifier). All others
expanded to keywords/numbers = class B, loud but cryptic parse
errors. So: class A rows ([CENSURA], exit 1) walk tok_titulus
slots; class B rows ([suspectum in errore], hint only) list the
DISTINCT latina macros inside each ERROR node — the culprit jumps
out of the list ('integer', 'nomen', 'redde' → nomen). List cap
VIII with explicit ", ..." marker (no silent caps).

**Exceptions (genus-scoped, exactly two):**
- principale → main, DECLARATOR_TITULUS only (every program binds it)
- FILUM → FILE, TYPUS_NOMINATUS only (deliberate type-name use;
  `int FILUM;` still fires because that's declarator position)

**FINDING — the git-blob validation corpus does not exist.** Sim-1
assumed "git history has every landmine as a before/after pair."
False: every firing (including exire — tessera has 2 commits) was
caught and fixed WITHIN a session, before commit. The validation
corpus is therefore SYNTHETIC: probationes/fixa/censoris/
- flagrationes.c — class A, 13 blessed rows (2026-07-10, every row
  read): tag-def/tag-ref×2/campus/accessus×2 (exire),
  typedef-name + label (imprimere), enumerator + param in prototype
  AND definition (liberare), locals (NIHIL, memoriae_allocare).
  Position discrimination proven in-file: NIHIL flags as declarator
  (l.44), stays silent as expression (l.51).
- fragores.c.fragor — class B, 3 suspecta rows naming
  nomen/registrum/magnitudo/structura/commutatio(post-cap)/externus.
  ".fragor" suffix keeps percursus and censor -corpus away from the
  deliberate parse errors (baselines must not move).

**Corpus contact (first full sweep, same day):** 762 plagulae, 528
cum latina, **0 flagrationes, 0 suspecta — FP rate 0.00 on first
contact.** The two sealed exceptions were exactly sufficient. Cost:
~90s whole repo, ~150-190ms/file + one capita_praeparare per
invocation.

**Gates (DECISUS: day one):** both silva/amalgamare.sh and
officina/amalgamare.sh gained porta 6 CENSURA — silva's runs the
regression probe (must exit 1 with exactly 13 [CENSURA] rows) then
sweeps silva fontes+instrumenta+probationes (~77 files, ~12s);
officina's sweeps its own (~25 files, ~4s). VERIFICATUM line now
says "+ censura". censoris/ is a named -corpus exclusion (the
tool must not flag its own regression corpus; the gate feeds it
explicitly and demands exit 1).

**Learned on contact:** tag REFERENCES fire alongside definitions
(3 rows for one bad struct tag — footprint reporting, useful);
prototype params fire separately from definition params. Both
correct; both in the blessed count.

**Named deferrals:** standing full-corpus sweep (FP rate now
measured at 0 — promotion is Fran's call); class-B culprit ranking
(v0 lists distinct macros; smarter position heuristics only if
field data asks); knotapel-side header praebere collisions
(basename-first-wins, percursus exemplar — revisit if a wrong-header
FP ever appears).

## 2026-07-13 — soft-tier verdict gap (macro-index chunk 1)

Fresh code used `nomen` as a local variable (THE canonical forbidden
word, firing ~#12). Censor's output listed 'nomen' among "macra
latinae intra fracturam parsationis" — the signal WAS there — but
flagrationes stayed 0 and conditio said PURUS, because the
identifier-position check cannot see inside broken parse regions
(where a real firing lands by definition: the macro expanded and
wrecked the parse). Reader sees PURUS and moves on; the diagnosis
came from clang + the legatus diagnostics push instead.

Improvement candidate (not built): when the [suspectum in errore]
list inside a broken region contains one of the HIGH-RISK words
(nomen/registrum/magnitudo/exire/structura/casus/externus — the
historical offender list), escalate conditio to SUSPECTUM instead of
PURUS. The suspecta count already exists; only the verdict line
underplays it. Park with a path back: censor.c verdict logic.

## 2026-08-06 — omnes unci PostToolUse taciti erant (schema JSON)

Inventum dum uncus novus (.genera, natura-custos.sh) probaretur:
**nullus uncus PostToolUse huius domus quicquam in contextum
iniciebat, et nemo id sciebat.**

CAUSA: forma exitus. Unci PostToolUse involucrum nidificatum
poscunt -

    {"hookSpecificOutput":{"hookEventName":"PostToolUse",
                           "additionalContext":"..."}}

- sed censor/examen/excubitor omnes clavem NUDAM emittebant,
`{"additionalContext":"..."}`, quae TACITE abicitur. Nulla
querela, nullus error, nihil in actis: uncus currebat, iudicium
rectum ferebat, et verbum eius in vacuum cadebat.

QUOMODO INVENTUM SIT, et cur aliter inventum non esset: uncum
novum probaturus notavi **'uncus tacet' et 'uncus non currit'
idem prorsus spectari**. Ergo probatio vera non est plagula sana
(silentium utroque modo) sed VITIUM CONSULTO PLANTATUM - cogere
portam ut LOQUATUR. Duabus editionibus factis nihil venit;
uncus tamen sanus stabat cum manu vocaretur. Discrimen
angustatum est uncum PreToolUse (generata-custos) probando, qui
editionem RECUSAT - signum binarium quod celari non potest. Ille
flagravit: ergo unci currebant, et vitium in forma exitus latebat.
Ille solus formam nidificatam adhibebat.

LEX QUAM HOC DOCET: **porta silens et porta mortua idem spectari
- ergo nulla porta credenda est donec eam LOQUI COEGERIS.**
Disciplina 'cursus purus = nihil in contextum' recta manet, sed
pretium eius est quod defectum PERFECTE occultat. Quapropter
uncus quisque novus vitio plantato probandus est, semel, in ipsa
nativitate.

Quattuor plagulae emendatae (censor I, examen III, excubitor I,
natura IV sedes). Probatum VIVE: vitio in natura/versio.genera
plantato, editio subsequens nuntium in contextum vere intulit.

QUAMDIU TACUERINT IGNOTUM. Logica uncorum ipsa recta erat toto
tempore (censor [CENSURA] recte quaerit, exitum I recte legit) -
quod ipsum peius facit: instrumenta bona, iudicia bona, canalis
fractus.
