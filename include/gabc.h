/* gabc.h - lector gabc (Gregorio): textus cantus Gregoriani ->
 * exemplar notationis plenae
 *
 * Spec: project-specs/gabc-spec.md (decreta G1-G18). Stratum PRIMUM:
 * carmen.h nescit, pixela nescit (G1). Conversor gabc_carmen.h inde
 * sonum facit, tabula antiphonale (postea) inde pingit; neuter textum
 * fontis iterum legit - exemplar notationem TOTAM servat (G2, G3):
 * figuras et coniunctiones notarum, signa rhythmica, claves,
 * divisiones, spatia, lineas fractas, custodes, textum ornatum.
 *
 * USUS:
 *   GabcCantus* c = gabc_legere(octeti, piscina);
 *   si (!gabc_successus(c))
 *       gabc_vitia_scribere(c, "salve.gabc", aedificator);
 *   per (s = ZEPHYRUM; s < c->numerus_syllabarum; s++)
 *       per (e = ZEPHYRUM; e < c->syllabae[s].numerus_elementorum; e++)
 *       {
 *           constans GabcElementum* el = &c->syllabae[s].elementa[e];
 *           si (el->genus == GABC_EL_NEUMA)
 *               ... c->notae[el->prima .. el->prima + el->numerus) ...
 *               ... gabc_figura(c, el) ...
 *       }
 *   gabc_tabulam_scribere(c, aedificator);        (probationes)
 *
 * FORMA FONTIS: caput "clavis: valor;" (valor per lineas plures: ';'
 * omissum, ';;' claudit) usque ad lineam '%%'; deinde corpus
 * "textus(notae) textus(notae) ...". Sine '%%' totum corpus putatur
 * (monitum GABC_SINE_CAPITE) - fragmenta GregoBase et a Claudio
 * scripta saepe caput non habent. '%' commentarium usque ad finem
 * lineae. GRAMMATICA = lexicon Gregorii
 * (src/gabc/gabc-notes-determination.l, lectum 2026-11-03), non
 * solum documenta: quod ille legit, nos legimus.
 *
 * NIHIL TACITE OMITTITUR (G5): constructio non exemplata (uncinorum
 * signa [cs:] [ob:] [oh:] [ll:] [nv:] [nm1] ..., nabc post '|',
 * polyphonia '{' '}', uncini duplices '[[' ']]') -> GABC_EL_SIGNUM_OPACUM
 * cum textu fontis et nota cui adhaeret. Quod legi non potest ->
 * vitium locatum, et lectio cum nota aut syllaba proxima pergit.
 *
 * TOTALITAS (G6): octeti quilibet -> cantus (NIHIL solum si memoria
 * deficit) cum vitiis locatis; numquam ruina, numquam mora infinita.
 * VITIUM = pars fontis amissa aut non intellecta; MONITUM = lectum,
 * sed suspectum. BOM UTF-8 et CRLF nihil monent.
 *
 * ALTITUDO (G9): positio = locus in systemate, littera a..n et p (0
 * = a ... 13 = n, 14 = p; 'o' oriscus est, non positio). gradus =
 * gradus diatonicus a DO (0 = do, 6 = si, 7 = do superius, -1 = si
 * inferius): clavis c in linea N do in ea ponit, clavis f fa (do =
 * fa - III). alteratio -1/0/+1 ex signis x / y / # quae ante notam
 * in eadem positione stant; AMBITUS signi: usque ad finem verbi aut
 * divisionem proximam aut signum aliud in eadem positione (regula
 * Solesmensis, Liber Usualis; Gregorius ipse ambitum non definit).
 * Clavis 'cb'/'fb' si per totum systema bemollem facit, usque ad
 * clavem proximam. Musica ficta (r6 r7 r8) notam SOLAM alterat.
 * Altitudo absoluta et rhythmus HIC NON SUNT (G8, G9): conversor
 * iudicat.
 *
 * DATUM NON COPIATUR: chordae exemplaris in octetos fontis monstrant
 * ubi possunt; fons in piscina vivere debet dum cantus vivit.
 */

#ifndef GABC_H
#define GABC_H

/* <aedilis corpus="lib/gabc.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"


/* ==================================================
 * Vitia
 * ================================================== */

nomen enumeratio {
    GABC_VITIUM,
    GABC_MONITUM
} GabcGravitas;

nomen enumeratio {
    GABC_SINE_CAPITE,            /* monitum: '%%' deest, totum corpus */
    GABC_SEPARATOR_ITERATUS,     /* monitum: '%%' secundum (textus) */
    GABC_CAPUT_MALUM,            /* linea capitis sine ':' aut valor
                                  * non clausus */
    GABC_PARENTHESIS_APERTA,     /* vitium: '(' sine ')' */
    GABC_NOTA_IGNOTA,            /* vitium: character in notis ignotus
                                  * (praeteritur) */
    GABC_SIGNUM_APERTUM,         /* vitium: '[' sine ']', signum textus
                                  * (b, i ...) non clausum */
    GABC_SIGNUM_IGNOTUM,         /* monitum: signum textus aut '[x:]' Gregorio
                                  * ignotum (opacum servatur) */
    GABC_ALTERATIO_SINE_POSITIONE, /* vitium: x y # sine littera ante */
    GABC_DIVISIO_IN_NEUMA,       /* monitum: divisio intra notas
                                  * coniunctas */
    GABC_CLAVIS_MALA,            /* vitium: linea clavis extra
                                  * systema */
    GABC_POSITIO_EXTRA,          /* monitum: positio supra systema
                                  * (lineae capitis) */
    GABC_NON_UTF8                /* monitum: textus non UTF-8 (octeti
                                  * servantur) */
} GabcVitiumGenus;

nomen structura {
       GabcGravitas gravitas;
    GabcVitiumGenus genus;
                i32 linea;       /* I-basata */
                i32 columna;     /* I-basata, in octetis */
             chorda causa;       /* Latine */
} GabcVitium;


/* ==================================================
 * Caput
 * ================================================== */

nomen structura {
    chorda clavis;               /* ut scripta ("name", "mode") */
    chorda valor;                /* sine spatiis extremis */
       i32 linea;
} GabcCaput;


/* ==================================================
 * Notae
 * ================================================== */

/* formae notae (GabcNota.formae, bits) */
#define GABC_FORMA_INCLINATUM        0x00001 /* littera maiuscula */
#define GABC_FORMA_VIRGA             0x00002 /* v */
#define GABC_FORMA_VIRGA_REVERSA     0x00004 /* V */
#define GABC_FORMA_STROPHA           0x00008 /* s */
#define GABC_FORMA_ORISCUS           0x00010 /* o */
#define GABC_FORMA_ORISCUS_SCAPUS    0x00020 /* O */
#define GABC_FORMA_QUILISMA          0x00040 /* w */
#define GABC_FORMA_QUILISMA_QUADRATUM 0x00080 /* W */
#define GABC_FORMA_CAVUM             0x00100 /* r, r0 */
#define GABC_FORMA_LINEA_PUNCTUM     0x00200 /* R, r0 */
#define GABC_FORMA_LINEA             0x00400 /* = */
#define GABC_FORMA_QUADRATUM         0x00800 /* q */
#define GABC_FORMA_LIQUESCENS        0x01000 /* ~ deminuta */
#define GABC_FORMA_AUCTA_ASCENDENS   0x02000 /* < */
#define GABC_FORMA_AUCTA_DESCENDENS  0x04000 /* > */
#define GABC_FORMA_INITIO_DEBILIS    0x08000 /* - ante litteram */
#define GABC_FORMA_FUSIO             0x10000 /* @ ante litteram */

/* signa supra notam (GabcNota.signa, bits) */
#define GABC_SIGNUM_ACCENTUS         0x01    /* r1 */
#define GABC_SIGNUM_ACCENTUS_REVERSUS 0x02   /* r2 */
#define GABC_SIGNUM_CIRCULUS         0x04    /* r3 */
#define GABC_SIGNUM_SEMICIRCULUS     0x08    /* r4 */
#define GABC_SIGNUM_SEMICIRCULUS_REVERSUS 0x10 /* r5 */
#define GABC_SIGNUM_FICTA            0x20    /* r6 r7 r8 (alteratio
                                              * notae solius) */

/* episema horizontale (GabcNota.episema, bits): ADEST, deinde unum
 * bit per numerum scriptum 0..5 (_0 infra, _1 supra, _2 non iungere,
 * _3.._5 breve ad sinistrum, medium, dextrum) */
#define GABC_EPISEMA_ADEST           0x01
#define GABC_EPISEMA_NUMERUS(n)      (0x02 << (n))

nomen structura {
     i8 positio;                 /* 0 = a ... 13 = n, 14 = p */
    s32 gradus;                  /* gradus a do (clavi resolutus) */
    s32 alteratio;               /* -1 bemolle, 0, +1 diesis */
    i32 formae;                  /* GABC_FORMA_* */
     s8 directio;                /* G0 G1 G2, o0 o1: numerus; -1 =
                                  * Gregorius eligit */
     i8 mora;                    /* puncta: 0, 1 ('.'), 2 ('..') */
     s8 mora_locus;              /* .0 infra, .1 supra, -1 */
     i8 episema;                 /* GABC_EPISEMA_* */
     i8 ictus;                   /* episema verticale ''' : 0/1 */
     s8 ictus_locus;             /* '0 infra, '1 supra, -1 */
     i8 signa;                   /* GABC_SIGNUM_* */
    i32 linea;
    i32 columna;                 /* litterae positionis */
} GabcNota;


/* ==================================================
 * Elementa syllabae
 * ================================================== */

nomen enumeratio {
    GABC_EL_NEUMA,               /* notae coniunctae [prima,
                                  * prima+numerus) - glypha una */
    GABC_EL_ALTERATIO,           /* signum x y # in systemate */
    GABC_EL_CLAVIS,
    GABC_EL_DIVISIO,
    GABC_EL_SPATIUM,             /* inter neumas eiusdem syllabae */
    GABC_EL_FRACTIO,             /* finis lineae z Z */
    GABC_EL_CUSTOS,              /* z0 (automaticus) aut g+ */
    GABC_EL_SIGNUM_OPACUM        /* G5: textus fontis servatus */
} GabcElementiGenus;

nomen enumeratio {
    GABC_DIVISIO_VIRGULA,        /* ` */
    GABC_DIVISIO_MINIMIS,        /* ^ */
    GABC_DIVISIO_MINIMA,         /* , */
    GABC_DIVISIO_MINOR,          /* ; */
    GABC_DIVISIO_MAIOR,          /* : */
    GABC_DIVISIO_MAIOR_PUNCTATA, /* :? */
    GABC_DIVISIO_FINALIS,        /* :: */
    GABC_DIVISIO_DOMINICANA      /* ,1-8 ;1-8 (numerus) */
} GabcDivisio;

nomen enumeratio {
    GABC_SPATIUM_NULLUM,         /* ! - nulla coniunctio */
    GABC_SPATIUM_DIMIDIUM,       /* /0 */
    GABC_SPATIUM_NEUMATICUM,     /* / */
    GABC_SPATIUM_MAGNUM,         /* // */
    GABC_SPATIUM_GLYPHAE,        /* ' ' intra parentheses */
    GABC_SPATIUM_FACTOR,         /* /[n] (factor in textu) */
    GABC_SPATIUM_INSEPARABILE    /* /! */
} GabcSpatium;

nomen enumeratio {
    GABC_ALTERATIO_BEMOLLE,      /* x */
    GABC_ALTERATIO_NATURALE,     /* y */
    GABC_ALTERATIO_DIESIS        /* # */
} GabcAlteratioGenus;

nomen structura {
    GabcElementiGenus genus;
                  i32 linea;
                  i32 columna;

    /* NEUMA */
                  i32 prima;     /* index in GabcCantus.notae */
                  i32 numerus;

    /* ALTERATIO */
   GabcAlteratioGenus alteratio;
                  b32 molle;     /* X Y: signum editoris (minus) */
                  b32 parenthesis; /* x? y? #?; DIVISIO: `? ,? */

    /* CLAVIS */
            character clavis;    /* 'c' aut 'f' */
                   i8 clavis_linea; /* I.. lineae systematis */
                  b32 clavis_bemolle; /* cb3 */
                  b32 clavis_secunda; /* @c4: altera supra priorem */

    /* DIVISIO */
          GabcDivisio divisio;
                   i8 divisio_numerus; /* dominicana: 1..8 */
            character divisio_signum;  /* dominicana: ',' aut ';' */
                  b32 alta;      /* `0 ^0 ,0 */

    /* SPATIUM */
          GabcSpatium spatium;
               chorda factor;    /* /[n]: n ut scriptum */

    /* FRACTIO */
                  b32 iustificata; /* z (VERUM), Z (FALSUM) */
                  s32 custos;    /* z+ 1, z- -1, aliter 0 */

    /* ALTERATIO, CUSTOS: positio; CUSTOS automaticus (z0) -1 */
                  s32 positio;

    /* SIGNUM_OPACUM */
               chorda textus;    /* ut in fonte ("[oh:h]", "{") */
                  s32 nota;      /* nota cui adhaeret; -1 nulla */
} GabcElementum;


/* ==================================================
 * Textus et syllabae
 * ================================================== */

/* stilus textus (GabcTextus.stilus, bits). "signum i" = signum textus
 * gabc i (uncinis angularibus); uncini in commentariis vitantur, ne
 * annotationes STML putentur. */
#define GABC_STILUS_ITALICUS         0x001   /* signum i */
#define GABC_STILUS_CRASSUS          0x002   /* signum b */
#define GABC_STILUS_SUBLINEATUS      0x004   /* signum ul */
#define GABC_STILUS_COLORATUS        0x008   /* signum c */
#define GABC_STILUS_CAPITELLA        0x010   /* signum sc */
#define GABC_STILUS_TELETYPUS        0x020   /* signum tt */
#define GABC_STILUS_ELISIO           0x040   /* signum e */
#define GABC_STILUS_PROTRUSIO        0x080   /* signum pr */

nomen enumeratio {
    GABC_TEXTUS_CANTATUS,        /* sub notis */
    GABC_TEXTUS_TRANSLATIO,      /* [...] ('/' = finis ambitus) */
    GABC_TEXTUS_SUPRA,           /* signum alt */
    GABC_TEXTUS_VERBATIM         /* signum v (TeX, crudum) */
} GabcTextusGenus;

nomen structura {
    GabcTextusGenus genus;
             chorda textus;      /* UTF-8 ut ostenditur (sp R/ ->
                                  * -> U+211F) */
                i32 stilus;      /* GABC_STILUS_* */
} GabcTextus;

/* signa syllabae (GabcSyllaba.signa, bits) */
#define GABC_SYLLABA_NLBA_INITIUM    0x01    /* signum nlba */
#define GABC_SYLLABA_NLBA_FINIS      0x02    /* signum finis nlba */
#define GABC_SYLLABA_EUOUAE_INITIUM  0x04    /* signum eu */
#define GABC_SYLLABA_EUOUAE_FINIS    0x08    /* signum finis eu */
#define GABC_SYLLABA_PURGA           0x10    /* signum clear */

nomen structura {
        GabcTextus* textus;       /* cursus textus, ordine */
               i32  numerus_textuum;
               b32  verbum_incipit; /* textus adest et (textus primus
                                    * aut spatium post textum
                                    * priorem); sine textu FALSUM */
               s32 vocalis_initium; /* '{' in octetis textus cantati
                                     * coniuncti; -1 = nulla */
               s32  vocalis_finis;
               i32  signa;        /* GABC_SYLLABA_* */
     GabcElementum* elementa;     /* ordine fontis */
               i32  numerus_elementorum;
               i32  linea;
               i32  columna;
} GabcSyllaba;


/* ==================================================
 * Cantus
 * ================================================== */

nomen structura {
       GabcCaput* capita;         /* omnia, ordine */
             i32  numerus_capitum;
          chorda  titulus;        /* name */
          chorda  modus;          /* mode ut scriptus */
             s32  modus_numerus;  /* 1..8; 0 = alius aut nullus */
          chorda  differentia;    /* mode-differentia */
          chorda  annotationes[II]; /* annotation (supra, infra) */
             i32  numerus_annotationum;
             i32  lineae;         /* staff-lines 2..5; ordinarie 4 */
     GabcSyllaba* syllabae;
             i32  numerus_syllabarum;
        GabcNota* notae;         /* omnes, ordine */
             i32  numerus_notarum;
      GabcVitium* vitia;
             i32  numerus_vitiorum;
} GabcCantus;


/* ==================================================
 * Figurae neumarum (G4)
 * ================================================== */

nomen enumeratio {
    GABC_FIGURA_IGNOTA,          /* tabula non novit - numquam falsa */
    GABC_FIGURA_PUNCTUM,
    GABC_FIGURA_PUNCTUM_INCLINATUM,
    GABC_FIGURA_VIRGA,
    GABC_FIGURA_STROPHA,
    GABC_FIGURA_ORISCUS,
    GABC_FIGURA_BIVIRGA,
    GABC_FIGURA_TRIVIRGA,
    GABC_FIGURA_DISTROPHA,
    GABC_FIGURA_TRISTROPHA,
    GABC_FIGURA_PES,
    GABC_FIGURA_PES_QUADRATUS,
    GABC_FIGURA_PES_QUILISMATICUS,
    GABC_FIGURA_EPIPHONUS,
    GABC_FIGURA_CLIVIS,
    GABC_FIGURA_CEPHALICUS,
    GABC_FIGURA_TORCULUS,
    GABC_FIGURA_TORCULUS_RESUPINUS,
    GABC_FIGURA_PORRECTUS,
    GABC_FIGURA_PORRECTUS_FLEXUS,
    GABC_FIGURA_SCANDICUS,
    GABC_FIGURA_SALICUS,
    GABC_FIGURA_CLIMACUS,
    GABC_FIGURA_PES_SUBBIPUNCTIS,
    GABC_FIGURA_PRESSUS
} GabcFigura;


/* ==================================================
 * Functiones
 * ================================================== */

/* octeti -> cantus; NIHIL solum si memoria deficit. datum non
 * copiatur: in piscina vivere debet dum cantus vivit. */
GabcCantus*
gabc_legere (
      chorda  datum,
     Piscina* piscina);

/* VERUM si nullum VITIUM (monita licent) */
b32
gabc_successus (
    constans GabcCantus* cantus);

/* 'via:linea:columna: monitum|vitium: causa' per lineam */
b32
gabc_vitia_scribere (
           constans GabcCantus* cantus,
            constans character* via,
             ChordaAedificator* exitus);

/* forma canonica (probationes): caput, syllabae, elementa, notae -
 * linea una per rem, columnae tabulis separatae (grammatica in
 * probatio_gabc.c fixa) */
b32
gabc_tabulam_scribere (
           constans GabcCantus* cantus,
             ChordaAedificator* exitus);

/* figura neumae (G4) ex contorno et formis notarum; elementum non
 * NEUMA -> GABC_FIGURA_IGNOTA */
GabcFigura
gabc_figura (
       constans GabcCantus* cantus,
    constans GabcElementum* neuma);

/* titulus Latinus figurae ("pes", "clivis" ...) */
constans character*
gabc_figurae_titulus (
    GabcFigura figura);

#endif /* GABC_H */
