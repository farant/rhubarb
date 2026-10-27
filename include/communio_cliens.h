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
    chorda error;
} CommunioStatus;

/* basis: "https://hospes" aut "http://127.0.0.1:portus" (sine '/'
 * finali); tessera XLIII signa (copiatur). NIHIL si argumenta mala. */
CommunioCliens*
communio_clientem_creare (
               Piscina* piscina,
               Reactor* reactor,
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

#endif /* COMMUNIO_CLIENS_H */
