/* iussum.h - iussa in textu, stilo acme (vicus-latera S3a)
 *
 * '$verbum' aut '$verbum(arg, arg)' in tabula characterum: verbum
 * '[a-z][a-z0-9_-]*' - litterae minusculae, numeri, '_' et '-' (intra
 * solum: '-' finale verbum non est - "$dies-" = $dies et '-'; S3e),
 * '$' in initio lineae aut post characterem non verbalem (littera,
 * numerus, '_'), ergo 'a$b' iussum non est.
 * Argumenta: '(' statim post verbum, ')' prima in EADEM linea;
 * commatibus divisa, spatia extrema dempta; '()' = nulla. '$verbum('
 * sine ')' iussum NON est (dimidium iussi numquam currit). Solum
 * verba NOTA iussa sunt (hospes respondet): cetera prosa manent
 * ('$5.00', '$foo(' - nullum effugium necessarium).
 *
 * <purus/>: tabulam legit, nihil mutat; chordae exitus in piscina
 * (copiae - tabula postea mutari potest). */

#ifndef IUSSUM_H
#define IUSSUM_H

/* <aedilis corpus="lib/iussum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_characterum.h"

/* verbum notum? NIHIL = omnia nota (probationes) */
nomen b32 (*IussumNotum)(chorda verbum, vacuum* ctx);

nomen structura {
       s32  linea;
       s32  initium;            /* columna '$' */
       s32  finis;              /* post ultimum (')' aut verbum) */
    chorda  verbum;             /* sine '$' */
       i32  numerus_argumentorum;
    chorda* argumenta;          /* in piscina; spatia extrema dempta */
} Iussum;

/* iussum quod cellulam (linea, columna) tegit (initium <= columna <
 * finis); FALSUM si nullum aut argumenta invalida */
b32
iussum_ad_locum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus);

/* iussum proximum in linea cuius '$' a columna data aut post iacet
 * (figura: omnia colorare - iterum a exitus->finis); FALSUM si
 * nullum */
b32
iussum_proximum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a_columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus);

/* ---- Nexus (S3d): '#verbum' ---- */

/* nexus '#verbum' ('[a-z0-9_-]+'; ante: initium lineae aut character
 * non verbalis); argumenta nulla - Iussum idem, verbum sine '#'. Omnis
 * nexus nexus est (non solum noti): tags ('#notae') per paginas
 * cycli, '#3' pagina id III, '#next' '#prev' '#first' '#last'. */
b32
iussum_nexus_ad_locum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                       Piscina* piscina,
                        Iussum* exitus);

/* nexus proximus in linea cuius '#' a columna data aut post iacet */
b32
iussum_nexus_proximus (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a_columna,
                       Piscina* piscina,
                        Iussum* exitus);

/* ---- Registrum (S3b): verba nota et quid faciunt ---- */

nomen structura IussumRegistrum IussumRegistrum;   /* opacum */

/* effectus iussi: textus inserendus et/aut error. textus: consumens
 * signum substituit, aliter post signum inseritur. error non vacuus:
 * nihil mutatur, nuntius ostenditur (S3b-2: linea status). */
nomen structura {
    chorda textus;
    chorda error;
} IussumEffectus;

nomen b32 (*IussumFunctio)(constans Iussum* iussum, vacuum* ctx,
                           Piscina* piscina, IussumEffectus* effectus);

IussumRegistrum*
iussum_registrum_creare (
    Piscina* piscina);

/* consumit: VERUM = signum ictu deletur (creatores, $dies); FALSUM =
 * signum manet ut bottone (aperientes, $terminale). Verbum iterum
 * registratum priorem substituit. FALSUM si verbum invalidum. */
b32
iussum_registrare (
        IussumRegistrum* r,
     constans character* verbum,
                    b32  consumit,
          IussumFunctio  functio,
                 vacuum* ctx);

/* forma IussumNotum: ctx = IussumRegistrum* */
b32
iussum_registrum_notum (
    chorda  verbum,
    vacuum* ctx);

b32
iussum_consumit (
    constans IussumRegistrum* r,
                      chorda  verbum);

/* effectus vacuatur, deinde functio verbi currit; FALSUM si verbum
 * ignotum aut functio FALSUM reddit */
b32
iussum_currere (
       IussumRegistrum* r,
       constans Iussum* iussum,
               Piscina* piscina,
        IussumEffectus* effectus);

#endif /* IUSSUM_H */
