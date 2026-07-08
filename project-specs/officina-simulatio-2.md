# Officina simulatio 2 — M0b (typatio expressionum + conversiones)

Date: 2026-07-08. Technique: simulate-work (pretend implementation,
simulated ~2.5h clock, real instruments for grounding). Target:
officina M0b per silva-semantica-design.md DECISUS 2/3 + the
phase-log ADDENDUM ANTE COMPACTIONEM engine hints. Simulated against
the SHIPPED M0a module (silva_c89_semantica.{h,c}, 2,458 lines).

Grounding (real, not simulated):
- Grammar loci inventory (agent, cross-checked genera-c89.md +
  silva_tabulae_c89.h + the LOCI species table at
  silva_tabulae_c89.c:11398): complete expression-position genus set
  = virgula, assignatio (ALL 11 ops incl. compound — one genus),
  ternarius, binarium (all 12 tiers — one genus), conversio,
  unarium (INCLUDING prefix ++/-- — no praecrementum genus exists),
  magnitudo-expressionis, magnitudo-typi, subscriptio, vocatio,
  accessus (one genus for . and -> via tok_operator; member name is
  a bare TOKEN), postcrementum, parenthesis (dual-use with
  declarators), folium-{identificator,integer,fluitans,character,
  chorda}, + AMBIGUUS materialized in any NODUS locus. folium-chorda
  = ONE node per adjacent-literal group, tok_valor = LISTA_MIXTA of
  string fragments. vocatio.argumenta / congeries.elementa =
  LISTA_MIXTA with comma TOKENS interleaved.
- Statement/decl expression loci (13): si/dum/fac-dum conditio,
  per→per-clausula{initium,conditio,passus} (all optional; initium
  may be declaration OR expression), commutatio discrimen, casus
  valor, redde valor, sententia-expressionis expressio,
  declarator-initiatus initiator, enumerator valor, campus latitudo,
  declarator-aciei mensura.
- Node census (agent, throwaway probe, oraculum=NIHIL upper bound):

  | file | nodes | expr nodes | expr% | arena peak |
  |---|---|---|---|---|
  | arbor2_glr_tabula.c | 145,462 | 116,588 | 80.2% | 3.18 GB |
  | chorda.c | 6,097 | 3,590 | 58.9% | 91.4 MB |
  | silva_parsare.c | 4,098 | 2,352 | 57.4% | 65.1 MB |

  Rule of thumb: expr ≈ 57-59% of nodes handwritten, ~80% generated
  (congeries-dominated: 19,731 in arbor2 = 13.6%). Leaves
  (identificator+integer) ≈ half the expr entries. Table cost at
  24-32 B/entry: worst file ≈ 3-3.7 MB.
- Closure mechanics (direct read): commit-time collapse copies the
  victor OVER the wrapper allocation (identity kept); recanonicare
  flips the canonica INDEX in place, wrapper remains, observer
  pointers stay valid (silva_commissio.h:12-15,177-185). percursus
  order: analysare → augere → vacare → recanonicare
  (percursus.c:449-491). Corpus closure: 1,939 versae + 1,051
  confirmationes + 411 indecisa.
- tabula_dispersa_creare(piscina, capacitas, friatio, comparatio)
  generic constructor confirmed (include/tabula_dispersa.h:66) —
  pointer-keyed table = chorda clavis over 8 stored pointer bytes,
  custom friatio/comparatio. systema: size_t = unsigned long,
  ptrdiff_t = long, wchar_t = int.

## Complications (four-part schema)

### 1. Typing is canonical-relative → authoritative analysis runs POST-closure
- discovered-while: honestly PRE-sim (re-orientation read of
  percursus.c); the sim confirmed it sharp against the flip
  mechanics and found the chicken-and-egg
- consists-in: percursus analyzes BEFORE recanonicare, and closure
  then flips 1,939 canonicals corpus-wide IN PLACE. Annotations
  written during analysis describe pre-flip canonical readings; the
  new canonical readings have NO entries. And sem #1 cannot be
  skipped: closure FEEDS on analysare's symbols (oraculum_augere) —
  typing cannot simply move after closure inside one pass
- consequences: demissio reads post-closure trees; a single-pass
  annotation table fails the bar on exactly the flipped forks;
  single-file API users without closure are unaffected
- handled-by: TWO-PASS percursus — sem₁ feeds closure (its
  annotations discarded) → recanonicare → sem₂ authoritative
  (reported, queried). API contract stated in silva.h: "typing is
  canonical-relative; analyze after your last recanonicare." Cost =
  2× analysis = invisible (M0a: semantica rides free). Probatio pins
  a flip fixture (type before ≠ type after; sem₂ correct)

### 2. Congeries: type flows DOWN, not up (+ brace elision)
- discovered-while: chunk-C pretend on initiator typing (the M0b
  unpark); census showing congeries = 13.6% of arbor2's nodes
- consists-in: every other expression SYNTHESIZES its type
  bottom-up; a congeries has no intrinsic type — it takes the
  aggregate type from CONTEXT (the declarator), and element↔member
  correspondence is a layout-guided walk that C89 brace elision
  ({1,2,3,4} legally initializing int[2][2]) makes stateful
- consequences: initiator typing is a top-down correspondence
  engine, not a recursion into typare; full elision support is a
  real sub-module
- handled-by: M0b annotates the congeries with its CONTEXT type and
  types element expressions where correspondence is direct
  (fully-braced); elision-shaped initializers get a NAMED diagnostic
  which IS the corpus measurement (expected ≈0 — generated tables
  are fully braced); full elision walk parked with landing spot M1
  demissio (static-data materialization needs the same engine anyway)

### 3. UAC ranking for the long long extension is OUR decision
- discovered-while: chunk-B pretend writing the conversion ladder
- consists-in: C89 defines usual arithmetic conversions without long
  long; the corpus requires long long (s64). Where LL/ULL rank, and
  the signed/unsigned representability outcomes on LP64 (unsigned
  int + long → long; unsigned long + long long → unsigned long
  long), are choices our base standard doesn't make for us
- consequences: demissio's frozen input contract embeds the choice;
  a silently wrong rank = wrong machinula arithmetic at M2
- handled-by: adopt C99 integer conversion ranks verbatim; worklog
  entry + pinned fixtures for the LP64 corners; certified by
  complication 4's instrument

### 4. AUSPEX — the conversion engine is exhaustively clang-certifiable
- discovered-while: asking how haruspex could verify expression
  types (it cannot: body expressions with locals are not liftable
  into a TU)
- consists-in: arbitrary expressions can't be replayed to clang, but
  the UAC table over primitives CAN — via integer CONSTANT
  expressions: `(int)1 + (unsigned)1` is a constant expression, so
  one generated TU can assert, for every primitive pair × operator:
  result SIZE (sizeof in an array bound) and result SIGNEDNESS
  (`((cast)1 op (cast)1) * 0 - 1 > 0` in an array bound — 1 for
  unsigned results, 0 for signed). Size+signedness = full integer
  identity. Known gap: double vs long double identical on Darwin
  (both 8) — noted, not distinguishable, harmless here
- consequences: M0b's conversion table gets the SAME certification
  standard as M0a's layout (probatio-pillar culture); dissent =
  wrong UAC caught before demissio exists
- handled-by: new small instrument auspex.c + auspex.sh (haruspex's
  sibling — bird-watching to its entrails): enumerate integer
  primitive pairs × representative operators, emit compile-time
  asserts, clang -fsyntax-only = verdict. Chunk-B bar: 0 dissent

### 5. folium-chorda drags the escape decoder in early
- discovered-while: chunk-A pretend typing string literals — the
  type is char[N] and N requires decoding
- consists-in: one node per adjacent-literal GROUP (fragment token
  LIST); N = Σ decoded fragment lengths + 1; decoding needs full
  C89 escapes including the parked \x/\NNN
- consequences: the \x/\NNN unpark moves from "while touching that
  code" to REQUIRED for chunk A; the decoder is shared with
  folium-character's aestimator path
- handled-by: one _fuga_decodere helper used by both aestimator and
  chorda-length; wide strings (L"...") = named-park diagnostic
  (corpus: none)

### Pleasant surprise (recorded, not a complication)
Implicit extern-int SELF-HEALS the POSIX residue for expression
typing: `close(fd)` types as int by C89 6.3.2.2 semantics — legal,
correct, no diagnostic. M0a residue category (b) costs M0b nothing.

## Dissolved fears / cheap parks
- **Table volume / eager-vs-lazy: REFUTED BY MEASUREMENT.** The
  pre-sim fear that generated files force a lazy table died in the
  census: worst file ≈ 3.7 MB of annotations riding beside a 3.18 GB
  parse arena (0.1%). Eager, uniform, no laziness machinery. Same
  lesson as the M0a arena story: measure before architecting.
- Wrapper-vs-reading annotation: annotate the READING node (concrete
  genus, post-_canonicum); the public query does _canonicum first so
  callers may pass either. Mild — the sharp version is complication 1.
- Prefix/postfix confusion: no praecrementum genus; prefix ++/--
  ride unarium (typed as operand's type, not int — unarium typing is
  operator-dispatched anyway). Inventory settled it before code.
- Statement-loci sprawl: exactly 13 expression-bearing loci,
  enumerated above. Mechanical.
- parenthesis dual-use (expression vs declarator grouping): the walk
  is positional — only expression positions get typed; same genus in
  declarator chains is already handled by M0a's unwind.
- accessus member name is a bare TOKEN — never recursed as a
  sub-expression; chorda-compare against tag membra; qualifiers of
  the basis MERGE onto the member type (const struct → const member).
- Pointer-keyed table mechanics: generic constructor confirmed;
  entry struct holds the SilvaNodus* and the chorda key points at
  those 8 bytes in the entry — no separate key allocation.
- Lifetime: annotations reference nodes in the parsura piscina; sem
  lifetime already nests inside it (M0a recorded constraint). Holds.
- lvalue-ness: NOT annotated in v1 — demissio derives it
  structurally (identificator/subscriptio/accessus/deref/chorda);
  assignment-lvalue legality is lint's. Named park, added on pull.

## The annotation contract (the API to freeze — Eskil rule)
Per-node PAIR, both TypusC89* (interned/nominal per M0a):
- **typus naturalis** — the expression's own synthesized type,
  UNdecayed (arrays stay arrays, function designators stay functions)
- **typus conversus** — the context-applied conversion target
  (UAC operand conversion, array/function decay, assignment RHS
  conversion, argument conversion, return conversion), NIHIL if none

Worked examples: `i + l` → i:{int, long}, l:{long, NIHIL},
(+):{long, NIHIL}. `arr[i]` → arr:{int[10], int*}. `f(ch)`
unprototyped → ch:{char, int}. sizeof-operand: NO decay, conversus
NIHIL (that's what makes sizeof(arr) right). Compound assignment
annotates the implied binary op's conversions on both operands.
Public rows (M0b): silva_c89_typus_expressionis(sem, nodus) and
silva_c89_conversio_expressionis(sem, nodus), both _canonicum-aware,
NIHIL-safe. Demissio reads exactly these two columns.

## Structural proposal (chunk map for the INTENTIO)
- **A** — annotation table (pointer-keyed) + walk descent into all
  13 loci + primaries: identificator resolution against LIVE scopes
  (incl. implicit extern-int registration), integer/float/char
  literals (suffix typing: 10u → unsigned, 10L → long, 1.5f →
  float), folium-chorda char[N] + _fuga_decodere (\x/\NNN unpark),
  parenthesis, unarium, postcrementum. Bar: fixtures + sweep "N
  expressionum typatae, 0 ruinae".
- **B** — UAC engine (C99 ranks, LP64) + binarium/ternarius
  (pointer composite rules: void*+T*, null-pointer-constant+T*) +
  assignatio (incl. compound implied-op) + virgula + pointer
  arithmetic (T*±int → T*; T*−T* → long) + **auspex**. Bar: auspex
  0 dissent + pinned LP64 corner fixtures.
- **C** — vocatio (reditus; prototyped arg conversions; variadic
  default promotions; implicit extern-int) + subscriptio + accessus
  (member lookup, qual propagation, incomplete-tag poison) +
  magnitudo-expressionis unpark (aestimator↔forma↔typatio
  three-way recursion; sizeof(arr) undecayed) + congeries context
  typing (elision diagnostic = the measurement). Bar: corpus
  coverage % printed per file.
- **D** — two-pass closure pipeline in percursus + publica (2
  silva.h rows + hospes calls) + coverage counters + officina
  spec-v2 §XII.8 stale-refs refresh + M0b RELATIO + full phase-log
  audit (milestone boundary). Bar: post-closure corpus number
  ("every expression node typed or carrying a named diagnostic"),
  VERIFICATUM.

## Interview questions for Fran
1. Two-pass percursus (sem₁ feeds closure, sem₂ authoritative) +
   the "canonical-relative" API contract — approve? Sub-question:
   expression typing always-on in analysare (cost invisible,
   complete-first) vs a skip-flag for the closure-feeder pass?
2. The {naturalis, conversus} per-node pair as demissio's FROZEN
   input contract — approve? (This is the API-first moment of M0b.)
3. Congeries scope: context-type + direct correspondence now,
   elision measured via named diagnostic, full elision walk parked
   to M1 demissio — approve?
4. Auspex in chunk B — approve the new instrument? (small; gives
   UAC the haruspex certification standard)
5. lvalue bit omitted from v1 annotations (demissio derives
   structurally) — approve the park?
6. Chunk map A-D — approve as the INTENTIO skeleton?

## Prediction audit
Pre-sim predictions (registered in conversation before the agents
returned): (i) expression-node volume on generated files forces an
eager-vs-lazy table decision — **REFUTED**, the census killed it
(3.7 MB vs 3.18 GB; 0 points; the fear was wrong but the instrument
was right to build). (ii) wrapper-vs-reading annotation wrinkle —
**CONFIRMED but mild** (half point; annotate the reading, query
through _canonicum). The genuinely new finds were unnamed by
prediction: congeries top-down typing + brace elision, and the
auspex opportunity. Complication 1 (sequencing) was found BEFORE the
sim by the re-orientation protocol itself — evidence the re-entry
discipline pays. Meta-lesson, twice now: the real wall is never
quite where predicted; simulate anyway — but this time HALF the
value was measurement (census, flip mechanics) rather than
narrative, which is itself a finding about the technique.
