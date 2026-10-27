/* articulus.h - articulus interretis in forma domus (navigator N1;
 * project-specs/navigator-spec.md). DATA SOLA: adaptatores situum
 * (vicipaedia.h) implent, navigator paginat; nullum rete, nulla UI.
 *
 * TEXTUS: clausula quaeque textum planum suum habet (UTF-8, entitates
 * decoctae, spatia contracta, sine notis referentiarum); notae (nexus,
 * emphasis) = spatia octetorum IN EO TEXTU. Adiuncta = columnae
 * laterales sectioni affixae (infobox; navigator eas ad paginam
 * primam sectionis ponit - spec V.18). */

#ifndef ARTICULUS_H
#define ARTICULUS_H

#include "latina.h"
#include "chorda.h"
#include "xar.h"

nomen enumeratio {
    ARTICULUS_TITULUS = ZEPHYRUM,  /* h2-h6; gradus II..VI */
    ARTICULUS_PARAGRAPHUS,
    ARTICULUS_INDEX,               /* li; gradus = profunditas I.. */
    ARTICULUS_CITATIO,             /* blockquote */
    ARTICULUS_PAR,                 /* clavis / valor (infobox) */
    ARTICULUS_FIGURA               /* imago + titulus (v2) */
} ArticulusGenus;

nomen enumeratio {
    ARTICULUS_NEXUS = ZEPHYRUM,    /* scopus = URL absoluta */
    ARTICULUS_EMPHASIS,            /* i, em */
    ARTICULUS_FORTIS               /* b, strong */
} ArticulusNotaGenus;

nomen structura {
    ArticulusNotaGenus genus;
                   i32 initium;   /* octeti in textu clausulae */
                   i32 finis;     /* exclusivus */
                chorda scopus;    /* nexus: URL; aliter vacua */
} ArticulusNota;

nomen structura {
    ArticulusGenus genus;
               s32 gradus;        /* titulus, index; aliter 0 */
            chorda textus;        /* PAR: valor; FIGURA: titulus */
            chorda clavis;        /* PAR: clavis; FIGURA: URL
                                    * imaginis; aliter vacua */
               Xar* notae;         /* ArticulusNota */
} ArticulusClausula;

nomen structura {
    chorda  titulus;               /* "Nutrition"; vacuum = sine */
       Xar* clausulae;             /* ArticulusClausula */
} ArticulusAdiunctum;

nomen structura {
    chorda  titulus;               /* vacuum = prooemium */
    chorda  ancora;                /* id sectionis ("Etymology") */
       Xar* clausulae;             /* ArticulusClausula */
       Xar* adiuncta;              /* ArticulusAdiunctum */
} ArticulusSectio;

nomen structura {
    chorda  titulus;               /* "Thomas Aquinas" */
    chorda  fons;                  /* URL canonica */
       Xar* sectiones;             /* ArticulusSectio; [0] prooemium */
} Articulus;

#endif /* ARTICULUS_H */
