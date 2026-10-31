
## 2026-10-31 — newest header once; no basename forks in wrapper preambles (budget B2 H4)

Every silva wrapper call paid two fixed costs before its binary ran:
1. 26-38 `$(basename "$src" .c)` subshells in the `silva/fontes/*.c`
   loop. Now `base="${src##*/}"; base="${base%.c}"`, the same string with
   no fork, in 31 scripts: 20 silva wrappers, officina cursor/fusor/
   interpretare/colloquium/compile_probationes, tessera, oratio, briar,
   ludus_tessera runners, tools/silex_struere.sh, tools/stml_struere.sh.
2. This file checked each of 7 materia objects against every header in
   include/ and materia/fontes (~300 `-nt` each). Now the newest header
   is found once per call: "object newer than every header ⟺ newer
   than the newest", ties still recompile. With no headers, the glob
   literal doesn't exist and `-nt` is true, so the object stays fresh
   as before.
Old vs new copies (lib/piscina.c, load 5): output IDENTICAL; formator
0.137 → 0.083 s, examen 0.205 → 0.166 s per call. That applies to every
commit hook in every worktree, plus pythonica and the nexus and unci
gates.

No gate ever touched a materia header. New stage VIII in
tools/instrumenta_nexus_fumus.sh (tegit gains materia_substratum.sh):
touch a materia/fontes header (mtime restored) → examen reports
[materia] recompiles. Plant (oldest header, `-ot`) → 0 recompiles,
FRACTUM.
