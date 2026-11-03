/* musicxml.h - lector MusicXML: documentum (.xml, .musicxml, .mxl)
 * -> exemplar partiturae neutrum
 *
 * Spec: project-specs/musicxml-spec.md (decreta M1-M15). Stratum
 * PRIMUM: carmen.h nescit (M1) - conversor musicxml_carmen.h solus
 * utrumque novit; pagina notationis aut scriptor MusicXML hoc
 * exemplar sine conversore adhibent.
 *
 * USUS:
 *   MusicxmlOpus* o = musicxml_legere(octeti, piscina);
 *   si (!musicxml_successus(o))
 *       musicxml_vitia_scribere(o, NIHIL, "a.mxl", aedificator);
 *   per (p = ZEPHYRUM; p < o->numerus_partium; p++)
 *       per (m = ZEPHYRUM; m < o->partes[p].numerus_mensurarum; m++)
 *           ... o->partes[p].mensurae[m].notae ...
 *   ordo = musicxml_ordo(o, piscina);         (repetitiones evolutae)
 *   musicxml_tabulam_scribere(o, ordo, aedificator);  (oraculum)
 *
 * OCTETI, NON VIA (M9): vocans filum legit. Arca ZIP ('PK\3\4') =
 * .mxl: META-INF/container.xml rootfile primus per sarcina.h; aliter
 * XML per xml_documentum.h. Solum UTF-8 (M13).
 *
 * TEMPUS EXACTUM (M4): unitas = divisio OPERIS, minimus communis
 * multiplex omnium <divisions> (per quartam) - omnis positio et
 * longitudo integer s64 in ea, nihil rotundatur. Multiplex ultra
 * 2^31 = vitium MUSICXML_DIVISIONES_NIMIAE. Lector NUMQUAM
 * approximat (M5): conversor iudicat quid carmen dicere possit.
 *
 * LINEA TEMPORIS COMMUNIS (M15): mensura k in omnibus partibus eodem
 * initio incipit - initium[k+1] = initium[k] + maxima longitudo
 * mensurae k inter partes. Pars brevior monitum accipit; notae
 * numquam moventur. Positio notae et directionis INTRA mensuram.
 *
 * QUOD LEGITUR (M2): quod sonat aut canitur - altitudo (orthographia
 * scripta et sonans derivata), longitudines, pausae, acorda, voces
 * (<backup>/<forward>), baculi, ligaturae, tupli, gratiae, notae
 * parvae (cue), verba (versus omnes), armatura, metrum, clavicula,
 * transpositio, tempus, dynamica, cunei, fermatae, articulationes,
 * ornamenta, arcus (ut signa), verba directionum, repetitiones,
 * voltae, saltus, percussio, instrumenta. PRAETERITUR: dispositio
 * paginae (<print>, <defaults>, positiones, caudae notarum, trabes,
 * fontes, colores, <credit> praeter titulum et auctores).
 *
 * TOTALITAS (M10): octeti quilibet -> opus (NIHIL solum si memoria
 * deficit) cum vitiis locatis; numquam ruina, numquam mora infinita.
 * VITIUM = nullum exemplar utile (non MusicXML, XML malum, partes
 * nullae); MONITUM = exemplar redditum, aliquid reparatum.
 *
 * NOMINA: clavis = armatura (key, ut carmen.h); clavicula = clef
 * (signum G/F/C in systemate) - ne duo 'claves' confundantur.
 * Valores enumerati MusicXML (syllabic, wedge, ...) enumerationes C
 * cum membro _IGNOTUM et textu ut scripto.
 */

#ifndef MUSICXML_H
#define MUSICXML_H

/* <aedilis corpus="lib/musicxml_ordo.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"


/* ==================================================
 * Vitia
 * ================================================== */

nomen enumeratio {
    MUSICXML_VITIUM,
    MUSICXML_MONITUM
} MusicxmlGravitas;

nomen enumeratio {
    MUSICXML_NON_MUSICXML,        /* radix nec score-partwise nec
                                   * score-timewise */
    MUSICXML_XML_MALUM,           /* non bene formatum (causa = xml) */
    MUSICXML_SINE_PARTIBUS,
    MUSICXML_OPUS_RADIX,          /* radix <opus>: non legitur */
    MUSICXML_ARCA_MALA,           /* arca aut container.xml mala */
    MUSICXML_NON_UTF8,         /* non UTF-8 (M13) */
    MUSICXML_DIVISIONES_NIMIAE,   /* multiplex > 2^31 (M4) */
    MUSICXML_FILIUS_DEEST,        /* deest <duration>, <step> ... */
    MUSICXML_NUMERUS_MALUS,       /* numerus illegibilis aut extra
                                   * fines */
    MUSICXML_VALOR_IGNOTUS,       /* valor enumeratus ignotus (textus
                                   * servatur) */
    MUSICXML_REGRESSUS_NIMIUS,    /* regressus <backup> ante initium
                                   * mensurae (coercetur) */
    MUSICXML_LIGATURA_PENDENS,    /* tie start sine stop aut contra */
    MUSICXML_ACORDUM_PRIMUM,      /* chord in nota prima mensurae */
    MUSICXML_MENSURA_BREVIOR,     /* pars mensuram breviorem habet
                                   * (M15) */
    MUSICXML_PARS_DISCORS,        /* index partium et <part>
                                   * discordant */
    MUSICXML_REPETITIO_APERTA,    /* structura repetitionis non
                                   * clauditur (ordo) */
    MUSICXML_SALTUS_SINE_SONO,    /* D.C./D.S. verbis solis (ordo) */
    MUSICXML_SALTUS_CIRCULUS      /* saltus sine fine secatur (ordo) */
} MusicxmlVitiumGenus;

nomen structura {
    MusicxmlGravitas gravitas;
 MusicxmlVitiumGenus genus;
                 i32 linea;      /* I-basata; 0 = sine sede (arca) */
                 i32 columna;
              chorda causa;      /* Latine; nomen introitus in arca */
} MusicxmlVitium;


/* ==================================================
 * Attributa: armatura, metrum, clavicula, transpositio
 * ================================================== */

nomen structura {
      b32  tradita;          /* FALSUM = key-step/key-alter */
      s32  quintae;          /* -7..7 (tradita) */
   chorda  modus;            /* ut scriptus ("major", "dorian", "") */
character* litterae;        /* non tradita: gradus 'A'..'G' */
      f64* alterationes;     /* non tradita: alteratio cuiusque */
      i32  numerus_graduum;
} MusicxmlClavis;

nomen structura {
  chorda pulsus;           /* beats ut scriptum ("3+2") */
     i32 summa;            /* summa pulsuum (5) */
     i32 unitas;           /* beat-type (8) */
} MusicxmlMetriPars;

nomen structura {
               b32  liberum;     /* senza-misura */
 MusicxmlMetriPars* partes;      /* plures = metrum compositum */
               i32  numerus_partium;
            chorda  symbolum;    /* "common", "cut", "" */
} MusicxmlMetrum;

nomen structura {
          i32 baculum;          /* I-basatum */
    character signum;           /* 'G' 'F' 'C', 'P' percussio, 'T'
                                  * tabulatura, 'N' nulla */
          i32 linea;            /* linea systematis (0 = ordinaria) */
          s32 octava_mutata;    /* clef-octave-change */
} MusicxmlClavicula;

nomen structura {
    s32 diatonica;
    s32 chromatica;
    s32 octava_mutata;
    b32 duplicata;              /* octava inferior addita */
} MusicxmlTranspositio;

/* mutationes ad positionem; campus absens = '*_adest' FALSUM */
nomen structura {
                 s64  positio;           /* intra mensuram */
                 s64  divisiones;        /* scriptae; 0 = nullae */
                 b32  clavis_adest;
      MusicxmlClavis  clavis;
                 b32  metrum_adest;
      MusicxmlMetrum  metrum;
   MusicxmlClavicula* claviculae;
                 i32  numerus_clavicularum;
                 b32  transpositio_adest;
MusicxmlTranspositio  transpositio;
                 i32  baculi;            /* staves; 0 = idem */
} MusicxmlAttributa;


/* ==================================================
 * Lineae mensurae (barline), directiones
 * ================================================== */

nomen enumeratio {
    MUSICXML_LOCUS_DEXTER,          /* ordinarius */
    MUSICXML_LOCUS_SINISTER,
    MUSICXML_LOCUS_MEDIUS
} MusicxmlLocusLineae;

nomen enumeratio {
    MUSICXML_REPETITIO_NULLA,
    MUSICXML_REPETITIO_PRORSUM,   /* forward: initium */
    MUSICXML_REPETITIO_RETRORSUM    /* backward: reditus */
} MusicxmlRepetitio;

nomen enumeratio {
    MUSICXML_CONCLUSIO_NULLA,
    MUSICXML_CONCLUSIO_INITIUM,
    MUSICXML_CONCLUSIO_FINIS,
    MUSICXML_CONCLUSIO_INTERRUPTA       /* discontinue */
} MusicxmlConclusio;

nomen structura {
MusicxmlLocusLineae  locus;
             chorda  stilus;       /* bar-style ut scriptum */
  MusicxmlRepetitio  repetitio;
                i32  vices;        /* times; 0 = non scriptum */
  MusicxmlConclusio  conclusio;    /* ending */
             chorda  conclusionum_textus;  /* "1, 2" ut scriptum */
                i32* conclusiones;         /* numeri lecti */
                i32  numerus_conclusionum;
                b32  corona;               /* fermata */
                i32  linea;
                i32  columna;
} MusicxmlLineaMensurae;

nomen enumeratio {
    MUSICXML_DIRECTIO_TEMPUS,       /* sound tempo, metronome */
    MUSICXML_DIRECTIO_DYNAMICA,     /* textus = nota ("mf", "sfz") */
    MUSICXML_DIRECTIO_CUNEUS,       /* wedge */
    MUSICXML_DIRECTIO_VERBA,        /* words */
    MUSICXML_DIRECTIO_EXERCITIUM,   /* rehearsal */
    MUSICXML_DIRECTIO_SIGNUM,       /* segno */
    MUSICXML_DIRECTIO_CAUDA,        /* coda */
    MUSICXML_DIRECTIO_FINIS,        /* fine */
    MUSICXML_DIRECTIO_A_CAPITE,     /* da capo */
    MUSICXML_DIRECTIO_A_SIGNO,      /* dal segno */
    MUSICXML_DIRECTIO_AD_CAUDAM,    /* to coda */
    MUSICXML_DIRECTIO_PEDALE,
    MUSICXML_DIRECTIO_OCTAVA,       /* octave-shift */
    MUSICXML_DIRECTIO_ALIA          /* other-direction et cetera */
} MusicxmlDirectioGenus;

nomen enumeratio {
    MUSICXML_CUNEUS_NULLUS,
    MUSICXML_CUNEUS_CRESCENDO,
    MUSICXML_CUNEUS_DIMINUENDO,
    MUSICXML_CUNEUS_FINIS,
    MUSICXML_CUNEUS_IGNOTUS
} MusicxmlCuneus;

nomen structura {
 MusicxmlDirectioGenus genus;
                   s64 positio;     /* intra mensuram, <offset>
                                      * sound="yes" inclusa */
                chorda vox;         /* "" = omnes */
                   i32 baculum;
                chorda textus;      /* verba, nota dynamicae,
                                      * exercitium, id segno/coda */
                   b32 valor_adest;
                   f64 valor;       /* tempus: quartae per minutum;
                                      * dynamica: <sound dynamics> */
        MusicxmlCuneus cuneus;
                   i32 numerus;     /* cunei, octavae (number) */
                   b32 sonus;       /* ex <sound> (saltus intellectus)
                                      * - FALSUM = verbis solis */
                   i32 linea;
                   i32 columna;
} MusicxmlDirectio;


/* ==================================================
 * Notae
 * ================================================== */

nomen enumeratio {
    MUSICXML_NOTA_SONANS,           /* pitch */
    MUSICXML_NOTA_PAUSA,            /* rest */
    MUSICXML_NOTA_PERCUSSA          /* unpitched */
} MusicxmlNotaGenus;

nomen enumeratio {
    MUSICXML_SYLLABA_SINGULA,       /* ordinaria si absens */
    MUSICXML_SYLLABA_INITIUM,
    MUSICXML_SYLLABA_MEDIA,
    MUSICXML_SYLLABA_FINIS,
    MUSICXML_SYLLABA_IGNOTA
} MusicxmlLocusSyllabae;

nomen structura {
             i32 versus;        /* number lectum; 0 = illegibile
                                  * (ordine documenti) */
               chorda versus_textus; /* number ut scriptum */
MusicxmlLocusSyllabae locus;
               chorda textus;
                  b32 extensio;      /* extend */
                  b32 iuncta;        /* post <elision> in eadem nota */
} MusicxmlSyllaba;

nomen enumeratio {
    MUSICXML_SIGNUM_CORONA,
    MUSICXML_SIGNUM_ARTICULATIO,
    MUSICXML_SIGNUM_ORNAMENTUM,
    MUSICXML_SIGNUM_ARCUS,          /* slur */
    MUSICXML_SIGNUM_TECHNICUM,
    MUSICXML_SIGNUM_ACORDUM_FRACTUM,
    MUSICXML_SIGNUM_DYNAMICA,       /* dynamica in notatione */
    MUSICXML_SIGNUM_ALIUD
} MusicxmlSignumGenus;

nomen structura {
 MusicxmlSignumGenus genus;
              chorda titulus;      /* elementum ("staccato", "sfz") */
                 i32 numerus;      /* arcus number */
                 b32 initium;      /* arcus start */
                 b32 finis;        /* arcus stop */
} MusicxmlSignum;

nomen structura {
               s64 positio;        /* intra mensuram (div. operis) */
               s64 longitudo;      /* gratia: 0 */
            chorda vox;            /* ut scripta ("1"); "" = nulla */
               i32 baculum;        /* I-basatum; 0 = nullum */
 MusicxmlNotaGenus genus;
               b32 mensura_tota;   /* rest measure="yes" */

         character littera;        /* scripta 'A'..'G' (percussa:
                                     * display-step); '\0' pausa */
               f64 alteratio;      /* alter (decimalis licet) */
               s32 octava;
         character littera_sonans; /* transpositione applicata */
               f64 alteratio_sonans;
               s32 octava_sonans;
               s32 nota;           /* sonans, alteratio rotundata;
                                     * percussa: midi-unpitched - I */
            chorda instrumentum;   /* id instrumenti (percussa) */

               b32 acordum;        /* chord: notae priori iungitur */
               b32 ligatura_initium;   /* tie start */
               b32 ligatura_finis;     /* tie stop */
               b32 gratia;
               b32 gratia_obliqua;     /* slash="yes" */
               s32 furtum_prioris;     /* steal-time-previous, % */
               s32 furtum_sequentis;
               s64 tempus_factum;      /* make-time, div. operis */
               b32 parva;              /* cue */
               i32 ratio_notarum;     /* time-modification (0 =
                                         * nullum) */
               i32 ratio_spatii;

    MusicxmlSignum* signa;
               i32  numerus_signorum;
   MusicxmlSyllaba* syllabae;
               i32  numerus_syllabarum;
               i32  linea;
               i32  columna;
} MusicxmlNota;


/* ==================================================
 * Mensurae, partes, opus
 * ================================================== */

nomen structura {
               chorda  numerus;     /* ut scriptus ("12", "X1") */
                  b32  implicita;   /* implicit="yes" (anacrusis) */
                  s64  initium;     /* linea temporis communis (M15) */
                  s64  longitudo;   /* huius partis (maxima vocum) */
    MusicxmlAttributa* attributa;
                  i32  numerus_attributorum;
MusicxmlLineaMensurae* lineae;
                  i32  numerus_linearum;
     MusicxmlDirectio* directiones;
                  i32  numerus_directionum;
         MusicxmlNota* notae;       /* ordine documenti */
                  i32  numerus_notarum;
                  i32  linea;
                  i32  columna;
} MusicxmlMensura;

nomen structura {
    chorda id;
    chorda titulus;        /* instrument-name */
       i32 canalis;        /* midi-channel 1..16; 0 = absens */
       i32 programma;      /* midi-program 1..128; 0 = absens */
       i32 percussio;      /* midi-unpitched 1..128; 0 = absens */
       f64 volumen;        /* 0..100; -1 = absens */
       f64 positio;        /* pan -180..180; 0 */
} MusicxmlInstrumentum;

nomen structura {
               chorda  id;
               chorda  titulus;        /* part-name */
               chorda  abbreviatio;
 MusicxmlInstrumentum* instrumenta;
                  i32  numerus_instrumentorum;
      MusicxmlMensura* mensurae;
                  i32  numerus_mensurarum;
} MusicxmlPars;

nomen enumeratio {
    MUSICXML_FORMA_PARTIUM,
    MUSICXML_FORMA_TEMPORUM
} MusicxmlForma;

nomen structura {
    MusicxmlForma forma;
           chorda versio;           /* ut scripta; "" = 1.0 */
           chorda titulus_operis;   /* work-title */
           chorda numerus_operis;
           chorda titulus_motus;    /* movement-title */
           chorda numerus_motus;
           chorda compositor;       /* creator composer */
           chorda poeta;            /* type="lyricist" (aut "poet") */
           chorda ius;              /* rights */
              b32 concentus;        /* concert-score (4.0): notae
                                      * iam sonantes */
              s64  divisiones;       /* divisio operis per quartam */
     MusicxmlPars* partes;
              i32  numerus_partium;
              s64* initia;           /* linea temporis communis */
              i32  numerus_mensurarum;
              s64  longitudo;        /* summa scripta */
   MusicxmlVitium* vitia;
              i32  numerus_vitiorum;
} MusicxmlOpus;


/* ==================================================
 * Ordo exsecutionis (lib/musicxml_ordo.c)
 * ================================================== */

nomen structura {
    i32 mensura;       /* index in partibus (0-basatus) */
    i32 transitus;     /* I-basatus: versus = transitus */
    s64 initium;       /* in linea evoluta */
} MusicxmlVisitatio;

nomen structura {
 MusicxmlVisitatio* visitationes;
               i32  numerus;
               s64  longitudo;       /* evoluta */
    MusicxmlVitium* vitia;           /* ordinis propria (opus
                                      * constans manet) */
               i32 numerus_vitiorum;
} MusicxmlOrdo;


/* ==================================================
 * Functiones
 * ================================================== */

/* octeti -> opus; NIHIL solum si memoria deficit. datum non copiatur:
 * in piscina vivere debet dum opus vivit. */
MusicxmlOpus*
musicxml_legere (
      chorda  datum,
     Piscina* piscina);

/* VERUM = nullum VITIUM (monita licent) */
b32
musicxml_successus (
    constans MusicxmlOpus* opus);

/* 'via:linea:columna: vitium|monitum: causa' per lineam; ordo NIHIL
 * licet. FALSUM = memoria deficit */
b32
musicxml_vitia_scribere (
    constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo,
       constans character* via,
        ChordaAedificator* exitus);

/* repetitiones, voltae, saltus -> visitationes (mensura, transitus);
 * NIHIL solum si memoria deficit */
MusicxmlOrdo*
musicxml_ordo (
    constans MusicxmlOpus* opus,
                  Piscina* piscina);

/* tabula canonica (TSV, oraculum - grammatica in plano P2): una linea
 * per notam sonantem post ligaturas iunctas, ordine exsecutionis;
 * ordo NIHIL = ordo scriptus, transitus unus. FALSUM = memoria */
b32
musicxml_tabulam_scribere (
    constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo,
        ChordaAedificator* exitus);

#endif /* MUSICXML_H */
