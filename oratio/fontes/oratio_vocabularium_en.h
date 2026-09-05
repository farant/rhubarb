/* oratio_vocabularium_en.h - Vocabularium Anglicum ex Moby Part-of-Speech
 *
 * Fons: oratio/vocabularium/en/mobypos.txt (Gutenberg #3203, dominium
 * publicum), VERBATIM venditum, sine coctione: fons ipse tabula est
 * (recordum = `forma\codices` CRLF; codices litterae ordine
 * praelationis, prima = pars principalis; FONTES.md legendam fert).
 * Lector eum semel legit (lex recordi in OMNI linea asserta: CRLF,
 * signum '\' unum, forma et codices non vacui - linea non cadens onus
 * sistit cum linea nominata), verba (formae sine spatio) in tabulam
 * dispersam per formam PLICATAM (minusculae ASCII) catenat; locutiones
 * (cum spatio) numerantur solum. Littera codicis extra legendam
 * INVENTUM est: numeratur, recordum primum nominatur, onus non sistit.
 *
 * Quaesitio: forma -> recorda (indices) ordine plagulae; vacua =
 * ignotum (inventum, non vitium). Morphologia (-s -ed -ing -ly) NULLA
 * hic: gradus IV eam ex relatione lintus ut DATA addet.
 */

#ifndef ORATIO_VOCABULARIUM_EN_H
#define ORATIO_VOCABULARIUM_EN_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_vocabularium.h"

/* recordum (chordae fontem referunt, non copiantur) */
nomen structura {
    chorda forma;      /* ut in fonte (octeti CP437 possibiles: 0x8E = e acutum) */
    chorda codices;    /* litterae legendae ordine praelationis ("Nt", "Dv") */
       i32 linea;      /* I-basata in mobypos.txt */
} OratioVocabulumEn;

nomen structura {
       i32 recorda;          /* lineae */
       i32 verba;            /* formae sine spatio: indexatae */
       i32 locutiones;       /* formae cum spatio: numeratae, non indexatae */
       i32 octeti_alti;      /* recorda cum octeto >= 0x80 */
       i32 codices_ignoti;   /* recorda cum littera extra legendam */
    chorda codex_ignotus_primus;   /* forma recordi primi talis (vacua = nullum) */
} OratioVocabulariumEnCensus;

nomen structura OratioVocabulariumEn OratioVocabulariumEn;

/* Regula morphologica (T15b, DATA): forma quae suffixo finitur ->
 * basis = forma sine suffixo + substitutio (geminatio: consonans duplex
 * finalis simplificatur, planned -> plan); basis in tabula quaeritur et
 * codicem unum ex 'codices' ferre debet (NIHIL = quilibet); analysis
 * classem 'classis' accipit (NIHIL = ex codice basis congruente: N p h
 * substantivum, V t i verbum). Suffixum "-" = COMPOSITUM: partes ad
 * hyphen scissae omnes formae exactae esse debent (non-null, a-plot).
 * Ordo tabulae = ordo analysium; forma exacta semper prior. Causa =
 * verba relationis diei primi cum numeris sedium. */
nomen structura {
    constans character* titulus;
    constans character* suffixum;
    constans character* substitutio;
                   b32  geminatio;
                   i32  basis_minima;   /* litterae basis minimae */
    constans character* codices;        /* NIHIL = quilibet */
    constans character* classis;        /* NIHIL = ex basi */
    constans character* causa;
} OratioRegulaEn;

externus constans OratioRegulaEn ORATIO_REGULAE_EN[];
externus constans i32 ORATIO_REGULAE_EN_NUMERUS;

/* Praefixa compositorum (non, multi, semi ...): pars ante hyphen quae
 * verbum non est; NIHIL terminata. */
externus constans character* constans ORATIO_PRAEFIXA_EN[];

/* analysis formae: recordum Moby (basis aut forma ipsa) cum regula */
nomen structura {
                   s32  recordum;   /* index recordi; -I = compositum (nullum recordum unum) */
                   s32  regula;     /* -I = forma exacta; alioquin index in ORATIO_REGULAE_EN */
                chorda  basis;      /* forma plicata aut basis derivata (copia in piscina) */
    constans character* classis;    /* classis universalis (NIHIL numquam: 'ignotum' si codex extra legendam) */
} OratioAnalysisEn;

/* Legenda codicum Moby (casus significans): N p h V t i A v C P ! r D I o */
externus constans character* constans ORATIO_VOCABULARIUM_EN_CODICES;

/* Fontem legere (octeti in memoria manent - recorda eos referunt).
 * NIHIL = vitium (plagula "mobypos.txt", linea, causa) aut memoria. */
OratioVocabulariumEn*
oratio_vocabularium_en_onerare (
                     Piscina* piscina,
                      chorda  fons,
    OratioVocabulariumVitium* vitium);

/* Forma plicata (copia in piscina): minusculae ASCII; octeti ceteri ut
 * sunt. Una functio clavibus et quaesitionibus. */
chorda
oratio_vocabularium_en_plicare (
    Piscina* piscina,
     chorda  forma);

/* Recorda formae (indices, Xar de s32) ordine plagulae; vacuus =
 * ignotum. NIHIL = memoria. Locutiones (cum spatio) numquam inveniuntur. */
Xar*
oratio_vocabularium_en_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                           chorda  forma);

constans OratioVocabulumEn*
oratio_vocabularium_en_recordum (
    constans OratioVocabulariumEn* voc,
                              s32  i);

/* Analyses formae (Xar de OratioAnalysisEn): recorda exacta primum
 * (regula -I), deinde regulae tabulae ordine; vacuus = ignotum. NIHIL =
 * memoria. */
Xar*
oratio_vocabularium_en_analysare (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                           chorda  forma);

OratioVocabulariumEnCensus
oratio_vocabularium_en_census (
    constans OratioVocabulariumEn* voc);

/* Classis universalis litterae codicis (spec par. VI): N p h o ->
 * substantivum; V t i -> verbum; A -> adiectivum; v -> adverbium; C ->
 * coniunctio-coordinans; P -> adpositio; ! -> interiectio; r ->
 * pronomen; D I -> determinans. NIHIL = littera extra legendam. */
constans character*
oratio_vocabularium_en_classis (
    i8 codex);

#endif /* ORATIO_VOCABULARIUM_EN_H */
