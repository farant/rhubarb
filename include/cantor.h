/* cantor.h - Cantor: carmen in tempore sonat (sequentia; sonus-spec
 * VIII-d)
 *
 * EXEMPLAR VIVUM: cantor carmen (carmen.h) in quoque pulsu LEGIT -
 * nihil praecomputatur; mutatio exemplaris (pagina musicae postea)
 * proximo transitu auditur. Positio in PULSIBUS (tempus musicum);
 * quadra ex mappa temporis carminis, ancorata ubi sonare coepit
 * (basis).
 *
 * PROSPECTUS: omni pulsu notae quae intra CANTOR_PROSPECTUS ms
 * incipiunt iubentur - TIBIAM ad quadrum initii exactum,
 * PORTAM_CLAUDERE ad finem; nota '@praeparatum' = sonitus unicus
 * (percussio). Lucrum = nota x pars x carmen_intensio (dynamica ad
 * initium notae); voces tibiae sonantes dynamicam sequuntur:
 * VOCEM_PONERE per octavas (et ad mutationes) intra prospectum,
 * numquam praeiussa - iussa pendentia finita manent. Gradus et clivus
 * eadem positione: gradus primum (evanescentia), clivus ab eius fine
 * (pp deinde crescendo: saltus non perit). Mutus omittitur,
 * solus (si quis) alios tacet. Prioritas II: cantilenae (I) voces
 * musicae non surripiunt.
 *
 * REGIO: [initium, finis) pulsuum; ad finem iterans basis exacte
 * longitudine regionis in quadris promovetur - sine hiatu, sine
 * tegimine.
 *
 * SISTERE / QUAERERE: iussa iam missa revocari nequeunt - cantor voces
 * suas meminit (ansa, initium, finis): sonans per evanescentiam V ms ab
 * nunc, nondum incepta SISTERE ad initium suum (clivo 0: nullum
 * exemplum sonat).
 *
 * TENET exitum (concentus_tenere) dum notas iubendas habet - ab
 * incipere / quaerere usque ad finem regionis sine iteratione aut
 * cantor_sistere: pausa longa exitum non claudit. Cantor sonans sine
 * cantor_sistere relictus exitum apertum tenet.
 *
 * PORTAMENTUM: nota 'continuatur' portam non claudit; nota proxima
 * vocem eius (pars eadem, finis = initium suum, transitus idem) per
 * NOTAM_PONERE ad initium suum glissat (clivus = portamentum in
 * quadris); lucrum ad initium suum (nota x pars x dynamica ibi) per
 * evanescentiam si ab eo quod vox tenet differt (lucrum notae aut
 * gradus dynamicae ibi); puncta dynamicae notae prioris ante initium
 * novum prius iubentur (eodem prospectu non pereunt); portam ad finem
 * suum claudit (nisi ipsa continuatur). Vox non inventa (quaerere,
 * iteratio, prior tacita) = nota ut solet incipit. Nota continuata ad
 * finem regionis (proxima extra) portam ut solet claudit - vox numquam
 * pendet: post prospectum quemque vox continuata cuius proxima non
 * venit (pars muta, exemplar mutatum, '@') ad finem suum clauditur.
 * Sonitus ('@') non glissant.
 *
 * Instrumenta: primum carminis ipsius (carmen_instrumentum: carminis,
 * libri), deinde per resolutorem (CantorResolutor): instrumentum partis
 * -> TibiaParametra, '@titulus' -> SonitusParametra; patches usoris
 * sine mutatione cantoris. NIHIL redditum = pars aut nota tacet.
 *
 * USUS:
 *   c = cantor_creare(piscina, concentus, NIHIL);
 *   cantor_regio(c, VERUM, 0, 0);
 *   (vacuum)cantor_incipere(c, carmen, 0);
 *   (vacuum)concentus_agere(k, nunc);   (* omni pulsu, *)
 *   (vacuum)cantor_agere(c);            (* hoc ordine *)
 */
/* <aedilis corpus="lib/cantor.c"/> */
#ifndef CANTOR_H
#define CANTOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "sonitus.h"
#include "tibia.h"
#include "concentus.h"
#include "carmen.h"

/* prospectus iubendi, ms */
#define CANTOR_PROSPECTUS CCL
/* voces cantoris memoratae (iussae aut sonantes) simul */
#define CANTOR_VOCES      LXIV
/* evanescentia sistendi, quadra (V ms) */
#define CANTOR_EVANESCENTIA CCXL

/* gradus dynamicae vocis sonantis: octava (pulsus) */
#define CANTOR_GRADUS_INTENSIONIS CDLXXX
/* iterationes regionis per pulsum maximae: regio brevissima pulsum
 * vici non sistit - reliqua pulsu proximo, sine hiatu */
#define CANTOR_ITERATIONES XVI

nomen structura {
    /* NIHIL functio = ordinarius (cantor_tibia_ordinaria,
     * cantor_sonitus_ordinarius); NIHIL redditum = pars aut nota tacet
     * (non vitium) */
    constans TibiaParametra*   (*tibia)(vacuum* ctx,
        chorda instrumentum);
    constans SonitusParametra* (*sonitus)(vacuum* ctx,
        chorda praeparatum);
    vacuum* ctx;
} CantorResolutor;

nomen structura Cantor Cantor;

/* resolutor ordinarius, publicus: instrumentum -> tibia_praeparatum
 * (titulus < LXIV octeti), '@titulus' -> sonitus_praeparatum. Resolutor
 * proprius (patches usoris) ignota sua huc remittat. ctx non adhibetur;
 * NIHIL = ignotum */
constans TibiaParametra*
cantor_tibia_ordinaria (
     vacuum* ctx,
     chorda  instrumentum);

constans SonitusParametra*
cantor_sonitus_ordinarius (
     vacuum* ctx,
     chorda  praeparatum);

/* resolutor copiatur (NIHIL = ordinarius); concentus vivat dum cantor.
 * NIHIL si memoria deest */
Cantor*
cantor_creare (
                     Piscina* piscina,
                   Concentus* concentus,
    constans CantorResolutor* resolutor);

/* carmen a pulsu dato sonare incipit (concentum aperit); carmen vivat
 * dum sonat. FALSUM: exitus recusat (nihil sonat) */
b32
cantor_incipere (
             Cantor* c,
    constans Carmen* carmen,
                s64  pulsus);

/* regio [initium, finis) pulsuum; finis 0 = carmen totum; iterare
 * FALSUM = semel ad finem */
vacuum
cantor_regio (
    Cantor* c,
       b32  iterare,
       s64  initium,
       s64  finis);

/* omni pulsu, POST concentus_agere: notas intra prospectum iubet, voces
 * finitas obliviscitur. VERUM dum sonat */
b32
cantor_agere (
    Cantor* c);

/* voces omnes cantoris tacent, etiam iussae nondum sonantes */
vacuum
cantor_sistere (
    Cantor* c);

/* positio nova (sonans manet): voces priores tacent, ab hoc pulsu */
vacuum
cantor_quaerere (
    Cantor* c,
       s64  pulsus);

/* positio currens in carmine (horologium exitus); 0 si non sonat */
s64
cantor_pulsus (
    Cantor* c);

b32
cantor_sonat (
    constans Cantor* c);

#endif /* CANTOR_H */
