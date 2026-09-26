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

## 2026-09-25 — the system stand-in completes its syntax-bearing macros (silva-migratio T16a)

lapide bugs/009 (`va_arg(va, char*)` a syntax error) was a stand-in gap,
not a grammar gap: `silva/fontes/systema_c89.h` defined `va_list` but no
`va_*` macros (its note deferred them to machinula, which never grew any
`va_*` handling). Audit of the class (macros that take a TYPE or change
data flow): `va_arg`, `offsetof` (syntax errors); `va_start`, `va_end`,
`assert` (implicit calls). All five now defined with valid C89
expansions. Measured side effect: lib/silex.c lost FIVE false suspects —
2 "vocatio implicita" (va_start/va_end) and 3 "variabilis ininitiata
legitur" on `argumenta` (dataflow never saw va_start assign it).

**My own gate bug, caught by the plant.** The first version of the
new sections wrote the fixture with `printf '%s' "...\n..."` — `%s`
does not interpret escapes, so the "file" was ONE line beginning
`#include` (a directive swallowing everything) and passed with or
without the fix. The plant (stand-in at HEAD) stayed green, which is
what exposed it. Fixed to pass the fixture as the format string; the
plant now reds 2/2 (examen-vectis) and 2/3 (diagnostica — the third
assertion, "lexicon read", correctly stays green).
