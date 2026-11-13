# haruspex worklog

## 2026-11-10 — header walk skips build, .git, node_modules (as examen)

Found while converting the copied header walkers to
directorium_fontes_ambulare (lib/iter_directoria.worklog.md): haruspex
was the only one of fifteen with NO skip rule - it skipped dot names
only, so every header under any build/ directory was offered as
context (first of each name wins).

Measured in this tree before the change: 1103 header names offered,
346 of them from a build/ directory - 340 that exist only there (tool
scratch output such as silva/build/delta/basis_*.h, generated capsule
headers) and 6 that shadowed a real header of the same name, all 6
byte-identical to it. So no wrong verdict today; the risk was a STALE
copy under build/ being met first and layouts being asserted against
the old shape.

Change (Fran's ruling): the same three names examen uses. Whole-corpus
differential, `./silva/haruspex.sh -v` before and after:
- headers offered 1103 -> 763;
- plagulae 955, TU emissae 625, assertiones 17417, praetermissa
  2 / 149 / 0: identical;
- all 625 generated assertion files (silva/build/exta/*.c)
  byte-identical;
- clang: 625 compiled, 0 dissenting, both times.
The only output line that differs is `capita: N praebita`. No file
needed a header that lives only under build/.
