/* plist.h - Tabulae proprietatum (XML property lists): legere,
 * scribere, conferre.
 *
 * Grammatica plist SOLA hic vivit. Stratum XML infra iam escapat et
 * entitates nominatas decodificat (lib/xml.c:564-615, :898-950), ergo
 * haec bibliotheca arborem XmlNodus struit ut scribat et eam ambulat
 * ut legat; prologus et declaratio generis duae lineae literales sunt.
 *
 * Consumptores: fasciculum (Info.plist), postea iura et LaunchAgentes.
 * Specificatio: project-specs/plist-spec.md v1.
 *
 * NON tractat: plists binarias (bplist00), validationem DTD,
 * versionem praeter 1.0 - singula RECUSANTUR nominatim.
 */

#ifndef PLIST_H
#define PLIST_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "fasti.h"
#include "xar.h"

nomen enumeratio {
    PLIST_DICTIO,
    PLIST_LISTA,
    PLIST_CHORDA,
    PLIST_VERITAS,
    PLIST_INTEGER,
    PLIST_REALIS,
    PLIST_DIES,
    PLIST_DATA
} PlistGenus;

nomen enumeratio {
    PLIST_SUCCESSUS = ZEPHYRUM,
    PLIST_ERROR_XML,
    PLIST_ERROR_NON_PLIST,
    PLIST_ERROR_VERSIO,
    PLIST_ERROR_BINARIUM,
    PLIST_ERROR_STRUCTURA,
    PLIST_ERROR_BASE64,
    PLIST_ERROR_NUMERUS,
    PLIST_ERROR_DIES,
    PLIST_ERROR_SIGNUM,
    PLIST_ERROR_MEMORIA
} PlistStatus;

nomen structura PlistValor PlistValor;

/* Par clavis-valor dictionis; ordo SERVATUR (par. III specificationis:
 * iter reditus stabile, differentia legibilis) */
nomen structura {
        chorda  clavis;
    PlistValor* valor;
} PlistPar;

structura PlistValor {
    PlistGenus  genus;
        chorda  textus;   /* CHORDA: textus; DATA: octeti decocti */
      DiesHora  dies;     /* DIES */
           s64  integrum; /* INTEGER ('integer' est macrum latina.h) */
           f64  realis;   /* REALIS */
           b32  veritas;  /* VERITAS */
           Xar* liberi;   /* DICTIO: PlistPar; LISTA: PlistValor* */
};


/* ==================================================
 * Structores - unus per genus
 * ================================================== */

PlistValor*
plist_dictio_creare (
    Piscina* piscina);

PlistValor*
plist_lista_creare (
    Piscina* piscina);

PlistValor*
plist_chordam_creare (
      chorda  textus,
     Piscina* piscina);

PlistValor*
plist_veritatem_creare (
         b32  valor,
     Piscina* piscina);

PlistValor*
plist_integrum_creare (
         s64  numerus,
     Piscina* piscina);

PlistValor*
plist_realem_creare (
         f64  numerus,
     Piscina* piscina);

PlistValor*
plist_diem_creare (
    DiesHora  dh,
     Piscina* piscina);

PlistValor*
plist_data_creare (
      chorda  octeti,
     Piscina* piscina);


/* ==================================================
 * Aditus
 * ================================================== */

/* Ponere par: clavem exsistentem SUBSTITUIT in loco, aliter APPENDIT -
 * ergo editio plist ordinem numquam permutat. */
b32
plist_dictio_ponere (
     PlistValor* dictio,
         chorda  clavis,
     PlistValor* valor,
        Piscina* piscina);

b32
plist_lista_addere (
    PlistValor* lista,
    PlistValor* valor,
       Piscina* piscina);

/* Clavis ut literae C: lectio fere semper literalis est in sede
 * vocationis. Geminus _chorda in PULL manet, non ante. */
PlistValor*
plist_dictio_capere (
            PlistValor* dictio,
    constans character* clavis);

/* Numerus parium (dictio) aut elementorum (lista); ZEPHYRUM pro genere
 * scalari. */
i32
plist_numerus (
    PlistValor* valor);

PlistValor*
plist_ad_indicem (
     PlistValor* lista,
            i32  index);

/* Aequalitas VALORUM (non octetorum): genus, numerus, ORDO, contenta.
 * Fasciculum ea utitur ut plist iam rectam non rescribat. */
b32
plist_aequalis (
    PlistValor* a,
    PlistValor* b);


/* ==================================================
 * Scribere
 * ================================================== */

/* Purum: nihil in disco tangit. Emittit prologum, declarationem
 * generis Apple, plist versionis 1.0, arborem.
 *
 * RECUSAT signum C0 in textu aut clave (PLIST_ERROR_SIGNUM): stratum
 * escapandi nostrum id exprimere non potest, et crudum XML invalidum
 * pareret (par. VI.7 specificationis). Redde chordam vacuam nisi
 * status == PLIST_SUCCESSUS.
 */
chorda
plist_scribere (
     PlistValor* radix,
    PlistStatus* status,
        Piscina* piscina);

b32
plist_scribere_plagulam (
            PlistValor* radix,
    constans character* via,
           PlistStatus* status,
               Piscina* piscina);


/* ==================================================
 * Legere
 * ================================================== */

/* Recusationes SEDEM nominant, non lineam: XmlNodus positionem non
 * fert (include/xml.h:68), ergo semita ut "plist/dict/key[3]". */
nomen structura {
     PlistStatus  status;
      PlistValor* radix;   /* NIHIL nisi PLIST_SUCCESSUS */
          chorda  semita;
          chorda  causa;
} PlistResultus;

PlistResultus
plist_legere (
                 chorda  textus,
                Piscina* piscina,
    InternamentumChorda* intern);

PlistResultus
plist_legere_plagulam (
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern);

#endif /* PLIST_H */
