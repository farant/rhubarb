/* silva_forma.h - FORMAE: fragmenta C cum lacunis super arbores silvae
 * (transformare T1-T2; spec project-specs/transformare-spec.md)
 *
 * Forma = fragmentum C (expressio aut sententia) in quo '$nomen'
 * lacuna est (expressionem unam capit) et '$...nomen' reliquum
 * listae argumentorum (nullum aut plura). Fragmentum ut C parsatur
 * cum lexico latino (si, redde, ZEPHYRUM...): arbor POST expansionem
 * est, ergo forma 'si (f($x) != ZEPHYRUM)' et fons 'if (f(y) != 0)'
 * eadem arbore congruunt, dum capturae octetos fontis ipsius servant.
 *
 * Lacunae ante lexationem nomina reservata fiunt ('$x' -> '__trf_x',
 * '$...r' -> '__trfv_r') - numquam in plagulam veram perveniunt;
 * litterae chordarum et characterum intactae manent.
 */
#ifndef SILVA_FORMA_H
#define SILVA_FORMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_nodus.h"
#include "silva_parsare.h"

nomen enumeratio {
    SILVA_FORMA_EXPRESSIO = ZEPHYRUM,   /* fragmentum = expressio una */
    SILVA_FORMA_SENTENTIA               /* fragmentum = sententia una */
} SilvaFormaGenus;

/* lacuna formae: nomen sine '$' (et sine '...'), reliquum = '$...' */
nomen structura {
    chorda titulus;
       b32 reliquum;
} SilvaLacuna;

nomen structura {
        SilvaFormaGenus  genus;
           SilvaParsura* parsura;   /* arbor fragmenti involuti */
    constans SilvaNodus* nodus;    /* expressio aut sententia ipsa */
                    Xar* lacunae;   /* SilvaLacuna, ordine primae
                                    * occurrentiae, unicae */
} SilvaForma;

/* Fragmentum parsare. NIHIL + causa nominata si: lacuna deformis
 * ('$' sine nomine, '$...' sine nomine), fragmentum non parsatur
 * (errores), genus non congruit (EXPRESSIO intra 'redde ...;' parsatur:
 * sententia data ut EXPRESSIO non parsatur; SENTENTIA non una). Textus
 * non NUL-terminatus licet. LIMES: SENTENTIA 'f($x);' nuda ambigua est
 * (declaratio sine prototypo) - vocatio nuda forma EXPRESSIO 'f($x)'
 * scribitur et contra expressionem sententiae congruit. */
SilvaForma*
silva_formam_parsare (
                Piscina* piscina,
     constans character* textus,
                    i32  mensura,
        SilvaFormaGenus  genus,
                 chorda* causa_out);

/* Estne nodus lacuna? (folium-identificator nominis reservati) -
 * titulus et reliquum ponuntur si non NIHIL. */
b32
silva_lacuna_est (
    constans SilvaNodus* nodus,
                 chorda* titulus_out,
                    b32* reliquum_out);

/* Locus nodi per NOMEN (registrum C89): NIHIL si nodus aut locus
 * ignotus. Ambulationes formarum loca nominant, numquam indices. */
constans SilvaValor*
silva_nodi_locus (
    constans SilvaNodus* nodus,
     constans character* titulus);

/* Nomen generis nodi (registrum C89); "" si ignotum */
constans character*
silva_nodi_genus_titulus (
    constans SilvaNodus* nodus);


/* ==================================================
 * Congruentia (transformare T2)
 * ================================================== */

/* captura lacunae: '$x' -> nodus (subarbor fontis ipsa, non copia);
 * '$...r' -> elementa (Xar de MateriaValor: nodi et commata listae
 * fontis, ordine; vacua licet) */
nomen structura {
                 chorda  titulus;
                    b32  reliquum;
    constans SilvaNodus* nodus;
                    Xar* elementa;
} SilvaCaptura;

/* Congruitne forma cum nodo? Comparatio per genera et valores
 * lexematum (trivia - spatia, commentaria - neglecta; arbor post
 * expansionem: 'si'/'if' et 'ZEPHYRUM'/'0' aequalia). '$x' quemlibet
 * nodum capit; '$x' iteratus subarborem aequalem poscit; '$...r'
 * elementum ULTIMUM listae reliquum capit (nullum licet - tunc comma
 * praecedens formae quoque vacuum congruit). capturae (Xar de
 * SilvaCaptura, a vocatore creata) evacuatur et impletur; NIHIL
 * licet. Congruentia fracta capturas partiales relinquere potest. */
b32
silva_forma_congruit (
                Piscina* piscina,
    constans SilvaForma* forma,
    constans SilvaNodus* nodus,
                    Xar* capturae);

/* Aequalitas subarborum trivia neglecta (genera, loci, valores
 * lexematum) - lacunae ut identificatores ordinarii */
b32
silva_nodi_aequales (
    constans SilvaNodus* a,
    constans SilvaNodus* b);


/* ==================================================
 * Situs (transformare T3)
 * ================================================== */

/* situs: vocatio functionis cuius nomen in plagula principali ipsa
 * scribitur (radix lexematis in fonte principali). in_macro:
 * parentheses vocationis ex expansione macro veniunt - vocatio a macro
 * facta,
 * numquam rescribitur (consumens NIHIL, macro = macro exterior in
 * plagula invocatus). Alioquin consumens = primus usor valoris:
 * ascensus per parentheses, ambigua et conversionem '(vacuum)'
 * (valorem abicit, non adhibet); declarator initiatus -> declaratio
 * tota. linea/columna = sedes nominis functionis (1-basatae). */
nomen structura {
    constans SilvaNodus* vocatio;
    constans SilvaNodus* consumens;
                    b32  in_macro;
                 chorda  macro;
                    i32  linea;
                    i32  columna;
} SilvaSitus;

/* Omnes vocationes 'functio' (valor nominis POST expansionem) in
 * spina canonica arboris (ambigua per lectionem canonicam), ordine
 * fontis. Xar de SilvaSitus (vacua licet); NIHIL = apparatus
 * (memoria). Vocationes in plagulis aliis (capita inclusa) omissae. */
Xar*
silva_situs_invenire (
                   Piscina* piscina,
     constans SilvaParsura* parsura,
                    chorda  functio);

/* Forma situs (non in_macro). clavis = genus consumentis + valores
 * lexematum post expansionem spatio separati, argumenta vocationis ->
 * $1..$n, corpora sententiarum (si/dum/per/fac/commutatio) -> '...':
 * clavis aequalis = eadem rescriptio. scriptura = consumens ut in
 * plagula scriptus (orthographia auctoris: si, ZEPHYRUM), eisdem
 * substitutionibus, commentaria omissa, spatia in unum contracta.
 * fons/mensura = textus plagulae principalis. FALSUM si situs
 * in_macro aut extentum ignotum. LIMES: invocatio macro functionalis
 * in FINE consumentis extentum ante ')' suam finit (expansio ')'
 * invocationis non fert). */
b32
silva_situs_forma (
                  Piscina* piscina,
    constans SilvaParsura* parsura,
      constans SilvaSitus* situs,
       constans character* fons,
                      i32  mensura,
                   chorda* clavis_out,
                   chorda* scriptura_out);

#endif /* SILVA_FORMA_H */
