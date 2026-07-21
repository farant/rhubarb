# aedilis — interview raw (2026-07-20)

Feature: the build layer's on-ramp (park 01KXJ2HV pulled → tractum).
Point aedilis at a C file → derived dependency closure → targeted
build script (saved or executed). Pre-interview finds (this session):
fabrica.tsv is the self-described exception ledger awaiting
absorption; amalgamare.sh ×3 + gesta RADIX_FONTES = four
hand-maintained lists; the -MM closure shape has three consumers;
legati inclusiones probe showed one-hop warm edges; via_normalizare
handles ".." and silva's fontes provably don't call it.

## Round 1 — structural

**Q1. First arc's deliverable?**
A: **Engine + script emitter.** Amalgam emitter and -plagulae
derivation are follow-ups on the same engine; they don't gate the
seal. (Amalgam desideratum: 01KY0XRBEX.)

**Q2. Closure lane?**
A: **Silva-primary from the start.** (Ambitious option, against the
recommend.) The silva work items become part of the arc; clang -MM
is not the primary derivation.

**Q3. Tool language?**
A: **C89 tool (bin/aedilis).** Eat the dogfood: chorda/via/toml/xar/
stml do the work; the engine that will link silva starts in the
right language.

**Q4. Artifact of record?**
A: **Manifest → script.** Engine emits a manifest per target (the
derived truth with per-entry provenance); the bash script is
generated FROM it. Manifest = diffable/queryable; script = the
executable convenience.

## Round 2 — engineering

**Q5. How does C-aedilis get silva's graph?**
A: **Link the silva amalgam + incremental cache.** Aedilis parses
files itself via linked silva, keeps a persistent include-graph
cache invalidated per-file by mtime — first run seconds,
steady-state milliseconds.

**Q6. clang -MM's role under silva-primary?**
A: **Gate + permanent minority lane.** differentia-clausurae ships
in this arc; silva-primary must show CONSENSUS on the whole corpus
before trust; -MM remains permanently for .m files and as the loud
fallback; manifests record which lane produced each entry.

**Q7. Manifest format?**
A: **STML.** The park's own word; lib/stml.c means aedilis emits and
parses natively; nesting fits per-entry provenance + flag groups.

**Q8. Config home for non-derivables?**
A (verbatim): "maybe an aedilis.stml?" → **aedilis.stml** — new
aedilis-owned committed config, SAME grammar as the manifests.
fabrica.tsv stays the excubitor's manual ledger under Fran's hand
(absorption remains a later aedilis milestone).

## Round 3 — consumers + proof

**Q9. Consumers in the first arc's seal?** (multi)
A: **Standalone `aedilis <file>`** (the original ask) +
**gesta RADIX_FONTES regeneration** (a real hand list retired;
subtree + vendor coupling proven) + **colloquium/interpretare
-plagulae derivation** (bench tools' lists derived). Amalgamare
lists deferred.

**Q10. Verification bar?**
A: **Whole-corpus gate.** Emit targeted scripts for EVERY probatio,
run all, all green + differentia-clausurae CONSENSUS vs -MM across
the corpus. Undercounts fail loud at link; overcounts show in the
differentia.

**Q11. Invocation?**
A: **Artifacts in build/aedilis/<basis>/ + flags.** Prints manifest
+ script paths; `-currere` executes, `-scribere <via>` saves the
script permanently, `-machina`/`-posix` pick platform (examen's
vocabulary). Repeat runs hit the graph cache.

**Q12. compile_tests.sh?**
A: **Untouched — additive only.** Suite adoption is a later decision
made with aedilis's track record in hand.

## Round 4 — edges

**Q13. Annotation grammar (capsula-extern class: linked object no
include reveals)?**
A: **`/* aedilis: <verbum> ... */`** — v1 verbs `obiectum <via>` and
`vexillum <flag>`. Aedilis already has the file's comments from its
silva parse — no second scanner. The generare: system stays separate
(it EXECUTES; aedilis directives DECLARE).

**Q14. Emitted script default?**
A: **Cache-reusing default + `-solitarius` flag.** Default reuses
build/'s .o cache (mtime compile-if-stale, then link); -solitarius
emits the hermetic compile-everything variant (the save/gift form).
Both from one manifest.

**Q15. Failure posture (silva can't parse AND -MM lane empty)?**
A: **Refuse loud.** No manifest, no script, named cause. Aedilis's
value is derived precision; a silently-fat fallback is the
link-everything world in a costume. (The verbose-superset fallback
stays right for STALENESS manifests — over-watching is safe;
build generation is not that.)
