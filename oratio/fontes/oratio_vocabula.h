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
 *   LATINUM   contextus ANGLICUS solum: tabula Latina aut entrium
 *             glossarii Latinum novit (notum Latine, non inventum)
 * PROSA (T15a, contextus Anglicus): nodi TEXTUS arboris md per arborem
 * orationis; glossarium (entria anglice licita) primum, Moby deinde,
 * tabula Latina tertia. Sedes numerantur (symbolorum, commentorum et
 * prosae seorsum), sedes PRIMA (via:linea) servatur. Relatio primum;
 * porta quae numerum ignotorum 'solum cadentem' pinnat post relationem
 * lectam.
 */

#ifndef ORATIO_VOCABULA_H
#define ORATIO_VOCABULA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "oratio_vocabularium_la.h"
#include "oratio_vocabularium_en.h"

/* genus sedis verbi: unde venit */
nomen enumeratio {
    ORATIO_SEDES_SYMBOLUM = 0,   /* identificator (nexus.tsv) */
    ORATIO_SEDES_COMMENTUM,      /* commentarium C */
    ORATIO_SEDES_PROSA           /* prosa markdown (contextus Anglicus) */
} OratioSedesGenus;

nomen enumeratio {
    ORATIO_VERBUM_NOTUM = 0,
    ORATIO_VERBUM_AMBIGUUM,
    ORATIO_VERBUM_PERMISSUM,
        ORATIO_VERBUM_IGNOTUM,
    ORATIO_VERBUM_LATINUM,     /* contextus Anglicus solum: tabula Latina novit */

    ORATIO_VERBUM_NUMERUS_STATUUM
} OratioVerbumStatus;

nomen structura {
    chorda verbum;             /* minusculum (copia) */
    OratioVerbumStatus status; /* post iudicare */
    i32 sedes;                 /* occurrentiae omnes */
        i32 sedes_symbolorum;
    i32 sedes_commentorum;
    i32 sedes_prosae;
    chorda classis;            /* analysis primaria: classis glossarii aut pars WORDS */
    chorda lemma;              /* analysis primaria */
    i32 analyses;
    i32 lemmata;               /* lemmata distincta */
    chorda via_prima;          /* sedes prima (copia) */
    i32 linea_prima;
            b32 ex_commento_prima;
    b32 ex_prosa_prima;
    chorda regula;             /* contextus Anglicus: regula morphologica analysis primae (vacua = forma exacta aut nulla) */
} OratioVerbum;

nomen structura OratioVocabula OratioVocabula;

/* voc: tabula Latina onerata (glossarium appensum si placet). */
OratioVocabula*
oratio_vocabula_creare (
                 Piscina* piscina,
    OratioVocabulariumLa* voc);

/* Contextus ANGLICUS (prosa markdown): glossarium (entria anglice
 * licita) primum, Moby (en) deinde, tabula Latina tertia -> status
 * LATINUM (notum Latine, non inventum); ambiguum numquam. voc = tabula
 * Latina cum glossario appenso. */
OratioVocabula*
oratio_vocabula_creare_anglice (
                 Piscina* piscina,
    OratioVocabulariumLa* voc,
    OratioVocabulariumEn* en);

/* Verbum unum (ut est; minusculatur) cum sede addere. */
b32
oratio_vocabula_verbum_addere (
        OratioVocabula* vc,
                chorda  verbum,
    constans character* via,
                   i32  linea,
                   b32  ex_commento);

/* Idem, genere sedis dato (prosa tertium genus). */
b32
oratio_vocabula_verbum_addere_sede (
        OratioVocabula* vc,
                chorda  verbum,
    constans character* via,
                   i32  linea,
      OratioSedesGenus  genus);

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

/* Viae EXCLUSAE lintus Anglici (prosa; decisio Frani 2026-09-04):
 * vendor/ et archivum/ ut supra, et plagulae GENERATAE (gesta/annales/
 * tabula.md = proiectio tabularii, md/CENSUS.md); knotapel/ INCLUSUM
 * (Anglicum est). */
externus constans character* constans ORATIO_PROSA_EXCLUSA[];

/* An via praefixo aliquo seriei (NIHIL terminatae; NIHIL = nullae)
 * incipiat. */
b32
oratio_vocabula_via_exclusa (
                          chorda  via,
    constans character* constans* exclusa);

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

/* Prosa plagulae markdown (contextus Anglicus): arbor md, nodi TEXTUS
 * soli (saepta, verbatim, destinationes nexuum, html, praefatio numquam
 * tanguntur), quisque per arborem orationis lectus; linea = lexematis
 * primi. */
b32
oratio_vocabula_prosa (
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
