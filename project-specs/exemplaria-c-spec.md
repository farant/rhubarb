# exemplaria in C — the extractor, and the boundary it must not cross

*2026-09-18, hours after the excusatio arc closed at `f51be337` and
tier 1 was wired into pre-commit at `ac0b6d89`. Origin: desideratum
`01M2RYR3JJ`, deferred by decree `01M2S9GR5W` — both now overtaken.
Fran's framing, which is the spine of this spec: **materia provides
tools that make a "crusta lint" easy to write; materia does NOT know
its clients.***

**What it is.** Tier-2 lint findings are extracted from an expanded
STML document by Python's `html.parser`. This moves that extraction
into C, so findings exist in C — reachable by the LSP, by the commit
path, and by any client — and the foreign HTML parser leaves the
middle of our own toolchain.

**Why now.** The deferral's stated reason was *"nihil hodie inventa in
C poscit — crusta regulas lintris ZERO habet, LSP non ordinatus"*.
Crusta has a rule. And a third trigger appeared that the ticket never
listed: **the commit path must not depend on Python**, so tier 2
cannot join tier 1 in `pre-commit` until this exists. Rule 1 therefore
runs nowhere automatically, and `-nt` regressions are currently
uncaught.

## 0. What already exists (measured 2026-09-18, do not re-derive)

- **Ratio today**: 2626 lines C / 512 Python across the diagnostics
  stack — 84% / 16%.
- **Every stage but extraction is already a library call**, tree to
  tree: `crusta_arbor_parsare` → `materia_arbor_proicere_nodum`
  (`consilium.sedes_scribere`, handing back
  `MateriaArborScriptura.arbor`) → `stml_expandere(radix, piscina,
  intern)`, which **takes a tree and returns a tree**.
  `materia_arbor.h:91` says that handed-back tree exists *"pro
  consumptore exemplarium"* — built for this consumer before it
  existed.
- **`stml vertere` has exactly one caller**, `silva._relata`. It exists
  *solely* so Python's `html.parser` can read our own STML.
- **`TRANSPARENTIA` is per-SUBSTRATE, not per-client.** The whole table
  is two entries: `materia` → `('ante post', 'sedes octeti')` and
  `silva`. All six materia clients share one preface.
- **materia has no notion of "a client"** — no descriptor, no registry.
  `tools/diagnostica.c`'s `_clientem_parsare` is the only cross-client
  dispatcher in the house.
- **The printer is generic but trapped.** `_diagnosticum_imprimere` is
  `interior` in `tools/diagnostica.c`; its parameters are
  `piscina, via, grammatica, fons, mensura, MateriaDiagnosticum,
  machina, excerptum, summa` — nothing client-specific.

## 1. Decisions

**E1 — materia RECEIVES a client's configuration; it never HOLDS one.**
An earlier draft proposed `MateriaCliens`, a table of all clients
inside materia, dispatched by suffix. Fran rejected it and M8 says why:
*"Language-specific code sits BESIDE materia, never inside. Every
'just one convenience that knows about a language' is the rot."*

The struct survives; its OWNERSHIP inverts. **The test, and it is the
one that matters:**

| | rejected | as specified |
|---|---|---|
| who holds it | materia, as a table | the client, per call |
| who dispatches | materia, by suffix | nobody — the client knows who it is |
| **adding a client** | **edit materia** | **edit nothing in materia** |

If adding JS means touching a file under `materia/`, the design has
failed. If JS fills in a struct and calls, materia never learned JS
exists.

**E2 — the extractor is CLIENT-BLIND.** It takes an expanded STML
tree and a rule, and returns `MateriaDiagnosticum` rows. It reads
`sedes` / `octeti` (which materia's writer emits uniformly) and
`lint` / `gravitas` / `causa` / `nota` (which the rule author writes).
It never sees a file, a suffix, or a grammar.

**E3 — each client owns a FACADE, and there is no separate lint
instrument.** A consumer of crusta must not orchestrate materia. It
calls crusta; crusta calls materia. Tier 2 therefore arrives INSIDE
an existing entry point rather than beside it — adding rule 2 changes
no caller and no signature.

**Orchestration is hidden; VOCABULARY is shared.** Consumers receive
`MateriaDiagnosticum` but never call a materia function — the way one
receives a `chorda` without knowing chorda's internals. Were crusta to
invent `CrustaDiagnosticum`, every client would reinvent the record,
cross-client tools would need N translations, the printer N overloads,
and `tools/diagnostica` could not be a thin dispatcher at all.

**E4 — the printer leaves the tool; its home is an INTERNAL choice.**
`_diagnosticum_imprimere` is `interior` in `tools/diagnostica.c` and
takes nothing client-specific, so every facade would otherwise
copy-paste it.

The rule at `materia_diagnostica.h:11` and `materia/CLAUDE.md:135` is
narrower than it first reads: `include/excerptum.h` states the reason —
*"sedem materia dat, formam haec; ergo nihil de arboribus scit et A
QUOLIBET FONTE vocari potest"*. The rule forbids REIMPLEMENTING form
in materia, not composing it. `lib/excerptum` already produces a
string into a `ChordaAedificator` and is tree-free.

What is missing is the layer above — `MateriaDiagnosticum` → full
diagnostic text — which needs a materia type and so cannot live in
`lib/`. Because E3 puts rendering behind the facade, this becomes an
internal placement decision rather than a public commitment: the plan
picks it, the spec does not bind it. **Generation returns a `chorda`;
only the instrument writes to stdout.**

**E5 — one parse serves both tiers.** The client instrument parses
once and runs tier 1 (registry) and tier 2 (rules) over the same tree,
with ONE annotation collection and ONE exemption pass covering both.
A `<tolera>` suppresses either kind.

**E6 — no TSV on the in-house path.** Findings go extractor →
exemption → printer in memory. The 12-field TSV survives as the
FOREIGN-producer entrance only; `-lege` and `-excusa` keep working for
anything outside the house. The seam's direction reverses, as
`01M2RYR3JJ` predicted — it is not deleted.

**E7 — `tools/diagnostica.sh` becomes a THIN DISPATCHER** (Fran,
2026-09-18): suffix → exec the client's instrument. The pre-commit
line never changes, the multi-file convenience survives, and suffix
knowledge stays out of the hook.

**E8 — the extractor is written against the VOCABULARY, not rule 1.**
`<relatum>`, wrapper elements, `nota`, the descent-suppression law are
specified in `sedes-multiplices-spec.md` §9 and the adaptator plan,
independent of any rule. Rule 1 is one consumer, not the contract.

## 2. The extractor

`materia/fontes/materia_exemplaria.{h,c}`:

```c
Xar*                                   /* de MateriaDiagnosticum */
materia_exemplaria_applicare (
                     Piscina* piscina,
       constans MateriaNodus* radix,        /* arbor clientis */
constans MateriaLexiconRatum* lexicon,
                   StmlNodus* regula,       /* documentum iam lectum */
         InternamentumChorda* intern);
```

Four pieces:

**(a) Document assembly, ~40.** Projected tree + `TRANSPARENTIA`
preface (if the rule declares none) + the rule's elements. Python
splices STRINGS here and needs a `_LIMES_RELATORUM` sentinel to find
the boundary again; with trees it is `xar_addere` and the sentinel
disappears.

**(b) The `<relatum>` walk, ~100–140.**

```
for each top-level <relatum> R:
    lint, gravitas, causa, nota ← attributes of R
    for each child element C of R:          /* one row per child */
        walk(C, nota_enclosing):
            if node has "sedes":
                record (sedes, octeti, nota_enclosing)
                return                      ← do NOT descend
            for each child: walk(child, node.nota ?: nota_enclosing)
        first recorded = primary; the rest = relata
```

**This is SHORTER than the Python it replaces.** `_relata`'s depth
counter, `notae` stack, `gradus_capti` bookkeeping and `_VACUA_HTML`
set are all SAX scaffolding: over a real tree, recursion IS the stack,
"stop descending" is `return`, and void elements do not exist because
there is no HTML.

**(c) Rows → `MateriaDiagnosticum`, ~40.** Parse `sedes="L:C-L:C"` and
`octeti="B-B"`, slice the source, `codex = "lint:" + lint`, default
`gravitas` to erratum.

**(d) Two-arm subtraction, ~30.** `X-negata` removes matching
positions from `X`, REFUSING when the negative arm is not a subset —
two arms capturing different positions yield a number that looks fine
and means nothing.

**Total ~250–290 new C.**

## 3. The shared sequence, and who owns it

Every client would otherwise copy the same hundred lines: derive tier 1,
extract tier 2 per rule, collect annotations, apply exemptions, sort.
That sequence is written ONCE, parameterised by a configuration the
CLIENT constructs (E1):

```c
nomen structura {
    constans character*               grammatica;   /* "crusta" */
    constans MateriaRegistrumCoctum*  registrum;
    constans MateriaLexiconRatum*     lexicon;
    constans MateriaDiagnosticaCocta* declarata;
    constans character*               praefixum;    /* "#" */
    Xar*                              regulae;      /* StmlNodus* IAM lecta */
} MateriaDiagnosticaRatio;

Xar*
materia_diagnostica_plena (
                          Piscina* piscina,
            constans MateriaNodus* radix,
constans MateriaDiagnosticaRatio* ratio);
```

`regulae` are already-parsed trees, so materia performs no rule
discovery and reads no file. Which directory holds the rules is client
policy; glob-and-parse is a shared helper the client calls with its own
path.

## 4. The client facade

Three levels, each a legitimate stopping point. A consumer picks one
and never sees materia's machinery:

```c
/* I - textus solus: qui errores videre vult */
chorda crusta_diagnostica_textus (Piscina*, constans character* via,
                                  constans character* fons, i32 mensura);

/* II - recorda: vocabularium commune, orchestratio abscondita */
Xar*   crusta_diagnostica_omnia  (Piscina*, constans character* fons,
                                  i32 mensura,
                                  constans CrustaOptiones*);

/* III - arbor, pro eo qui plus vult (IAM EXSTAT, immota) */
MateriaNodus* crusta_arbor_parsare (...);
```

`optiones == NIHIL` runs **everything in `crusta/lintrum/`** — the
default. A caller wanting named rules fills `optiones->regulae`
(Fran, 2026-09-18). Level II is where both tiers, annotations and
exemptions happen; the LSP stops there and gets structured ranges, a
CLI stops at level I.

The facade itself is ~20 lines:

```c
Xar*
crusta_diagnostica_omnia (piscina, fons, mensura, opt)
{
    radix = crusta_arbor_parsare(piscina, fons, mensura,
                &CRUSTA_BASH, &relatio);        /* crustae proprium */
    ratio = { "crusta", &CRUSTA_REGISTRUM, &ratum, &CRUSTA_DIAGNOSTICA,
              "#", opt ? opt->regulae : _regulae_ordinariae() };
    d = materia_diagnostica_plena(piscina, radix, &ratio);
    xar_extendere(d, crusta_diagnostica_emissa(piscina, radix,
                         &relatio));  /* quod parsator SOLUS scit */
    redde d;
}
```

**Dependency direction: consumer → crusta → materia → lib.** crusta
always depended on materia; what changes is that the consumer no longer
does.

**Porcelain — globs, file lists, exit codes, counters — stays at the
INSTRUMENT layer** (`tools/`), where `git ls-files` and exit contracts
already live. One call per file is the library shape; iterating is tool
policy.

## 5. What Python loses

`_relata` (109), `_vertere` (18), `_sedem_ordinis` (14),
`_sedem_relatam` (22), `_notam_mundare` (9), `diagnostica_tsv` (23),
`diagnostica_ex_congruentiis` (20), `congruentiae_minus` (34),
`diagnostica_lintris` (31), `diagnostica_pingere` (24) ≈ **300 lines**,
plus the `stml vertere` STAGE entirely.

What survives: `exemplaria`'s orchestration (glob, parallelism) as a
thin face over the instrument. If orchestration ever moves, decree
`01M2S9GR5W` governs: **processes, not threads.**

## 6. Gates, each born red by a plant

| # | gate | plant |
|---|---|---|
| I | extractor rows match the Python extractor over the house corpus | change the descent-suppression `return` to a descend |
| II | wrapper `nota` reaches the right span | drop the enclosing-nota parameter |
| III | `A − B` closes; non-subset REFUSED | make the subtraction silent |
| IV | one parse, both tiers, one exemption pass | collect annotations twice |
| V | dispatcher reaches the client instrument | break the suffix case |
| VI | printer output byte-identical to today | — regression guard |

**Gate I is the real one**: the C extractor must reproduce the Python
extractor's rows over all 239 `.sh` files before the Python is
deleted. Two implementations, one corpus, zero divergences — the same
method the sedes arc used for 5,501,210 elements.

## 7. Non-goals and named doors

- **No `MateriaCliens`, ever** (E1). A second client wanting lint
  writes its own instrument; that is the design, not a gap.
- **No parallelism here.** Serial always; `-fila N` is `01M2S9GR5W`'s
  shape if it ever comes, and its gate is *serial output == parallel
  output*.
- **`-lege` / `-excusa` stay** for foreign producers (E6). They become
  unexercised in-house — deferred with a named trigger, not deleted.
- **css gets no lint instrument here.** Only crusta has rules. When
  css does, it writes its own — and its block comments trip the
  `clausura` door already named in `excusatio-declarata-spec.md` §11 B.

## 8. AUDIENDA

1. ~~Does the client instrument subsume tier 1?~~ **ANSWERED by E3:**
   yes, by construction — the facade runs both tiers, so there is no
   separate lint instrument and no fallback to specify.
2. `exemplaria`'s Python face still shells out per file. Once the
   instrument takes a file LIST, is the face one call or N?
3. ~~Rule-file discovery convention?~~ **PARTLY ANSWERED:** the
   default is "everything in `<cliens>/lintrum/`" and the explicit API
   takes parsed trees. What is still open is whether the shared
   glob-and-parse helper lives in materia (which reads files only in
   `materia_coctor` today) or at the instrument layer.
4. EX8 of the excusatio spec (dead `lint:` exemptions) becomes
   judgeable once a facade knows its full rule set — `ratio->regulae`
   IS that set. This arc is its named trigger.
5. **Six near-identical facades is the `recentius_ex` shape** (one
   defect, four files, the hand census found one). Fran's position
   (2026-09-18): not a long-term worry, because this is exactly what
   a structural lint catches once the machinery exists — the thing
   being built polices the thing being risked. Recorded as a NAMED
   FUTURE, not an architectural guard: a rule over facade shape, at
   whatever granularity silva/materia make cheap.
