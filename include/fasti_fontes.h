/* fasti_fontes.h - FONTES DIERUM (nomenclator-plan P6, decisio XIV):
 * portus generalis per quem dies signati (natales, ...) ad calendarium
 * veniunt sine dependentia calendarii a fontibus. Introitus TYPATI
 * (genus): calendarium regulam pingendi per genus habet; genus ignotum
 * = ordo planus cum titulo.
 *
 * Fons intervallum [prima, ultima] (inclusive) diebus CONCRETIS
 * respondet: introitus annui a FONTE iterantur
 * (fasti_annuum_expandere) - forma quam bibliotheca recursionis
 * (decisio XV) postea substituet; portus non mutatur. */
#ifndef FASTI_FONTES_H
#define FASTI_FONTES_H

/* <aedilis corpus="lib/fasti_fontes.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "fasti.h"

#define FASTI_GENUS_NATALIS   "natalis"
#define FASTI_FONTES_MAXIMI   VIII

nomen structura {
      Dies dies;            /* concreta, intra intervallum petitum */
    chorda genus;           /* "natalis"; ignotum: ordo planus */
    chorda titulus;         /* "Anna Rossi" */
       s32 annus_originis;  /* natalis: annus nativitatis; 0 ignotus */
    chorda fons;            /* titulus fontis (a portu ponitur) */
    chorda referentia;      /* id in fonte (persona) - ad aperiendum */
} FastiIntroitus;

/* fons: introitus intervalli in exitus (Xar de FastiIntroitus) addit */
nomen b32 (*FastiFonsLegere)(
       vacuum* ctx,
          Dies prima,
          Dies ultima,
          Xar* exitus,
      Piscina* piscina);

/* fons: introitum (referentia) aperire - e.g. latus contactus; VERUM
 * si apertum */
nomen b32 (*FastiFonsAperire)(
                    vacuum* ctx,
    constans FastiIntroitus* introitus);

nomen structura FastiFontes FastiFontes;

FastiFontes*
fasti_fontes_creare (
    Piscina* piscina);

/* fons registratur: titulus unicus, aperire NIHIL licet; FALSUM si
 * plenus (FASTI_FONTES_MAXIMI) aut titulus iteratus */
b32
fasti_fontes_addere (
            FastiFontes* f,
     constans character* titulus,
        FastiFonsLegere  legere,
       FastiFonsAperire  aperire,
                 vacuum* ctx);

/* introitus omnium fontium in [prima, ultima]: Xar de FastiIntroitus
 * ordine diei, deinde generis, deinde tituli; f NIHIL = vacuum */
Xar*
fasti_fontes_legere (
    constans FastiFontes* f,
                    Dies  prima,
                    Dies  ultima,
                 Piscina* piscina);

/* introitum fonti suo reddere ad aperiendum; FALSUM si fons ignotus
 * aut sine aperire */
b32
fasti_fontes_aperire (
          constans FastiFontes* f,
       constans FastiIntroitus* introitus);

/* textus introitus pro ordine: natalis "Anna Rossi turns 40" (annus
 * originis notus) aut "Anna Rossi's birthday"; genus ignotum: titulus
 * ipse (D4b: ex kalendarium.c translatum - conspectus 'dies' et
 * pictura idem textum habent) */
chorda
fasti_introitum_textus (
    constans FastiIntroitus* introitus,
                    Piscina* piscina);

/* annuum (mensis, dies) in [prima, ultima]: dies concreti in
 * exitus[maximum] ordine; 29 II in anno communi -> 28 II; redde
 * numerum */
i32
fasti_annuum_expandere (
     s32  mensis,
     s32  dies,
    Dies  prima,
    Dies  ultima,
    Dies* exitus,
     i32  maximum);

#endif /* FASTI_FONTES_H */
