/* GENERATUM a canon_coquere e silva/quaestiones.canon - NOLI MANU MUTARE.
 * (signum generata-custodis: editio manualis vetatur)
 * Lex folii: involve, numquam emenda - regeneratio
 * emendationes vincit. Lectio sola; plagulam canone
 * sanam praesumit (ignota tacite praeterit).
 * Regeneratio: bin/canon_coquere silva/quaestiones.canon -praefixum Qs -caput include/quaestiones_lectio.h -corpus lib/quaestiones_lectio.c */
#ifndef QUAESTIONES_LECTIO_H
#define QUAESTIONES_LECTIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"

nomen structura QsQuaestiones QsQuaestiones;
nomen structura QsQuaestio QsQuaestio;
nomen structura QsSelector QsSelector;
nomen structura QsCausa QsCausa;
nomen structura QsParametrum QsParametrum;

nomen enumeratio {
    QS_QUAESTIO_GRADUS_ABSENS = ZEPHYRUM,
    QS_QUAESTIO_GRADUS_OCULI,
    QS_QUAESTIO_GRADUS_INVARIANS
} QsQuaestioGradus;

structura QsQuaestiones {
    StmlNodus*  nodus;
    Xar*        quaestio;   /* QsQuaestio* */
};

structura QsQuaestio {
    StmlNodus*  nodus;
    chorda*     titulus;
    QsQuaestioGradus gradus;
    QsSelector* selector;
    QsCausa* causa;
    Xar*        parametrum;   /* QsParametrum* */
};

structura QsSelector {
    StmlNodus*  nodus;
    chorda      textus;
};

structura QsCausa {
    StmlNodus*  nodus;
    chorda      textus;
};

structura QsParametrum {
    StmlNodus*  nodus;
    chorda*     titulus;
};

QsQuaestiones* qs_quaestiones_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
QsQuaestiones* qs_quaestiones_legere(
    chorda fons, Piscina* piscina,
    InternamentumChorda* intern, chorda* causa);
QsQuaestio* qs_quaestio_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
QsSelector* qs_selector_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
QsCausa* qs_causa_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
QsParametrum* qs_parametrum_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);

#endif /* QUAESTIONES_LECTIO_H */
