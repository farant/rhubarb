# C89 LACUNAE — the standing completeness ledger

*Born 2026-07-11 (M4a on-ramp, Fran's request). ONE place answering:
"what stands between this toolchain and complete C89 support?" The
parks were all documented but scattered — five phase-logs, two dragon
audits, three specs. This ledger is MAINTAINED, not append-only:
update a row when its park opens, closes, or changes owner; every
claim cites its source; suspicions live in AUDIENDA until verified on
touch — do not promote a guess to a row.*

*Discipline note: this is a map of NAMED deferrals, not a work queue.
The house rule stands — rows graduate on PULL (a consumer needs it),
never checklist-completionism. The corpus bar is the acceptance bar;
these rows are exactly the C89 territory the corpus never exercises.*

Status vocabulary: **PARCATUM** (deliberately parked, source cited) ·
**IN CURSU** (inside an active milestone) · **AUDIENDUM** (state
unknown; audit on first touch) · **MORTUUM** (corpus-dead by audit;
park indefinitely).

## I. Lexer / praeprocessor (silva)

| Feature | C89 | Status | Detail + pull |
|---|---|---|---|
| L"wide string" / L'wc' literals | 6.1.4 | PARCATUM | Pre-parser park (M1a find, silva books) — wide literals never reach the tree. Pull: first corpus/user need; none in 782 files. |
| __DATE__ / __TIME__ | 6.8.8 | PARCATUM | Deferred-named at pre-M1 expander work (they break determinism/resume anyway — see workflow rules). __FILE__/__LINE__/__STDC__ SHIPPED. |
| defined(__FILE__) corner | 6.8.1 | PARCATUM | Named limit at pre-M1; cosmetic. |
| Trigraphs | 5.2.1.1 | AUDIENDUM | Almost certainly absent; nobody mourns. Conformance-trivia row so the deferral is named. |
| #pragma / #line / #error handling | 6.8 | AUDIENDUM | Corpus passes 750+ files so core pp is solid; the directive corners want a one-time audit on touch. |

## II. Iudicium (semantica / examen) — compile-time constraints

M4a (EXAMEN) v0 SHIPPED 2026-07-11: compatibility relation,
target-context three-way seat, locus (lvalue/const/modifiability),
arity, return void/non-void, controlling-scalar, redeclaration
type-compat, operator-site pointer-target compat, + the ~40
pre-existing diagnostics productionized. See officina-m4a-spec.md
+ silva/phase-log.md M4a RELATIO. Deferred:

| Feature | C89 | Status | Detail + pull |
|---|---|---|---|
| switch case-set (dup cases, >1 default, integral discriminant) | 6.6.4.2 | PARCATUM | M4a spec §VII. Aestimator ready; needs switch-scoped walk state. Pull: REPL reports / corpus need. |
| Initializer validation (excess, shape, static-const, string-into-array sizing) | 6.5.7 | PARCATUM | Completion-by-initializer SHIPPED (M0b worklog 2026-07-10); VALIDATION distinct and unbuilt. Pull: first real miss. |
| Struct/union member legality (bitfield widths, fn/incomplete members) | 6.5.2.1 | PARCATUM | Rides the bitfield park (§III). |
| Cast constraints (both sides scalar) | 6.3.4 | PARCATUM | Decidable from tables, deliberately unwired — casts = "trust me" in v0. |
| Object-of-incomplete-type declarations | 6.5 | PARCATUM | Access side ENFORCED; declaration side (`struct S x;` never completed) silent. |
| Enum int-representability | 6.5.2.2 | PARCATUM | Enum-constant constant-ness enforced; representability unchecked (rare). |
| Lint tier growth (sign-compare, shadowing, narrowing…) | — | PARCATUM | Not C89 CV — domesticum tier grows on pull (silva-lsp lane). |
| Logical-operand scalar (&&/\|\|/! operands) | 6.3.13-15 | PARCATUM | M4a wired statements+ternary conditions only; the logical-op operand seats exist unchecked. Pull: fixture demand. |
| break/continue outside loop/switch | 6.6.6.2-3 | PARCATUM | M4b chunk A V1 audit: NO loop-context codex exists (54 codices); a wrapped top-level `frange;` turn passes judgment silently — demissio/machinula own the honesty downstream. Pull: REPL user reports / fixture demand. |
| & of register-storage object | 6.3.3.2 | PARCATUM | Tiny CV; repositio flag exists. Pull: first miss. |
| typedef redeclaration in same scope | 6.5 | PARCATUM | Genus-filter exempts TYPEDEF from redecl compat. Pull: REPL reports. |
| Cross-parsura tag identity (systema seam) | 6.1.2.6 | PARCATUM | M4a find: same-text tags across parsuras nominally distinct; redecl seat clement on ex_systemate priors. Rides the cross-TU park (§V). |
| Bare return in non-void: oracle demotion | 6.6.6.4 | PARCATUM | Legal C89, SUSPECTUM in examen; clang-16 auto-error and -Wno-error=return-type would also demote the REAL CV (value-in-void). Fixture waits on demotion decision — named vectis EXCLUSIO. |
| Lexicon growth (network/deep POSIX) | — | PARCATUM | The 39 pinned corpus exclusions are all lexicon classes; systema_posix lacks Wave-3 (sockets/netinet). Pull: shrinking exclusiones.txt. |

## III. Demissio / medulla / semantica shared

| Feature | C89 | Status | Detail + pull |
|---|---|---|---|
| Bitfields | 6.5.2.1 | MORTUUM | Dragon audit (officina-brainstorm): zero corpus uses. Semantica: "campi - forma parcata"; demissio/machinula consequently absent. Pull: first real consumer unparks the WHOLE column (forma → judgment → lowering → VM). |
| long double | 6.1.2.5 | AUDIENDUM | On arm64-darwin long double == double (64-bit) — the platform may make this free, but whether silva/semantica/demissio type it AT ALL is unaudited. |
| User-defined variadic function BODIES (stdarg.h va_*) | 4.8 | MORTUUM | Dragon audit: no monorepo varargs bodies. CALLING variadics works (pons variadicus, printf-family builtins). Defining one interpreted does not. Pull: first corpus body. |

## IV. Machinula / libc (runtime)

| Feature | C89 | Status | Detail + pull |
|---|---|---|---|
| setjmp / longjmp | 4.6 | MORTUUM | Dragon audit: corpus-unused; machinula has no non-local-jump machinery (exit = vexillum halitus, no longjmp by design). Unparking = real VM work (frame unwinding). |
| signal.h | 4.7 | AUDIENDUM | Presumed absent from the builtin table; corpus demand unmeasured. C89-required but interpreter-hostile. |
| locale.h | 4.4 | AUDIENDUM | Presumed absent; "C" locale semantics are what the house wants anyway. |
| Full C89 libc audit vs shipped builtins | 4.x | AUDIENDUM | ~60 corpus-measured builtins shipped (M2d) of the ~105 audited list (spec-v2 §IV). The DELTA list (scanf family? strtod? full math?) wants one audit pass — corpus-measured shipping was the deliberate posture, so "missing" = "unpulled". |
| time/entropy determinism (Wave 2 exclusions) | 4.12 | PARCATUM | Named wave exclusions (M2d exclusion table); v2 time-travel flagship points the resolution way (recorded builtin results). |

*(Not lacunae: network/GUI/opendir waves are POSIX, not C89. POSIX
completeness is systema_posix territory, out of this ledger's scope.)*

## V. Trans-TU / conexio (link-time)

| Feature | C89 | Status | Detail + pull |
|---|---|---|---|
| Cross-TU structural compatibility + composite type formation | 6.1.2.6 | PARCATUM | M4a judges one TU (sim-6 C2 — this is what made the relation small). Conexio censuses duplicate symbols but compares NO types. Pull: M4b/link era; the REPL's incremental world will hit implicit-then-real reconciliation first (fundamenta fact 6). |

## VI. AUDIENDA — the standing question list

Rows above marked AUDIENDUM, collected: trigraphs; #pragma/#line/
#error corners; long double typing through the pipeline; signal.h /
locale.h presence; the shipped-builtins vs full-C89-libc delta.
Resolution rule: audit on first touch of the neighborhood, then
promote the row to PARCATUM (with pull) or close it; never leave a
resolved row in AUDIENDA.

## Cross-references

officina-m4a-spec.md §VII (examen parks mirror §II here) ·
officina-spec-v2.md §IV (builtin audit) + §XI (milestones) ·
officina-brainstorm.md (dragon audits) · silva/phase-log.md M0b/M1a
RELATIO (bitfield, L-string, initializer-completion finds) ·
officina/phase-log.md M2c/M2d (waves, exclusion tables, builtin
posture). EXCLUSIO rows in the M4a differential harness point at §II
rows by name (Bar D condition 5).
