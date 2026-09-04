/* oratio_vocabularium.h - Vocabularium Latinum ex WORDS Whitakeri (T7,
 * spec par. IV)
 *
 * Fontes venditi verbatim sub oratio/vocabularium/la/ (FONTES.md):
 * DICTLINE.GEN (stirpes, FIXAE columnae), INFLECTS.LAT (flexiones,
 * spatiis separatae), ADDONS.LAT (praefixa, suffixa, tackons - terna),
 * UNIQUES.LAT (formae unicae - terna). COCTIO fontes legit, columnas
 * in OMNI recordo asserit (recordum quod non cadit = inventum quod
 * coctionem sistit, cum linea et causa) et tabulam UNAM emittit:
 * fluxus nuntii (protobuf-formatus) recordorum nidorum -
 *
 *   tag I  CAPUT         versio, fons, numeri recordorum
 *   tag II STIRPS        stirpes I-IV, pars, declinatio, varians,
 *                        species (residuum recordi partis verbatim,
 *                        lexemata spatio uno iuncta), codices aetas /
 *                        area / geo / frequentia / fons, sensus, linea
 *   tag III FLEXIO       pars, declinatio, varians, accidentia (lexemata
 *                        iuncta), clavis stirpis, mensura, finis, aetas,
 *                        frequentia, linea
 *   tag IV ADDITAMENTUM  genus (TACKON/PREFIX/SUFFIX), forma,
 *                        descriptio (recordum), sensus, linea
 *   tag V  UNICUM        forma, descriptio, sensus, linea
 *
 * Tabula commissa oratio/vocabularium/la.bin; porta probat coctio
 * (fontes) == tabula commissa octetim et sigillum (SHA-256, XVI hex)
 * pinnat. Quaesitio (gradus T8) tabulam dispersam per clavem plicatam
 * (u/v, i/j) ex fluxu in onere struit - nihil hic plicatur: octeti
 * fontis ut sunt. Partes orationis ordine enumerationis Ada
 * (Part_Of_Speech_Type): X N PRON PACK ADJ NUM ADV V VPAR SUPINE PREP
 * CONJ INTERJ TACKON PREFIX SUFFIX.
 */

#ifndef ORATIO_VOCABULARIUM_H
#define ORATIO_VOCABULARIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"

nomen enumeratio {
    ORATIO_PARS_X = 0,
    ORATIO_PARS_N,
    ORATIO_PARS_PRON,
    ORATIO_PARS_PACK,
    ORATIO_PARS_ADJ,
    ORATIO_PARS_NUM,
    ORATIO_PARS_ADV,
    ORATIO_PARS_V,
    ORATIO_PARS_VPAR,
    ORATIO_PARS_SUPINE,
    ORATIO_PARS_PREP,
    ORATIO_PARS_CONJ,
    ORATIO_PARS_INTERJ,
    ORATIO_PARS_TACKON,
    ORATIO_PARS_PREFIX,
    ORATIO_PARS_SUFFIX,

    ORATIO_PARS_NUMERUS_PARTIUM
} OratioPars;

/* tags fluxus */
nomen enumeratio {
    ORATIO_VOC_CAPUT        = 1,
    ORATIO_VOC_STIRPS       = 2,
    ORATIO_VOC_FLEXIO       = 3,
    ORATIO_VOC_ADDITAMENTUM = 4,
    ORATIO_VOC_UNICUM       = 5
} OratioVocTag;

#define ORATIO_VOCABULARIUM_VERSIO 1

nomen structura {
    chorda dictline;
    chorda inflects;
    chorda addons;
    chorda uniques;
} OratioVocabulariumFontes;

nomen structura {
       i32 stirpes;
       i32 flexiones;
       i32 additamenta;
       i32 unica;
       i32 versio;
    chorda fons;        /* CAPUT: fons tabulae (commissio) */
} OratioVocabulariumCensus;

nomen structura {
    constans character* plagula;   /* "DICTLINE.GEN" ... aut "la.bin" */
                   i32  linea;     /* I-basata; ZEPHYRUM = fluxus (offset in octetis) */
                   s32  offset;
    constans character* causa;
} OratioVocabulariumVitium;

/* recorda decocta (chordae fluxum referunt, non copiantur) */
nomen structura {
        chorda stirpes[4];
    OratioPars pars;
           i32 declinatio;
           i32 varians;
        chorda species;
     character aetas;
     character area;
     character geo;
     character frequentia;
     character fons;
        chorda sensus;
           i32 linea;
} OratioStirps;

nomen structura {
    OratioPars pars;
           i32 declinatio;
           i32 varians;
        chorda accidentia;
           i32 clavis;
           i32 mensura;
        chorda finis;
     character aetas;
     character frequentia;
           i32 linea;
} OratioFlexio;

nomen structura {
    OratioPars genus;
        chorda forma;
        chorda descriptio;
        chorda sensus;
           i32 linea;
} OratioAdditamentum;

nomen structura {
    chorda forma;
    chorda descriptio;
    chorda sensus;
       i32 linea;
} OratioUnicum;

/* Fontes coquere in fluxum nuntii. *exitus = octeti tabulae (in
 * piscina); FALSUM = vitium (recordum non cadens, plagula/linea/causa
 * in *vitium) aut memoria. census (si datum) numeros recordorum fert. */
b32
oratio_vocabularium_coquere (
                              Piscina* piscina,
    constans OratioVocabulariumFontes* fontes,
                   constans character* fons_titulus,
                               chorda* exitus,
             OratioVocabulariumCensus* census,
             OratioVocabulariumVitium* vitium);

/* Tabulam recensere: fluxum totum ambulare, recordum quodque decoquere,
 * numeros contra caput conferre. FALSUM = vitium (offset, causa). */
b32
oratio_vocabularium_recensere (
                     Piscina* piscina,
                      chorda  tabula,
    OratioVocabulariumCensus* census,
    OratioVocabulariumVitium* vitium);

/* Decoctio recordi nidi (chorda ex nuntium_legere_nuntium); FALSUM =
 * campus ignotus aut deest. */
b32
oratio_vocabularium_stirps_legere (
         Piscina* piscina,
          chorda  nidus,
    OratioStirps* r);
b32
oratio_vocabularium_flexio_legere (
         Piscina* piscina,
          chorda  nidus,
    OratioFlexio* r);
b32
oratio_vocabularium_additamentum_legere (
               Piscina* piscina,
                chorda  nidus,
    OratioAdditamentum* r);
b32
oratio_vocabularium_unicum_legere (
         Piscina* piscina,
          chorda  nidus,
    OratioUnicum* r);

/* Sigillum tabulae: SHA-256, XVI hexadecimales primi (chorda in piscina). */
chorda
oratio_vocabularium_sigillum (
    Piscina* piscina,
     chorda  tabula);

/* Pars orationis ex titulo ("N", "ADJ", ...); ORATIO_PARS_NUMERUS_PARTIUM
 * si ignota. Titulus partis. */
OratioPars
oratio_pars_ex_titulo (
    chorda titulus);

constans character*
oratio_pars_titulus (
    OratioPars pars);

#endif /* ORATIO_VOCABULARIUM_H */
