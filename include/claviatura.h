/* claviatura.h - servitium claviaturae systematis (communio-plan-5
 * T7/T9): applicatio campum suum ipsa pingit; testa telephoni
 * claviaturam ostendit (genus: epistula, codex) aut celat, et litteras
 * ut EVENTUS_TEXTUS, RETRORSUM, REDITUS reddit. macOS: NIHIL (claviatura
 * physica). Caput solum - nihil ligat. */
#ifndef CLAVIATURA_H
#define CLAVIATURA_H

#include "latina.h"

nomen enumeratio {
    TESTA_CLAVIATURA_NULLA = ZEPHYRUM,   /* celare */
    TESTA_CLAVIATURA_EPISTULA,           /* inscriptio epistulae */
    TESTA_CLAVIATURA_CODEX               /* codex: maiusculae, ASCII */
} TestaClaviaturaGenus;

/* genus = TestaClaviaturaGenus */
nomen vacuum (*TestaClaviaturamPonere)(vacuum* ctx, i32 genus);

#endif /* CLAVIATURA_H */
