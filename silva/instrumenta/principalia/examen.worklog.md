# examen — worklog

## 2026-09-25 — located syntax errors and excerpts (silva-migratio T15b)

Asked by Fran after T15 ("do we have this nice printing for examen?"):
no. Syntax failures were a COUNT with line 0 ("nodi erroris (syntaxis) 1"
/ "\t0\t0\t" in -machina) — and examen is what the post-edit hook
(.claude/hooks/examen-custos.sh) runs, so every agent editing C here was
told "syntax broke" and never where. Semantic findings had a location
but no source line.

Now: each GLR death point (T15, `silva_mortes_diagnostica` through the
frontend's hook — examen already links all of silva/fontes and the
materia substrate) is its own row with the real line/column and cause;
the human mode prints the two-location excerpt ("hic coepit" / "hic
exspectatur") via lib/excerptum, the same printer tools/diagnostica
uses; semantic findings in the judged file get their source line and
caret (byte offset derived from line + column over the judged text;
findings inside included headers get none — their text isn't at hand).
-machina keeps its row shape, so the hook relays the location with no
hook change. A residual summary row ("nodi erroris (syntaxis) N")
remains for ERROR nodes without a located death in this file (headers,
apparatus).

**examen_vectis.sh was NOT a registered gate.** Not in pythonica's
PORTAE, not in the 'suitae probationum' inventory — examen's whole
differential bar only ran by hand, and no commit ever owed it. Now
'examen-vectis' in PORTAE + inventory row (currit binaria examen.c;
tegit viae examen.sh, examen_vectis.sh, fixa/examinis, silva/fontes,
silva/grammatica). Its new section ④ (6 assertions: syntax first line,
both excerpt carets, -machina line/col/cause, semantic excerpt + caret)
planted red in one plant (death points forced empty + excerpt call
removed): 6/6 FRACTUM, restored TENET. Also fixed in the same
inventory: diagnostica_fumus's 'tegit viae' lacked silva/fontes/* since
T14 made the tool compile silva.

Not done here (desideratum …W87Q): legati's `diagnostica` (officina/
instrumenta/legatus.c) keeps its OWN copy of examen's output format and
is built against the amalgam, where materia's position functions are
internal — it needs a public silva.h surface for death points.
