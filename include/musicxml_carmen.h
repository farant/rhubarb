/* musicxml_carmen.h - conversor: exemplar MusicXML -> textus carminis
 *
 * Stratum SECUNDUM (spec project-specs/musicxml-spec.md M1, par. IV):
 * solus locus qui musicxml.h et carmen.h ambo novit. Exitus TEXTUS est
 * (STML quem carmina_legere legit, carmen.h) - "textus manet veritas":
 * quod in librum musicae intrat homo legere et emendare potest, et
 * lector libri id iudicat ut carmen manu scriptum.
 *
 * USUS:
 *   MusicxmlCarmenForma f;
 *   MusicxmlCarmenRelatio r;
 *   musicxml_carmen_formam_ordinariam(&f);
 *   si (!musicxml_carmen_scribere(opus, NIHIL, &f, piscina, a, &r))
 *       musicxml_relationem_scribere(&r, "a.mxl", b);   (cur)
 *   ... "<carmina>" + a + "</carmina>" -> carmina_legere ...
 *
 * APPROXIMATIO (M5) = quidquid SONUM aut CANTUM mutat (altitudo,
 * initium, longitudo, nota omissa, versus omissus, tempus aut dynamica
 * mota). SIGNA NON CONVERSA (arcus, articulationes, ornamenta, verba
 * directionum, exercitium, pedale ...) carmen non fert et cantor
 * interpretatur: numerantur per genus, numquam approximationes,
 * numquam recusant. STRICTUS: approximatio prima -> FALSUM, nihil
 * scriptum (relatio eam nominat). Lector numquam approximat; conversor
 * solus.
 *
 * TABULA CONVERSIONIS (spec par. IV, summatim):
 *   pars / vox / sonus acordi   -> <pars> (M7: vox quaeque et nota
 *                                  acordi quaeque pars sua, titulo
 *                                  '<titulus>_<n>'; nota structurae)
 *   altitudo                    -> MML littera [+ -] o N (SONANS;
 *                                  alteratio duplex enharmonice
 *                                  scribitur - MML signum unum fert;
 *                                  nota structurae, non approximatio)
 *   longitudo                   -> longitudines MML in reticulo CMLX,
 *                                  '&' summae; extra reticulum
 *                                  rotundata (summa mensurae servata)
 *   mensura                     -> '|'; anacrusis, mensura irregularis
 *                                  -> <metrum> mensurae illius (M14),
 *                                  commentarium numeros scriptos dat
 *   senza-misura                -> metrum="liberum", incisum per
 *                                  mensuram (M6)
 *   armatura, metrum            -> <clavis>, <metrum> ad mensuram
 *   tempus                      -> <tempus> integer 20..400
 *   dynamica pp..ff             -> <dynamica pars mensura|pulsus>
 *   cuneus                      -> <dynamica ... usque> (finis ad
 *                                  initium mensurae proximae)
 *   fermata                     -> <fermata factor="2">
 *   verba                       -> <verba!> per frustum, versus unus;
 *                                  nota sine syllaba -> '_' (melisma);
 *                                  syllaba PURGATUR: characteres quos
 *                                  carmen dividit (spatium - _ , . ; :
 *                                  ! ?) et '<' tolluntur; vacua -> '_'
 *   gratiae, notae parvae       -> omissae (approximatio)
 *   repetitiones, voltae, saltus -> evoluta per musicxml_ordo (M8)
 *   titulus, compositor ...     -> titulus carminis (plicatus) +
 *                                  commentarium (opus, compositor,
 *                                  poeta, ius, fons)
 * Positiones in carmine: mensura="M" ad initium mensurae, aliter
 * pulsus="N" (exactum; ictus fractus esse posset).
 *
 * INSTRUMENTA (forma.instrumentum vacua; prima regula congruens vincit,
 * verba tituli partis minusculis comparata):
 *   I.  vox: soprano sopran superius cantus discantus treble "s." ->
 *       superius; alto alt altus contralto contratenor countertenor
 *       mezzo "a." -> altus; tenor "t." -> tenor; bass bassus basso
 *       baritone bariton "b." -> bassus (instrumentarium.stml: schola
 *       cum vox); voice voce stimme singstimme vox -> ex ambitu partis
 *       (nota media >= LXVII superius, >= LX altus, >= LIII tenor,
 *       aliter bassus)
 *   II. nomen instrumenti: violin violino viola cello violoncello
 *       kontrabass contrabass -> chordae; pizzicato -> pizzicato; harp
 *       arpa harfe -> harpa; piano pianoforte klavier -> chorda;
 *       harpsichord cembalo -> cembalo; organ organo orgel -> organum;
 *       flute flauto flote -> fistula; clarinet clarinetto klarinette
 *       -> clarinetum; horn corno -> cornu; bells glocken -> campanae_
 *       tubulares; glockenspiel -> glockenspiel; marimba -> marimba
 *   III. programma MIDI (familiae GM): 1-6 chorda, 7-8 cembalo,
 *       9-16 glockenspiel (13 marimba, 15 campanae_tubulares), 17-24
 *       organum, 25-40 chorda, 41-44 chordae, 45-46 pizzicato, 47
 *       harpa, 49-52 chordae, 53-55 schola (vox ex ambitu ut I),
 *       57-64 cornu, 65-72 clarinetum, 73-80 fistula, cetera aether
 *   IV. aliter aether
 * Electio in relatione nota structurae est (mutari potest per formam).
 */

#ifndef MUSICXML_CARMEN_H
#define MUSICXML_CARMEN_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "musicxml.h"


/* ==================================================
 * Approximationes et signa
 * ================================================== */

nomen enumeratio {
    MUSICXML_CARMEN_ALTERATIO_FRACTA,  /* alter non integer
                                        * (microtonus): semitonium
                                        * proximum */
    MUSICXML_CARMEN_EXTRA_RETICULUM,   /* longitudo non in reticulo CMLX
                                        * (septupla ...): rotundata */
    MUSICXML_CARMEN_EXTRA_AMBITUM,     /* nota extra o0..o8 (MIDI
                                        * XII..CXIX): octava mota */
    MUSICXML_CARMEN_NOTAE_SE_TEGUNT,   /* notae eiusdem vocis se tegunt:
                                        * prior truncata */
    MUSICXML_CARMEN_GRATIA_OMISSA,
    MUSICXML_CARMEN_PARVA_OMISSA,      /* cue */
    MUSICXML_CARMEN_VERSUS_OMISSUS,    /* versus praeter electum (semel
                                        * per partem) */
    MUSICXML_CARMEN_TEMPUS_ROTUNDATUM, /* tempus non integer */
    MUSICXML_CARMEN_TEMPUS_EXTRA_FINES,/* extra 20..400: coercitum */
    MUSICXML_CARMEN_DYNAMICA_IGNOTA,   /* sfz, fp, rf ...: omissa */
    MUSICXML_CARMEN_CLIVUS_MOTUS,      /* cuneus intra mensuram finitus:
                                        * ad initium proximae */
    MUSICXML_CARMEN_CLAVIS_NON_TRADITA,/* key-step/key-alter: omissa */
    MUSICXML_CARMEN_METRUM_ADDITIVUM,  /* 3+2/8 -> 5/8 */
    MUSICXML_CARMEN_METRUM_IMPOSSIBILE,/* N/D carmini alienum (D non
                                        * potentia II, N > LXIV) */
    MUSICXML_CARMEN_PERCUSSIO_OMISSA,  /* unpitched sine instrumento
                                        * noto */
    MUSICXML_CARMEN_APPROXIMATIONUM_GENERA
} MusicxmlApproximatioGenus;

nomen enumeratio {
    MUSICXML_CARMEN_ARCUS,             /* slur */
    MUSICXML_CARMEN_ARTICULATIO,
    MUSICXML_CARMEN_ORNAMENTUM,        /* non realizatum */
    MUSICXML_CARMEN_TECHNICUM,
    MUSICXML_CARMEN_ACORDUM_FRACTUM,   /* arpeggiate */
    MUSICXML_CARMEN_DYNAMICA_NOTAE,    /* dynamica in notatione */
    MUSICXML_CARMEN_VERBA_DIRECTIONIS, /* words */
    MUSICXML_CARMEN_EXERCITIUM,        /* rehearsal */
    MUSICXML_CARMEN_PEDALE,
    MUSICXML_CARMEN_OCTAVA_SIGNATA,    /* octave-shift (notae iam
                                        * sonantes) */
    MUSICXML_CARMEN_DIRECTIO_ALIA,
    MUSICXML_CARMEN_SIGNUM_ALIUD,
    MUSICXML_CARMEN_SIGNORUM_GENERA
} MusicxmlSignumCarminisGenus;

nomen structura {
 MusicxmlApproximatioGenus genus;
                    chorda pars;         /* id partis MusicXML */
                    chorda mensura;      /* numerus SCRIPTUS */
                       i32 transitus;    /* ordinis (M8) */
                       i32 linea;        /* fontis MusicXML */
                       i32 columna;
                    chorda descriptio;   /* Latine, brevis */
} MusicxmlApproximatio;


/* ==================================================
 * Forma et relatio
 * ================================================== */

nomen structura {
       b32 strictus;      /* approximatio prima -> FALSUM */
    chorda titulus;       /* carminis; vacua = ex opere (plicatus) */
       i32 versus;        /* versus electus (I); 0 = versus transitus
                            * (repetitione evoluta: transitu n versus
                            * n) */
    chorda instrumentum;  /* omnibus partibus; vacua = tabula supra */
} MusicxmlCarmenForma;

nomen structura {
    MusicxmlApproximatio* approximationes;
                     i32  numerus_approximationum;
                     i32  signa[MUSICXML_CARMEN_SIGNORUM_GENERA];
                  chorda* structurae;      /* notae structurae: partes
                                            * scissae (M7), repetitiones
                                            * evolutae, instrumenta,
                                            * respellatio, syllabae
                                            * purgatae, mensurae
                                            * renumeratae */
                     i32 numerus_structurarum;
} MusicxmlCarmenRelatio;


/* ==================================================
 * Functiones
 * ================================================== */

/* strictus FALSUM, titulus vacuus, versus I, instrumentum vacuum */
vacuum
musicxml_carmen_formam_ordinariam (
    MusicxmlCarmenForma* forma);

/* opus -> UNUM <carmen>...</carmen> in exitum (sine <carmina>). ordo
 * NIHIL = musicxml_ordo(opus). FALSUM: opus cum VITIO, strictus et
 * approximatio (relatio eam solam fert, exitus intactus), aut memoria
 * deficit. relatio semper impletur (in piscina). */
b32
musicxml_carmen_scribere (
           constans MusicxmlOpus* opus,
           constans MusicxmlOrdo* ordo,
    constans MusicxmlCarmenForma* forma,
                         Piscina* piscina,
               ChordaAedificator* exitus,
           MusicxmlCarmenRelatio* relatio);

/* relatio humana: linea per approximationem 'via: pars m.N t.T
 * linea:columna: genus - descriptio', numeri signorum per genus,
 * notae structurae. FALSUM = memoria deficit */
b32
musicxml_relationem_scribere (
    constans MusicxmlCarmenRelatio* relatio,
                constans character* via,
                 ChordaAedificator* exitus);

/* titulus brevis Latinus generis (relatio, auditio) */
constans character*
musicxml_approximationis_titulus (
    MusicxmlApproximatioGenus genus);

constans character*
musicxml_signi_carminis_titulus (
    MusicxmlSignumCarminisGenus genus);

#endif /* MUSICXML_CARMEN_H */
