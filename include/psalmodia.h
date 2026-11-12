/* psalmodia.h - aptator: textus psalmi punctatus + tonus -> gabc
 *
 * Spec: project-specs/psalmodia-spec.md (P1 gabc solum, P6 textus
 * punctatus, P8 accentus ex signis, P9 totalitas, P10 forma librorum,
 * P11 rhythmus conversoris est; III-V). Planum:
 * project-specs/psalmodia-plan-2.md T3-T6. Tabula: psalmodia_tabula.h
 * (REGULA ACCENTUS et TEXTUS DEFICIENS in capite tabulae ipsius).
 * Stratum: psalmodia_tabula, syllabae_latinae, chorda_aedificator -
 * carmen, gabc lectorem, vicum nescit.
 *
 * USUS:
 *   terminatio = psalmodia_signum_invenire(tabula, signum, &tonus, c);
 *   psalmus = psalmodia_aptare(tonus, terminatio, textus, NIHIL, p);
 *   psalmodia_admonitiones_scribere(psalmus, "116.txt", nuntii);
 *   psalmodia_gabc_scribere(psalmus, NIHIL, exitus);
 *
 * TEXTUS PUNCTATUS (UTF-8):
 *   - versus unus per lineam; lineae vacuae et lineae quarum signum
 *     primum '%' est (commentaria) praetereuntur; BOM initio, CRLF
 *     et CR accipiuntur;
 *   - numerus initio lineae ("1. ", "12 ") omittitur; numeri exitus
 *     ab aptatore dantur;
 *   - '*' mediatio (semel per versum); '†' aut '+' flexa (semel, ante
 *     '*'); signa haec etiam verbis adhaerentia agnoscuntur;
 *   - verba spatiis separantur; interpunctio verbo adhaerens in textu
 *     manet (Dómino, -> syllaba ultima "no,");
 *   - accentus ut in libro: acutus (á é í ó ú ý ǽ); '-' syllabas
 *     cogit (E-u-o-u-a-e); diaeresis hiatum facit (Israël). Divisio
 *     et accentus: syllabae_latinae.h;
 *   - verbum trium aut plurium syllabarum sine acuto (nec in
 *     VOCIBUS HEBRAICIS): admonitio verbum nominans; aptatur quasi
 *     sine accentu (accentus cadentiae in eo non recedit) - numquam
 *     accentus coniectus;
 *   - signa gabc in textu ( ( ) [ ] { } < > | % ) et octeti non UTF-8:
 *     admonitio, ex textu exitus omittuntur.
 *
 * VOCES HEBRAICAE (decretum Frani 1, 2026-11-12): index in
 * lib/psalmodia.c, cum divisione, syllaba accentus et "abrupta".
 * Agnoscuntur sine respectu maiusculae, acuti, diaeresis, i/j et
 * interpunctionis adhaerentis; acutus in textu syllabam indicis
 * vincit; '-' in textu divisionem indicis vincit. Nunc:
 *   Is-ra-el (Ís), Ja-cob (Ja), Si-on (Si), Da-vid (Da),
 *   A-a-ron (Á), Ju-da (Ju), E-phra-ta (É), Ba-san (sán),
 *   Si-na (ná), Je-ru-sa-lem (rú), Sa-ba-oth (Sá), Ce-dar (Ce).
 * Omnes abruptae: semiversus in voce hebraica desinens ut in
 * monosyllaba tractatur (AR p. 5*: "in monosyllaba vel voce
 * hebraica") - clausula soror casus="monosyllaba" si tonus eam
 * imprimit (psalmodia_clausulam_monosyllabam), aliter regula
 * communis cum accentu indicis; in flexa vox in tenore sustinetur.
 * (Exempla: II "ex Sion" h h i, IV "Dómine David" h h g h i, in
 * directum "rédimet Ísrael".) Index crescit cum psalmus vocem novam
 * postulat. "Amen" non est in indice: syllabae et accentus (Á-men)
 * regulas communes sequuntur, et terminatio super "sæculórum. Amen"
 * communis est (EUOUAE).
 *
 * REGULA (verbatim ex capite tabulae): clausula retrorsum aptatur.
 * Nota ultima syllabam ultimam capit; quisque <accentus> locum suum
 * nominalem (syllabam ante locos sequentes) capit, NISI ea syllaba
 * accentum verbi sui proxime sequitur (dó-MI-nus, sae-CU-li,
 * lo-cú-TUS sum): tunc accentus in syllabam accentus recedit et
 * syllaba transita supervenientem capit. Syllaba ultima
 * proparoxytoni (dó-mi-NUS) accentum capere potest. Monosyllaba
 * finalis accentum in syllabam priorem mittit (eadem regula).
 * Praeparationes ante accentum primum, numero fixo; tenor ceteris.
 * TEXTUS DEFICIENS: in terminatione notae primae sine syllaba
 * omittuntur; in mediatione notae reliquae cum tenore in syllabam
 * primam contrahuntur (I: Qui = h ixi h); semiversus primus (aut
 * canticum) nimis brevis pro intonatione et mediatione: mediatio
 * omittitur (I, III: Magníficat). Semiversus vacuus: admonitio.
 * INTONATIO: notae <initium> syllabis primis semiversus primi,
 * una per syllabam, sine accentu; versu primo solum, aut omni versu
 * (canticum). Peregrinus: <initium> terminationis secundum
 * semiversum versus primi incipit; tenor terminationis valet.
 * FLEXA: syllaba ultima ante '†' notam flexae capit, et paenultima
 * si verbum proparoxytonum est; in monosyllaba vel voce hebraica
 * vox in tenore sustinetur (AR p. 5*).
 * Versus sine '*': admonitio; totus ut semiversus secundus
 * aptatur.
 *
 * STILUS: syllabae in quas loci <accentus> cadunt -> ACCENTUS
 * (gabc <b>); loci <praeparatio> -> PRAEPARATIO (<i>); ceterae
 * (tenor, intonatio, notae, superveniens) nullus.
 *
 * EXITUS GABC (P10, P11; GregoBase psalmi scripti, plan T3):
 *   name: <titulus>;          (si titulus datus)
 *   mode: <modus tabulae>;    ("8", "peregrinus", "in directum")
 *   annotation: <signum>;     (modus + " " + differentia: "8 G")
 *   %%
 *   (<clavis>) 1. Lau(j)dá(j)te(j) ... <b>Dó</b>(k)mi(jr)num, *(:)
 *   lau(j)... <i>e</i>(i)<b>um</b>(h)... (::)
 * Clavis: terminationis si data (II A: c3), aliter toni - totus
 * psalmus in ea clave. Syllaba = textus(notae); spatium ante
 * syllabam verbum incipientem; notae tabulae ipsae (superveniens
 * cum 'r'), puncta in tenore, nihil additum (morae, episemata
 * conversoris sunt). Signa: " †(,)" post flexam, " *(:)" in
 * mediatione, " (::)" fine versus, linea nova post. Numerus "N. "
 * syllabae primae versus psalmi praefigitur (1, 2, ...); Gloria
 * Patri sine numero. Gloria Patri (nisi sine_gloria) = versus duo
 * ultimi, textus fixus:
 *   Glória Patri, et Fílio, * et Spirítui Sancto.
 *   Sicut erat in princípio, et nunc, et semper, * et in sǽcula
 *   sæculórum. Amen.
 * Omnis exitus per gabc_legere sine vitio legitur (probatio T6).
 *
 * TOTALITAS: octeti quilibet -> psalmus (NIHIL solum si memoria
 * deficit) cum admonitionibus 'linea:columna: causa' (lineae et
 * columnae ab I; columna = characteres UTF-8, non octeti); tonus
 * aut terminatio NIHIL -> admonitio, versus nulli. Gabc semper
 * scribitur (versus aptati, admonitiones non impediunt).
 *
 * DATA: omnia in piscina; textus non copiatur (syllabae in textum
 * vocantis monstrant, praeter Gloriam et numeros); tabula post
 * aptationem servanda (psalmus tonum monstrat).
 */

#ifndef PSALMODIA_H
#define PSALMODIA_H

/* <aedilis corpus="lib/psalmodia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "psalmodia_tabula.h"


/* ==================================================
 * Syllabae et versus
 * ================================================== */

nomen enumeratio {
    PSALMODIA_STILUS_NULLUS,
    PSALMODIA_STILUS_ACCENTUS,       /* locus accentus: <b> */
    PSALMODIA_STILUS_PRAEPARATIO     /* locus praeparationis: <i> */
} PsalmodiaStilus;

/* syllaba aptata. Signa '†' et '*' syllabae propriae sunt (textus et
 * notae "†" aut "*"), ut notae versus iunctae <exemplum notae>
 * tabulae aequent. */
nomen structura {
             chorda textus;   /* cum interpunctione adhaerente */
             chorda notae;    /* gabc: "h", "ixi", "hr" */
    PsalmodiaStilus stilus;
                b32 verbum_incipit;
                i32 linea;    /* in textu; 0 = Gloria */
                i32 columna;
} PsalmodiaSyllaba;

nomen structura {
    PsalmodiaSyllaba* syllabae;
                 i32  numerus;
                 i32  linea;
                 b32  gloria;   /* versus Gloriae: sine numero */
} PsalmodiaVersus;

nomen structura {
       i32 linea;
       i32 columna;
    chorda causa;
} PsalmodiaAdmonitio;

/* optiones: zephyro impletae (aut NIHIL) = ordinariae */
nomen structura {
    chorda titulus;           /* name:; vacua = nullum */
       b32 sine_gloria;       /* Gloria Patri omittitur */
       b32 antiqua;           /* mediatio ad libitum (VI) */
       b32 canticum;          /* intonatio in omni versu */
       b32 sine_intonatione;  /* versus primus sine intonatione
                               * (exempla versuum sequentium) */
} PsalmodiaOptiones;

nomen structura {
          constans PsalmodiaTonus* tonus;
       constans PsalmodiaClausula* terminatio;
                  PsalmodiaVersus* versus;
                              i32  numerus_versuum;
               PsalmodiaAdmonitio* admonitiones;
                              i32  numerus_admonitionum;
} PsalmodiaPsalmus;

/* quomodo pars textus desinit (psalmodia_partem_aptare) */
nomen enumeratio {
    PSALMODIA_FINIS_NULLUS,          /* textus pergit (exempla
                                      * "initium") */
    PSALMODIA_FINIS_FLEXA,
    PSALMODIA_FINIS_MEDIATIO,
    PSALMODIA_FINIS_TERMINATIO
} PsalmodiaFinis;


/* ==================================================
 * Aptatio
 * ================================================== */

/* textus punctatus totus -> psalmus: mediatio per
 * psalmodia_mediationem_eligere(tonus, antiqua), soror monosyllaba
 * per semiversum ubi desinit in monosyllaba aut voce hebraica;
 * Gloria Patri nisi sine_gloria. NIHIL solum si memoria deficit. */
PsalmodiaPsalmus*
psalmodia_aptare (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* terminatio,
                        chorda  textus,
    constans PsalmodiaOptiones* optiones,
                       Piscina* piscina);

/* pars una (semiversus, aut pars ante flexam) -> psalmus versu uno,
 * sine signis (exempla libri). clausula NIHIL pro FLEXA et NULLUS;
 * soror monosyllaba ut in psalmodia_aptare. intonatio: <initium>
 * toni (MEDIATIO, NULLUS) aut terminationis (TERMINATIO,
 * peregrinus). '*' aut '†' in textu: admonitio. */
PsalmodiaPsalmus*
psalmodia_partem_aptare (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* clausula,
                PsalmodiaFinis  finis,
                           b32  intonatio,
                        chorda  textus,
                       Piscina* piscina);


/* ==================================================
 * Scriptura
 * ================================================== */

/* psalmus -> gabc (EXITUS GABC). FALSUM solum si memoria deficit. */
b32
psalmodia_gabc_scribere (
     constans PsalmodiaPsalmus* psalmus,
    constans PsalmodiaOptiones* optiones,
             ChordaAedificator* exitus);

/* 'via:linea:columna: causa' per admonitionem */
b32
psalmodia_admonitiones_scribere (
    constans PsalmodiaPsalmus* psalmus,
           constans character* via,
            ChordaAedificator* exitus);

#endif /* PSALMODIA_H */
