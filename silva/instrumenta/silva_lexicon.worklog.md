# silva_lexicon worklog

## 2026-10-21 - quoted `postulata_*.h` includes evoke sections (sonus S3, sexta)

`_inclusa_colligere` collected only `#include <...>` names (quoted
includes are resolved by the repository header walk). It now also
collects quoted includes whose name begins `postulata_` - requirement
headers that name a COMPILER surface (first: `postulata_atomica.h`, the
`__atomic` builtins; see silva/fontes/systema_posix.worklog.md same
date). Only the judged file's OWN include lines count (not transitive),
so the surface stays visible to exactly the file that asks for it.

First direct test of the derivation: `silva/probationes/
probatio_silva_lexicon.c` (synthetic lexicon: semper, caput angled,
poscit transitive, quoted postulata evokes, quoted house header does
NOT evoke a same-named section; plus the real systema_posix.h section).
Plants (silva.planta, porta silva): accept all quoted includes -> red
("domus.h" evoked DOMUS_FUNCTIO); accept none -> red.
