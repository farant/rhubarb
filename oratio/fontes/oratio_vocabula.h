/* oratio_vocabula.h - Recensio vocabulorum: identificatores et commentaria
 * contra vocabularium (T10, spec par. IV 'consumptor: lint vocabulorum')
 *
 * IDENTIFICATORES: sedes tabulae build/nexus.tsv (definitiones), nomen
 * quodque in verba scissum ad '_' et ad limitem minuscula->CAPITALIS
 * (piscina_allocare -> piscina allocare; MateriaNodus -> materia nodus;
 * ORATIO_PARS_N -> oratio pars), minusculata; partes sine littera aut
 * litterae unius omittuntur. COMMENTARIA: plagula C per lexatorem
 * TOTALEM silvae (silva_lexare_cruda) - lexemata commentarii - deinde
 * corpus commentarii per arborem orationis (margines '*' interpunctio
 * fiunt), vocabula eius verba. Verbum quodque SEMEL quaeritur (glossarium
 * primum, tabula WORDS deinde) et status accipit:
 *   NOTUM     analyses >= I, lemma unum
 *   AMBIGUUM  lemmata distincta > I (virum: vir virus vis)
 *   PERMISSUM analysis glossarii classis ignotum-permissum (offset)
 *   IGNOTUM   nihil - INVENTUM, non vitium (Anglica gradus IV; nomina
 *             propria; vitia scripturae)
 * Sedes numerantur (symbolorum et commentorum seorsum), sedes PRIMA
 * (via:linea) servatur. Relatio primum; porta quae numerum ignotorum
 * 'solum cadentem' pinnat post relationem lectam.
 */

#ifndef ORATIO_VOCABULA_H
#define ORATIO_VOCABULA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_vocabularium_la.h"

nomen enumeratio {
    ORATIO_VERBUM_NOTUM = 0,
    ORATIO_VERBUM_AMBIGUUM,
    ORATIO_VERBUM_PERMISSUM,
    ORATIO_VERBUM_IGNOTUM,

    ORATIO_VERBUM_NUMERUS_STATUUM
} OratioVerbumStatus;

nomen structura {
    chorda verbum;             /* minusculum (copia) */
    OratioVerbumStatus status; /* post iudicare */
    i32 sedes;                 /* occurrentiae omnes */
    i32 sedes_symbolorum;
    i32 sedes_commentorum;
    chorda classis;            /* analysis primaria: classis glossarii aut pars WORDS */
    chorda lemma;              /* analysis primaria */
    i32 analyses;
    i32 lemmata;               /* lemmata distincta */
    chorda via_prima;          /* sedes prima (copia) */
    i32 linea_prima;
    b32 ex_commento_prima;
} OratioVerbum;

nomen structura OratioVocabula OratioVocabula;

/* voc: tabula Latina onerata (glossarium appensum si placet). */
OratioVocabula*
oratio_vocabula_creare (
                 Piscina* piscina,
    OratioVocabulariumLa* voc);

/* Verbum unum (ut est; minusculatur) cum sede addere. */
b32
oratio_vocabula_verbum_addere (
        OratioVocabula* vc,
                chorda  verbum,
    constans character* via,
                   i32  linea,
                   b32  ex_commento);

/* Identificatorem in verba scindere et addere (regula supra). */
b32
oratio_vocabula_identificatorem_addere (
        OratioVocabula* vc,
                chorda  identificator,
    constans character* via,
                   i32  linea);

/* Viae EXCLUSAE lintus Latini (decisio Frani 2026-09-04): knotapel/
 * (Anglicum consulto, laboratorium mathematicum), vendor/ (alienum),
 * archivum/ (generationes praecedentes). Series praefixorum NIHIL
 * terminata; instrumentum -omnes-viae eam praeterit. */
externus constans character* constans ORATIO_VOCABULA_EXCLUSA[];

/* Sedes tabulae nexus.tsv (lineae 'sedes'): identificatores, viis
 * praefixis in 'exclusa' (NIHIL terminata; NIHIL = nullae) omissis. */
b32
oratio_vocabula_symbola (
                  OratioVocabula* vc,
                          chorda  nexus_tsv,
    constans character* constans* exclusa);


/* Commentaria plagulae C: silva lexat, oratio legit, vocabula addit. */
b32
oratio_vocabula_commenta (
        OratioVocabula* vc,
                chorda  fons,
    constans character* via);

/* Verbum quodque quaerere, statum/classem/lemma ponere. */
b32
oratio_vocabula_iudicare (
    OratioVocabula* vc);

/* Verba ordine primi adventus (Xar de OratioVerbum). */
Xar*
oratio_vocabula_verba (
    constans OratioVocabula* vc);

/* Indices verborum status dati (aut omnium si status < 0) ordine sedium
 * DESCENDENTE, deinde verbi. Xar de s32. */
Xar*
oratio_vocabula_ordinata (
                    Piscina* piscina,
    constans OratioVocabula* vc,
                        s32  status);

i32
oratio_vocabula_numerus (
    constans OratioVocabula* vc,
         OratioVerbumStatus  status);

i32
oratio_vocabula_sedes (
    constans OratioVocabula* vc);

constans character*
oratio_verbum_status_titulus (
    OratioVerbumStatus status);

#endif /* ORATIO_VOCABULA_H */
