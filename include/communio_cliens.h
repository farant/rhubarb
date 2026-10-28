/* communio_cliens.h - cliens communionis in apparatu (project-specs/
 * communio-spec.md II.3, II.9; plan communio-plan-2 T6)
 *
 * Machina status in reactore, semitae DUAE independentes ne acta
 * propria post tractionem dilatam (XXV s) exspectent:
 *
 *   ASCENSUS  replicam semel ligat; deinde acta propria post cursorem
 *             (derivata praetermissa) per fasciculos - CC acta et
 *             DXII KB ad plurimum - massis prius missis (desunt ->
 *             PUT); omni
 *             secunda (aut communio_cliens_excitare) acta nova quaerit
 *   DESCENSUS tractio post cursorem servi cum mora (long-poll); massae
 *             nominatae quae desunt prius petuntur, deinde acta ordine
 *             importantur; advenerunt vocatur si quid importatum
 *
 * Cursores in praesentia voluminis (numquam communicantur): clavis
 * "communio/<basis>/ascensus" (seq locale actus proprii ultimi missi)
 * et ".../descensus" (seq servi ultimi tracti), textus decimalis, post
 * omnem fasciculum acceptum scripti. Defectus (rete, servus, tessera):
 * error in statu, semita post XXX s iterum (aut excitare statim).
 *
 * TESSERA in memoria clientis sola (copia; destruere eam delet);
 * numquam in volumine, in errore, in nuntio. HTTP in functione una
 * (C7: phase 5 URLSession per servitium hospitis eam substituet).
 */

#ifndef COMMUNIO_CLIENS_H
#define COMMUNIO_CLIENS_H

/* <aedilis corpus="lib/communio_cliens.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "volumen.h"
#include "reactor.h"
#include "moneta.h"
#include "veredarius.h"

nomen structura CommunioCliens CommunioCliens;

/* post acta importata (numerus > 0): applicatio documenta aperta
 * sequitur (pictor_documentum_sequi) */
nomen vacuum (*CommunioAdvenerunt)(vacuum* ctx, i32 numerus);

/* error: copia in memoria clientis (vacuus si nullus) */
nomen structura {
       s64 ascensus;
       s64 descensus;
       i32 missa;        /* acta accepta a servo (summa) */
       i32 recepta;      /* acta importata (summa) */
       b32 connexus;     /* petitio ultima successit */
       b32 recusata;     /* plan-5 T5: servus tesseram recusavit (401) -
                          * cliens desiit; numquam iterum petit */
    chorda error;
} CommunioStatus;

/* plan-5 T5: semel vocatur cum servus tesseram recusat (401: apparatus
 * revocatus) - cliens iam desiit */
nomen vacuum (*CommunioRecusata)(vacuum* ctx);

/* basis: "https://hospes" aut "http://127.0.0.1:portus" (sine '/'
 * finali); tessera XLIII signa (copiatur); veredarius = via HTTP
 * unica (plan-5 T1: hospes eum praebet - veredarius_domesticus in
 * macOS et instrumentis, URLSession in iOS). NIHIL si argumenta
 * mala. */
CommunioCliens*
communio_clientem_creare (
               Piscina* piscina,
               Reactor* reactor,
            Veredarius* veredarius,
               Volumen* volumen,
    constans character* basis,
                chorda  tessera,
    CommunioAdvenerunt  advenerunt,
                vacuum* ctx);

/* mora tractionis in secundis (ordinarium XXV; 0 = sine exspectatione:
 * tractio vacua iterum post secundam) - ante aut post incipere */
vacuum
communio_cliens_moram_ponere (
    CommunioCliens* cliens,
               i32  secundae);

vacuum
communio_cliens_incipere (
    CommunioCliens* cliens);

/* plan-5 T5: nuntius recusationis (NIHIL licet) */
vacuum
communio_cliens_recusationem_ponere (
      CommunioCliens* cliens,
    CommunioRecusata  functio,
              vacuum* ctx);

/* nunc: ascensus acta nova quaerit; semita in mora (error aut tractio
 * sine exspectatione) statim pergit; otiosus deletur */
vacuum
communio_cliens_excitare (
    CommunioCliens* cliens);

/* petitiones in itinere abrumpit, horas cancellat; incipere iterum
 * licet */
vacuum
communio_cliens_sistere (
    CommunioCliens* cliens);

/* sistit, tesseram delet, arenas reddit; postea non adhibetur */
vacuum
communio_clientem_destruere (
    CommunioCliens* cliens);

CommunioStatus
communio_cliens_status (
    constans CommunioCliens* cliens);

/* nihil mittendum (quaestio ultima vacua) ET tractio ultima vacua sine
 * 'plura' - utrumque post excitare ultimum; nulla petitio in itinere */
b32
communio_cliens_otiosus (
    constans CommunioCliens* cliens);


/* ==================================================
 * Tessera in filo (T7) - secretum: numquam per lectiones
 * ================================================== */

/* tesseram novam cudit (communio_tesseram_facere) et in filum NOVUM
 * modo 0600 scribit (O_EXCL: numquam supra exsistens; O_NOFOLLOW);
 * digestum (LXIV hex + NUL) solum redditur ut administrator eum
 * inscribat */
b32
communio_tesseram_scribere (
     constans character* via,
              character  digestum[LXV]);

/* tesseram ex filo legit: filum regulare, non nexus symbolicus, modus
 * non latior quam 0600, XLIII signa (linea finalis tolerata) - aliter
 * FALSUM et causa in stderr (sine tessera) */
b32
communio_tesseram_legere (
     constans character* via,
              character  exitus[XLIV]);


/* ==================================================
 * Inscriptio per epistulam (plan-5 T2) - NUMQUAM OBSTRUIT
 *
 * Apparatus tesseram suam cudit (filum novum 0600), servus epistulam
 * cum nexu et codice mittit (si epistula permissa est - responsum
 * idem utroque modo), deinde: nexus pressus (tessera inspicitur omni
 * mora, ordinarie II s) aut codex datus. Pendens in praesentia
 * voluminis "communio/inscriptio" = {aditus, incepta_ms} (numquam
 * epistula): applicatio necata eam resumit. Omnia per reactorem;
 * mutata in quaque mutatione gradus.
 * ================================================== */

nomen enumeratio {
    INSCRIPTIO_PETIT = ZEPHYRUM,  /* intrare in itinere */
    INSCRIPTIO_EXSPECTAT,         /* epistula missa: nexus aut codex */
    INSCRIPTIO_CONFIRMAT,         /* codex in itinere */
    INSCRIPTIO_INSCRIPTA,         /* finis: tessera valet */
    INSCRIPTIO_CODEX_FALSUS,      /* codex recusatus; alius licet */
    INSCRIPTIO_EXPIRATA,          /* finis: XV min aut V conatus */
    INSCRIPTIO_RETE               /* finis: servus non attingitur */
} InscriptioGradus;

nomen vacuum (*CommunioInscriptioMutata)(vacuum* ctx,
    InscriptioGradus gradus);

nomen structura CommunioInscriptio CommunioInscriptio;

/* tesseram in via_tesserae cudit (exsistens: NIHIL, filum intactum) et
 * intrare mittit; volumen NIHIL = nihil servatur (instrumentum).
 * Redit ante responsum (gradus PETIT). Finis irritus (EXPIRATA, RETE)
 * filum tesserae et entrium delet. */
CommunioInscriptio*
communio_inscriptionem_incipere (
                     Piscina* piscina,
                     Reactor* reactor,
                  Veredarius* veredarius,
                     Volumen* volumen,
          constans character* basis,
                      chorda  epistula,
          constans character* via_tesserae,
                      chorda  titulus,
    CommunioInscriptioMutata  mutata,
                      vacuum* ctx);

/* inscriptionem pendentem resumere (gradus EXSPECTAT). NIHIL si nihil
 * pendet, filum tesserae abest aut XV minuta transierunt - tum filum
 * et entrium purgantur. */
CommunioInscriptio*
communio_inscriptionem_resumere (
                     Piscina* piscina,
                     Reactor* reactor,
                  Veredarius* veredarius,
                     Volumen* volumen,
          constans character* basis,
          constans character* via_tesserae,
    CommunioInscriptioMutata  mutata,
                      vacuum* ctx);

/* codex normatur (lineolae, spatia, minusculae); non VIII signa
 * Crockford aut gradus nec EXSPECTAT nec CODEX_FALSUS -> FALSUM sine
 * petitione */
b32
communio_inscriptioni_codicem_dare (
    CommunioInscriptio* ci,
                chorda  codex);

InscriptioGradus
communio_inscriptionis_gradus (
    constans CommunioInscriptio* ci);

/* inspectio tesserae omni 'ms' (ordinarie MM) */
vacuum
communio_inscriptionis_moram_ponere (
    CommunioInscriptio* ci,
                   i32  ms);

/* vita in ms (ordinarie XV minuta = vita aditus servi; probationes
 * breviorem ponunt) */
vacuum
communio_inscriptionis_vitam_ponere (
    CommunioInscriptio* ci,
                   i32  ms);

/* petitiones abrumpit, horas cancellat; nisi INSCRIPTA, filum tesserae
 * et entrium delet. Postea non adhibetur. */
vacuum
communio_inscriptionem_abicere (
    CommunioInscriptio* ci);

/* plan-5 T7: applicatio finitur medio - petitiones et horae sistuntur,
 * filum tesserae et entrium MANENT (resumere postea). Postea non
 * adhibetur. */
vacuum
communio_inscriptionem_relinquere (
    CommunioInscriptio* ci);

#endif /* COMMUNIO_CLIENS_H */
