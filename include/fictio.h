/* fictio.h - valores ficti sine JSON (norma-spec par. IV)
 *
 * Nomina, email, uuid, tempora, Latina VERA (numquam lorem ipsum),
 * numeri et textus intra fines, scrinium textuum difficilium. Utilis
 * extra JSON quoque (UI ficti, fixa STML, data probationum C).
 *
 * Omnis functio Sors VOCANTIS accipit: semen idem -> octeti idem.
 *
 * STATUS: PROBATUM a Frano 2026-10-08 (norma-plan-1 N0); implementatio in norma-plan-2.
 */
#ifndef FICTIO_H
#define FICTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "sors.h"

/* praenomen + nomen gentile ("Marcus Tullius", "Gaia Iulia") */
chorda
fictio_nomen (
       Sors* sors,
    Piscina* piscina);

/* praenomen.nomen@example.org|com|net - RFC 2606 SOLUM, numquam
 * inscriptio vera */
chorda
fictio_email (
       Sors* sors,
    Piscina* piscina);

/* forma v4: 8-4-4-4-12 hex minuscula */
chorda
fictio_uuid (
       Sors* sors,
    Piscina* piscina);

/* ISO 8601 UTC ('Z'), instans in [ab, ad] (epocha unix secundorum) */
chorda
fictio_tempus (
       Sors* sors,
        s64  ab,
        s64  ad,
    Piscina* piscina);

/* sententiae verae ex corpore (fons citatus in fictio.c), spatio
 * iunctae; sententiae >= I */
chorda
fictio_textus_latinus (
       Sors* sors,
        i32  sententiae,
    Piscina* piscina);

/* longitudo in RUNIS (code points) in [minimum, maximum]; alphabetum
 * UTF-8 (runae eliguntur), NIHIL = 'a'..'z' */
chorda
fictio_textus (
                  Sors* sors,
                   i32  minimum,
                   i32  maximum,
    constans character* alphabetum,
               Piscina* piscina);

s64
fictio_integer (
    Sors* sors,
     s64  minimum,
     s64  maximum);

f64
fictio_numerus (
    Sors* sors,
     f64  minimum,
     f64  maximum);

/* optiones NIHIL-terminatae; NIHIL si vacuae */
constans character*
fictio_eligere (
                     Sors* sors,
    constans character* constans* optiones);

/* SCRINIUM DIFFICILIUM: notae combinantes, littera non-BMP, textus
 * dextrorsum, virgulae + lineae obliquae, U+0000 effugiendum, spatia
 * sola, emoji cum modificatore, textus longus - omnia UTF-8 valida.
 * Seminatum: */
chorda
fictio_textus_difficilis (
       Sors* sors,
    Piscina* piscina);

/* ...et omnia ordine (probationes exhaustivae): */
i32
fictio_difficilia_numerus (vacuum);

chorda
fictio_difficile (
         i32  index,
     Piscina* piscina);

#endif /* FICTIO_H */
