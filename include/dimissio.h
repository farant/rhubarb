/* dimissio.h - SYSTEMA DIMISSIONIS (status-plan S5c; census XIII):
 * campi aspectus "popover" ramorum registratorum clauduntur scriptore
 * 'dimissio':
 *   Esc (ramo activo popover aperto) - consumitur; NISI gestus in
 *   cursu (motus.captura): interior est, Esc suum primum accipit;
 *   ictus extra subarborem DIALOGUS rami - in eodem organo consumitur,
 *   in alio transit; ictus ad actionem domini campi (aperiens) exemptus
 *   (ipse commutat);
 *   actio successa componentis 'claudit' - per PRESSIONEM (muris aut
 *   clavis) solum; motus consumptus (illuminatio) non claudit.
 * Assumptio expressa: popover unus per organum (DIALOGUS = intus). */

#ifndef DIMISSIO_H
#define DIMISSIO_H

/* <aedilis corpus="lib/dimissio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "organum.h"
#include "dispensator.h"

nomen structura Dimissio Dimissio;

Dimissio*
dimissio_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo);

/* ramum registrare (campi aspectus popover ex organo); spatium
 * componentium = via rami (status-plan S8c) */
b32
dimissio_ramum_addere (
                Dimissio* dimissio,
    constans InsulaRamus* ramus,
        constans Organum* organum);

/* hamos in dispensatore ponere (dispensator_medium_ponere) */
vacuum
dimissio_dispensatori_ligare (
       Dimissio* dimissio,
    Dispensator* d);

#endif /* DIMISSIO_H */
