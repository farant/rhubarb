#ifndef BIBLIA_H
#define BIBLIA_H

#include "latina.h"
#include "chorda.h"

/* ==================================================
 * BIBLIA - Scriptura Sacra in Memoria
 *
 * Bibliotheca pro accedere textum Scripturae Sacrae
 * statice compilatum in programma.
 *
 * EXEMPLUM:
 *   constans Biblia* biblia = biblia_obtinere_dr();
 *   chorda versus = biblia_versus(biblia, LIBER_GENESIS, 1, 1);
 *   // "In the beginning God created heaven, and earth."
 *
 * ================================================== */


/* ==================================================
 * Indices Librorum (Douay-Rheims ordering)
 * ================================================== */

enumeratio {
    /* Vetus Testamentum */
    LIBER_GENESIS = 0,
    LIBER_EXODUS,
    LIBER_LEVITICUS,
    LIBER_NUMERI,
    LIBER_DEUTERONOMIUM,
    LIBER_JOSUE,
    LIBER_JUDICUM,
    LIBER_RUTH,
    LIBER_I_REGUM,      /* 1 Samuel / 1 Kings */
    LIBER_II_REGUM,     /* 2 Samuel / 2 Kings */
    LIBER_III_REGUM,    /* 1 Kings / 3 Kings */
    LIBER_IV_REGUM,     /* 2 Kings / 4 Kings */
    LIBER_I_PARALIPOMENON,  /* 1 Chronicles */
    LIBER_II_PARALIPOMENON, /* 2 Chronicles */
    LIBER_I_ESDRAE,     /* Ezra / 1 Esdras */
    LIBER_II_ESDRAE,    /* Nehemiah / 2 Esdras */
    LIBER_TOBIAE,
    LIBER_JUDITH,
    LIBER_ESTHER,
    LIBER_JOB,
    LIBER_PSALMORUM,
    LIBER_PROVERBIORUM,
    LIBER_ECCLESIASTES,
    LIBER_CANTICUM,
    LIBER_SAPIENTIAE,
    LIBER_ECCLESIASTICUS,  /* Sirach */
    LIBER_ISAIAE,
    LIBER_JEREMIAE,
    LIBER_LAMENTATIONES,
    LIBER_BARUCH,
    LIBER_EZECHIELIS,
    LIBER_DANIELIS,
    LIBER_OSEE,
    LIBER_JOEL,
    LIBER_AMOS,
    LIBER_ABDIAE,
    LIBER_JONAE,
    LIBER_MICHAEAE,
    LIBER_NAHUM,
    LIBER_HABACUC,
    LIBER_SOPHONIAE,
    LIBER_AGGAEI,
    LIBER_ZACHARIAE,
    LIBER_MALACHIAE,
    LIBER_I_MACHABAEORUM,
    LIBER_II_MACHABAEORUM,

    /* Novum Testamentum */
    LIBER_MATTHAEUS,
    LIBER_MARCUS,
    LIBER_LUCAS,
    LIBER_JOANNES,
    LIBER_ACTUS,
    LIBER_ROMANOS,
    LIBER_I_CORINTHIOS,
    LIBER_II_CORINTHIOS,
    LIBER_GALATAS,
    LIBER_EPHESIOS,
    LIBER_PHILIPPENSES,
    LIBER_COLOSSENSES,
    LIBER_I_THESSALONICENSES,
    LIBER_II_THESSALONICENSES,
    LIBER_I_TIMOTHEUM,
    LIBER_II_TIMOTHEUM,
    LIBER_TITUM,
    LIBER_PHILEMONEM,
    LIBER_HEBRAEOS,
    LIBER_JACOBI,
    LIBER_I_PETRI,
    LIBER_II_PETRI,
    LIBER_I_JOANNIS,
    LIBER_II_JOANNIS,
    LIBER_III_JOANNIS,
    LIBER_JUDAE,
    LIBER_APOCALYPSIS,

    LIBRI_NUMERUS
} LiberIndex;


/* ==================================================
 * Typi
 * ================================================== */

/* Versus - singulus versus cum metadata */
nomen structura {
    i32 liber;
    i32 capitulum;
    i32 versus;
    i32 textus_initium;   /* offset in textus totalis */
    i32 textus_longitudo;
} Versus;

/* Capitulum - metadata pro capitulo */
nomen structura {
    i32 versus_numerus;
    i32 versus_initium;   /* index in versus[] array */
} Capitulum;

/* Liber - metadata pro libro */
nomen structura {
    constans character* nomen_latinum;
    constans character* nomen_anglicum;
    constans character* abbreviatio;
    i32 capitula_numerus;
    i32 capitula_initium;  /* index in capitula[] array */
} Liber;

/* Biblia - tota scriptura */
nomen structura {
    constans character* titulus;       /* e.g., "Douay-Rheims" */
    constans character* annus;         /* e.g., "1899" */

    constans character* textus;        /* totus textus concatenatus */
    memoriae_index textus_longitudo;

    i32 libri_numerus;
    constans Liber* libri;

    i32 capitula_numerus;
    constans Capitulum* capitula;

    i32 versus_numerus;
    constans Versus* versus;
} Biblia;


/* ==================================================
 * Accessores
 * ================================================== */

/* Obtinere Douay-Rheims Bible
 *
 * Reddit: pointer ad structuram Biblia (statice allocata)
 */
constans Biblia*
biblia_obtinere_dr(vacuum);

/* Obtinere textum versus
 *
 * biblia: pointer ad Biblia
 * liber: index libri (LIBER_GENESIS, etc.)
 * capitulum: numerus capituli (1-indexed)
 * versus_num: numerus versus (1-indexed)
 *
 * Reddit: chorda cum textu versus, vel chorda vacua si non invenitur
 */
chorda
biblia_versus(
    constans Biblia* biblia,
    i32 liber,
    i32 capitulum,
    i32 versus_num);

/* Obtinere nomen libri
 *
 * biblia: pointer ad Biblia
 * liber: index libri
 *
 * Reddit: nomen anglicum libri
 */
constans character*
biblia_nomen_libri(
    constans Biblia* biblia,
    i32 liber);

/* Obtinere numerum capitulorum in libro
 *
 * biblia: pointer ad Biblia
 * liber: index libri
 *
 * Reddit: numerus capitulorum
 */
i32
biblia_capitula_in_libro(
    constans Biblia* biblia,
    i32 liber);

/* Obtinere numerum versuum in capitulo
 *
 * biblia: pointer ad Biblia
 * liber: index libri
 * capitulum: numerus capituli (1-indexed)
 *
 * Reddit: numerus versuum
 */
i32
biblia_versus_in_capitulo(
    constans Biblia* biblia,
    i32 liber,
    i32 capitulum);


#endif /* BIBLIA_H */
