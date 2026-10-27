/* explicatio.h - Harmonia explicata: fenestrae, acorda sine notis
 * alienis, nota quaeque cum genere suo (desideratum ...DDCMRP)
 *
 * FENESTRA: dum bassus TENET - classis notae infimae sonantis
 * (partes non mutae, sine '@') eadem manet; linea mensurae fenestram
 * secat. Bassus per octavas saliens (E2 E3 E2 E3) fenestra una est.
 * BASSUS ARPEGGIANS (extra acorda declarata): intra mensuram bassus
 * SALTU (>= III semitonia; gradu = transiens) ad classem quae iam in
 * fenestra sonat, nulla alia nota ibi incipiente (superiores tenent),
 * fenestram non secat - bassus alternans F C sub A C tentis = F, non
 * F F/C; inversio ex basso primo. Bassus ad classem novam aut gradu
 * (C D E sub C E: transiens) aut cum superioribus mutatis ut ante.
 *
 * ACORDUM fenestrae: classes omnium notarum sonantium, notis
 * alienis remotis - pars minima notarum candidatarum (forma
 * melodica alienae) removetur ut acordum_ex_classibus acordum
 * NOMINET; aequales: plures notae acordi, deinde trias/septima ante
 * suspensam ante dyadem, deinde radix = bassus (Bb A G super C E: A
 * removetur, C7 - non Bb, Am7/C). Bassus numquam removetur. Dyas
 * tritoni = septima dominantis implicita, eadem magnitudine
 * remotionis ac nomina (radix in clave: [G# D] in a minor -> E7,
 * implicitum). Fenestra sine nomine: classes omnes praeter bassum
 * alienae (genus per formam) - nomen a tono addito non tento
 * reiectum non redit (acordum.h NOTA ADDITA). NUCLEUS: si notae tentae
 * (non candidatae, cum basso) iam acordum plenum nominant (trias aut
 * septima), acordum MAIUS (septima aut toni additi) cuius radix aut
 * tertia solum ex notis formae alienae venit reicitur et remotio
 * pergit: D F# A cum G B transeuntibus = D, non Gmaj9/D. Tentae non
 * plenae melodiam radicem dare sinunt (G# B sub C D E: E7/G#);
 * triades et dyades non iudicantur (tertia ornata acordi est).
 *
 * ACORDA DECLARATA (Carmen.acorda, <acorda>): intra spatium acordi
 * declarati (non 'r') fenestrae ad mutationes eius et lineas
 * mensurae secantur, NON ad bassum - bassus inter classes acordi
 * motus fenestram non mutat (bassus alternans: I, non I I64);
 * acordum fenestrae = declaratum (litterae auctoris), bassus = nota
 * infima ad initium fenestrae (inversio; extra classes acordi:
 * bassus declaratus); notae ad acordum declaratum iudicantur (classes
 * eius acordi; ceterae per formam). Remotio minima et bassus transiens
 * ibi non adhibentur. Extra spatia (aut sine <acorda>) ut supra.
 *
 * BASSUS TRANSIENS: nota bassi gradu intrans et exiens (transitus aut
 * vicinus in parte sua) cuius superiores omnes in acordo fenestrae
 * prioris (contiguae, nominatae) sunt, acordum id servat cum basso suo
 * (C D E sub C E tentis: "C/D", D transitus). Superiores mutati (I
 * V6 I, sexta-quarta transiens) acordum novum nominant ut solet.
 *
 * SEGMENTUM: nota in fenestra una (nota per fenestras plures sonans
 * segmenta plura habet - suspensio sic videtur). Genus per motum in
 * VOCE sua (pars et CarmenNota.vox: comitatus voces suas dat - '[1 2
 * 3 4]' quattuor lineae, non una; textus scriptus vox una; nota
 * prior, proxima eiusdem vocis; pausa motum frangit). Nota generata
 * (CarmenNota.acordi) candidata numquam est: acordo declarato sumpta
 * (lilia: F6 vocibus D tenet):
 *   acordi       classis in acordo
 *   septima      septima acordi; resoluta = proxima gradu descendit,
 *                aut transiens sursum INTRA acordum idem (C D E
 *                super E7: proxima in fenestra eadem), aut sursum in
 *                inversione secunda cum basso gradu ascendente (V43
 *                -> I6: F G super D E, decimae)
 *   transitus    gradu intrat, gradu exit, eadem directione
 *   vicinus      gradu exit, gradu redit ad eandem
 *   appoggiatura saltu intrat, gradu exit
 *   suspensio    altitudo eadem ex segmento priore ubi acordi erat;
 *                resoluta = proxima gradu DESCENDIT
 *   retardatio   ut suspensio, gradu ASCENDIT
 *   anticipatio  proxima eadem altitudo, acordi in fenestra proxima
 *   escapata     gradu intrat, saltu contrario exit
 *   ignota       aliena sine forma
 * SEMITONIUM (segmenti cuiusque): nota proxima eiusdem vocis
 * semitonio unico distat - exitus chromaticus (censura: ornamentum
 * chromaticum extra clavem licet).
 * gradus = I aut II semitonia; saltus >= III. Ordo probationis:
 * suspensio, transitus, vicinus, appoggiatura, escapata, anticipatio.
 * Tempus forte NON hic iudicatur (censura id facit, segmenti pulsus
 * datur). Pedalis (bassus sub acordis mutatis) nondum.
 */
/* <aedilis corpus="lib/explicatio.c"/> */
#ifndef EXPLICATIO_H
#define EXPLICATIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "carmen.h"
#include "harmonia.h"

nomen enumeratio {
    EXPLICATIO_ACORDI,
    EXPLICATIO_SEPTIMA,
    EXPLICATIO_TRANSITUS,
    EXPLICATIO_VICINUS,
    EXPLICATIO_INNIXA,
    EXPLICATIO_SUSPENSIO,
    EXPLICATIO_RETARDATIO,
    EXPLICATIO_ANTICIPATIO,
    EXPLICATIO_EFFUGIENS,
    EXPLICATIO_IGNOTA
} ExplicatioGenus;

nomen structura {
         s64 initium;      /* pulsus carminis */
         s64 finis;
     Acordum harmonia;     /* sine notis alienis */
         b32 implicitum;   /* septima ex tritono sola */
         b32 declaratum;   /* acordum ex <acorda> (non inventum) */
} ExplicatioFenestra;

nomen structura {
    constans CarmenNota* nota;
                    i32  pars;        /* index in carmine */
                    s64  pulsus;      /* initium notae, carminis */
                    s64  initium;     /* segmenti (>= pulsus) */
                    i32  fenestra;
        ExplicatioGenus  genus;
                    b32  resoluta;    /* septima, suspensio,
                                       * retardatio, appoggiatura */
                    b32 semitonium;  /* nota proxima eiusdem vocis
                                       * semitonio unico distat
                                       * (exitus chromaticus) */
} ExplicatioSegmentum;

nomen structura {
     ExplicatioFenestra* fenestrae;     /* ordine temporis */
                    i32  numerus_fenestrarum;
    ExplicatioSegmentum* segmenta;      /* ordine (initium, pars) */
                    i32  numerus_segmentorum;
} Explicatio;

/* carmen totum. FALSUM: memoria deest */
b32
explicatio_carminis (
    constans Carmen* carmen,
            Piscina* piscina,
         Explicatio* explicatio);

/* "transitus", "septima" ... (statica) */
constans character*
explicatio_titulus (
    ExplicatioGenus genus);

/* linea addita partiturae (PartituraAddita.scribere, ctx =
 * Explicatio*):
 * per fenestram "C7:2. (A tr.)" - notae alienae abbreviatae, '*'
 * implicitum, '!' non resoluta */
b32
explicatio_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus);

#endif /* EXPLICATIO_H */
