# probatio_stml_aurea.worklog.md

## 2026-09-23 — the corpus is frozen (54 tracked sources, copied)

**Was.** `tools/stml_aurea_generare.sh` ran `find .` for every
`*.stml`/`*.census`/`*.canon` under 1 MB on the LIVE tree, and the golden
recorded each source's path + size + FNV; the test re-read those live
paths. Snapshot over a live corpus: any edit to any of the 148 sources
turned the root suite red — and 94 of the 148 were NOT TRACKED (80
build outputs in `officina/build/census/`, 14 from the gitignored
`stml-parser-reference/`), so a fresh clone was red until officina had
been built. Quiet in practice only because the tracked sources are
generated/reference files that rarely move (none had changed since the
08-24 capture).

**Now.** The 54 TRACKED sources are copied to
`probationes/fixa/stml_aurea/fontes/<original path>.fons` (the `.fons`
suffix keeps every house walker that finds `*.stml`/`*.canon`/`*.census`
by name away from them; the bridge parses bytes, extension irrelevant).
The generator lists that directory instead of `find .`; the test is
unchanged apart from the golden's banner (`CORPUS CONGELATUM: …` —
first draft said `FONTES CONGELATI`, which collided with the golden's
own `FONTES <n>` section marker and misled my comparison script; the
test only keys on `F ` lines, but a marker that reads two ways is a
trap). Gitignored `stml-parser-reference/` content was deliberately
NOT copied into the repo.

**Verified before accepting the regeneration:** banner line the only
header change; all 463 inline-case lines identical; all 54 surviving
source blocks (29,424 lines) byte-identical to the old golden; 94
untracked sources dropped, nothing new.

**Proofs:** editing live `natura/cocta/acta.canon` → GREEN (the new
property); editing its `.fons` copy → RED, "DIVERGENTIA ad octetum
11253 (linea 659 aurei)"; `officina/build/census` moved away (fresh
clone) → GREEN.

**Refreshing the corpus** is deliberate: re-copy the chosen sources,
run the generator, read the diff, one commit with the cause.
Doctrine: quaestio 01M365VXAF (snapshot needs a frozen input).
