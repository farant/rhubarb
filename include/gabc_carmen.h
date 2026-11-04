/* gabc_carmen.h - conversor: cantus gabc -> textus carminis
 *
 * Stratum SECUNDUM (spec project-specs/gabc-spec.md G1, par. III):
 * solus locus qui gabc.h et carmen.h ambo novit, ut musicxml_carmen.h.
 * Exitus TEXTUS est (STML quem carmina_legere legit): UNUM carmen
 * metro libero, pars una, sine involucro carminum.
 *
 * USUS:
 *   GabcCarmenForma f;
 *   GabcCarmenRelatio r;
 *   gabc_carmen_formam_ordinariam(&f);
 *   si (!gabc_carmen_scribere(cantus, &f, piscina, a, &r))
 *       gabc_relationem_scribere(&r, "salve.gabc", b);   (cur)
 *   ... "<carmina>" + a + "</carmina>" -> carmina_legere ...
 *   ... r.nexus: pulsus cantoris -> nota et syllaba gabc (G11) ...
 *
 * APPROXIMATIO (G10, ut musicxml M5) = quidquid SONUM aut CANTUM
 * mutat. SIGNA quae cantor interpretatur (ictus, signa supra notam,
 * liquescentia, episema cum mora, signa opaca) numerantur, numquam
 * approximationes, numquam recusant. Rhythmus Solesmensis (infra)
 * lectio est, non approximatio. STRICTUS: approximatio prima ->
 * FALSUM, exitus intactus (relatio eam nominat). Cantus cum VITIO
 * (gabc_successus FALSUM) aut sine notis -> FALSUM: tabula
 * antiphonale versionem priorem servat (G13). Typographica (spatia,
 * lineae fractae, custodes, translationes, textus supra systema)
 * nihil sonant et non numerantur.
 *
 * TABULA CONVERSIONIS (spec par. III, decreta G8 G9):
 *
 *   ALTITUDO   sonus notae a do (semitonia) = XII x octava + scala
 *              maior {0 2 4 5 7 9 11}[gradus mod VII] + alteratio.
 *              do absolutum (MIDI) = forma.do_fixum; aut (do_fixum -1)
 *              summus - sonus notae altissimae: nota altissima
 *              cantus in summo stat EXACTE (ordinarie LXV = F4; Salve
 *              2435: do = F, ut salve.py olim). Nomen do ex classe
 *              toni: C Db D Eb E F Gb G Ab A Bb B (bemolles
 *              praeferuntur).
 *              Littera notae = littera do + gradus; signum ex scala
 *              maiore do + alteratio; signum duplex -> enharmonice
 *              (MML signum unum fert; nota structurae, non
 *              approximatio). Octava MML litterae SCRIPTAE (Cb4 =
 *              'o4 c-'). Extra MIDI XII..CXIX (o0..o8) -> octava mota
 *              (approximatio)
 *   RHYTHMUS   (G8, regulae salve.py deleti, ordine; notae eius
 *              fixae: probationes/fixa/gabc/salve_py_notae.tsv) in
 *              sextis decimis:
 *              1. nota quaelibet (punctum, inclinatum, virga,
 *                 stropha, oriscus, quilisma, cavum, liquescens) = II
 *                 (octava)
 *              2. mora '.' -> IV; '..' in nota ultima neumae -> IV in
 *                 notis DUABUS ultimis (Gregorius: puncta clivis
 *                 ambarum)
 *              3. episema '_' -> III, solum sine mora (cum mora:
 *                 signum numeratum); numeri '_0'..'_5' locum solum
 *                 dant
 *              4. nota ante quilisma in EADEM syllaba -> max(sua,
 *                 III); quilisma prima syllabae nihil longat
 *              5. unisoni ligati: nota eiusdem soni ac nota carminis
 *                 prior, in eadem syllaba, sine divisione inter eas,
 *                 eam longat (summa; etiam trans neumas: 'gh/hg') -
 *                 bivirga, tristropha, pressus. Numquam trans
 *                 syllabam aut divisionem
 *              longitudo -> MML: 1 16, 2 8, 3 8., 4 4, 6 4., 8 2, 12
 *              2., 16 1, 24 1.; ceterae summa avida ex his cum '&'
 *              (nota iterata: "c4 & c16")
 *   INCISA     divisio -> '|' (carmen.h: incisum quodque mensura
 *              sua) cum spiritu ante lineam: virgula '`', minimis '^',
 *              minima ',', minor ';', dominicana ',N' ';N' -> r16;
 *              maior ':', maior punctata, finalis '::' -> r8.
 *              Divisiones sine nota inter eas (aut ante notam primam)
 *              unum finem faciunt, spiritu longissimo; incisum
 *              vacuum numquam. Incisum ultimum sine spiritu et sine
 *              '|'. Divisio intra syllabam aut neumam ibi secat
 *   VERBA      syllaba cum nota carminis incipiente -> syllaba
 *              cantata; ' _' pro nota carminis quaque altera eius
 *              (post unisonos ligatos). Separator ante syllabam: '-'
 *              intra verbum, ' ' ubi verbum_incipit; primae nihil.
 *              Syllaba sine textu cantato (aut purgata vacua) -> '_'
 *              pro nota quaque (melisma; ante syllabam primam '_'
 *              quoque - carmen vocalem ordinariam dat). Syllaba sine
 *              notis non cantatur ('*()' ...). Nullus textus in cantu
 *              -> verba nulla.
 *              PURGATIO (syllaba_munda salve.py olim, Latine extensa):
 *              textus solum CANTATUS, non ELISIO; '{' '}' tolluntur;
 *              ae oe ligatae -> ae oe (etiam cum accentu); vocales
 *              accentu notatae -> sine accentu; deinde solum A-Z a-z
 *              servantur; littera prima ut scripta, ceterae
 *              minusculae ("SAL" -> "Sal"). Signa nota (R/ V/ cruces,
 *              interpunctio, cifrae, spatia, virgulae typographicae)
 *              tacite tolluntur; littera non ASCII ignota tollitur
 *              cum approximatione
 *   CLAVIS     finalis = nota carminis ultima. Collectio: si notae
 *              si (gradus VI) plus quam dimidia parte bemolles sunt
 *              (etiam clave 'cb'), collectio fa (do' = fa, si
 *              bemolle); aliter collectio do. Nomen = littera finalis
 *              + modus ex gradu finalis in collectione: 0 ionius, 1
 *              dorius, 2 phrygius, 3 lydius, 4 mixolydius, 5 aeolius
 *              (Salve: "f ionius"). Gradus VI collectionis (si) aut
 *              finalis alia alterata -> "<do collectionis> maior" +
 *              approximatio. Modus capitis (mode, differentia) in
 *              commentario solum
 *   CARMEN     commentarium "gabc: name; modus M [differentia];
 *              office-part; liber book; transcriptor T; do = F3"
 *              (partes absentes omissae, '--' in valore -> '- -');
 *              carmen titulus (forma.titulus aut name capitis,
 *              plicatus [a-z0-9_]+ ut musicxml_carmen; nullo "gabc"),
 *              tempus forma.tempus, metrum "liberum", clavis supra;
 *              reverberatio aulae ecclesiae (aula 0.9, mollities 0.6,
 *              latitudo 1, lucrum 0.5, ut salve); pars "cantus",
 *              instrumentum forma.instrumentum (vacuum = schola),
 *              lucrum 1, reverberatio 0.55; frustum unum mensura 1
 *              cum textu MML et verbis
 *   NEXUS      (G11) linea una per notam carminis, ordine initii:
 *              initium et longitudo in pulsibus (CMLX per quartam,
 *              punctum = CDLXXX), nota gabc prima et numerus notarum
 *              ligatarum, syllaba, neuma, linea:columna notae primae.
 *              Spiritus lineam non habet (lacuna inter lineas)
 */

#ifndef GABC_CARMEN_H
#define GABC_CARMEN_H

/* <aedilis corpus="lib/gabc_carmen.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "gabc.h"


/* ==================================================
 * Approximationes et signa
 * ================================================== */

nomen enumeratio {
    GABC_CARMEN_CLAVIS_SINE_MODO,      /* finalis in si collectionis aut
                                        * alterata: "<do> maior" */
    GABC_CARMEN_EXTRA_AMBITUM,         /* nota extra o0..o8 (MIDI
                                        * XII..CXIX): octava mota */
    GABC_CARMEN_LITTERA_IGNOTA,        /* littera non Latina e verbis
                                        * sublata */
    GABC_CARMEN_APPROXIMATIONUM_GENERA
} GabcApproximatioGenus;

nomen enumeratio {
    GABC_CARMEN_ICTUS,                 /* ''' episema verticale */
    GABC_CARMEN_SIGNUM_NOTAE,          /* r1..r5: accentus, circuli */
    GABC_CARMEN_LIQUESCENTIA,          /* ~ < > et initio debilis */
    GABC_CARMEN_EPISEMA_CUM_MORA,      /* episema non sonatum (regula
                                        * RHYTHMUS 3) */
    GABC_CARMEN_SIGNUM_OPACUM,         /* GABC_EL_SIGNUM_OPACUM */
    GABC_CARMEN_SIGNORUM_GENERA
} GabcSignumCarminisGenus;

nomen structura {
    GabcApproximatioGenus genus;
                      s32 syllaba;   /* -1 = cantus totus */
                      i32 linea;     /* fontis gabc; 0 = nulla */
                      i32 columna;
                   chorda descriptio; /* Latine, brevis */
} GabcApproximatio;

/* G11: linea una per notam carminis */
nomen structura {
    s64 initium;                     /* pulsus in carmine */
    s64 longitudo;                   /* pulsus */
    i32 nota;                        /* index in GabcCantus.notae */
    i32 numerus;                     /* notae gabc ligatae [nota,
                                      * nota+numerus) */
    i32 syllaba;
    i32 elementum;                   /* neuma notae primae (index in
                                      * syllaba.elementa) */
    i32 linea;
    i32 columna;
} GabcNexus;


/* ==================================================
 * Forma et relatio
 * ================================================== */

nomen structura {
       b32 strictus;      /* approximatio prima -> FALSUM */
       s32 summus;        /* MIDI notae altissimae (LXV) */
       s32 do_fixum;      /* MIDI do; -1 = ex summo */
       i32 tempus;        /* quartae per minutum (LXXX) */
    chorda titulus;       /* carminis; vacua = name capitis */
    chorda instrumentum;  /* vacuum = schola */
} GabcCarmenForma;

nomen structura {
    GabcApproximatio* approximationes;
                 i32  numerus_approximationum;
                 i32  signa[GABC_CARMEN_SIGNORUM_GENERA];
              chorda* structurae;      /* notae structurae: do et
                                        * summus, collectio clavis,
                                        * syllabae purgatae,
                                        * respellatio enharmonica */
                 i32  numerus_structurarum;
           GabcNexus* nexus;
                 i32  numerus_nexuum;
                 s32  do_midi;          /* do electum; -1 si FALSUM
                                         * ante altitudinem */
              chorda clavis;           /* "f ionius" ut scripta */
} GabcCarmenRelatio;


/* ==================================================
 * Functiones
 * ================================================== */

/* strictus FALSUM, summus LXV, do_fixum -1, tempus LXXX, titulus et
 * instrumentum vacua */
vacuum
gabc_carmen_formam_ordinariam (
    GabcCarmenForma* forma);

/* cantus -> commentarium + UNUM <carmen>...</carmen> in exitum (sine
 * <carmina>). FALSUM: cantus cum VITIO aut sine notis, strictus et
 * approximatio (relatio eam solam fert, exitus intactus), aut memoria
 * deficit. relatio semper impletur (in piscina). */
b32
gabc_carmen_scribere (
         constans GabcCantus* cantus,
    constans GabcCarmenForma* forma,
                     Piscina* piscina,
           ChordaAedificator* exitus,
           GabcCarmenRelatio* relatio);

/* relatio humana: linea per approximationem 'via:linea:columna: genus
 * - descriptio', numeri signorum per genus, notae structurae. FALSUM
 * = memoria deficit */
b32
gabc_relationem_scribere (
    constans GabcCarmenRelatio* relatio,
            constans character* via,
             ChordaAedificator* exitus);

/* titulus brevis Latinus generis (relatio, auditio) */
constans character*
gabc_approximationis_titulus (
    GabcApproximatioGenus genus);

constans character*
gabc_signi_carminis_titulus (
    GabcSignumCarminisGenus genus);

#endif /* GABC_CARMEN_H */
