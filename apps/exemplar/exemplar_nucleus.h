/* exemplar_nucleus.h - applicatio probans hospitii v3: inscriptio
 * (codice invitationis), sessiones, notae cum filis, salus
 *
 * Status totus in volumine UNO (data/exemplar.volumen, durabilitas
 * plena): acta usor.creatus, tessera.mutata, sessio.creata (SIGILLUM
 * signi, numquam signum), sessio.deleta, filum.receptum, nota.scripta,
 * nota.deleta; massae = fila. Ad initium status in memoria ex actis
 * replicatur. Secretum servi data/secretum (0600, extra volumen),
 * fila temporaria data/tmp (0700, idem systema filorum).
 *
 * Viae: GET / | GET,POST /inscribere | GET,POST /intrare | POST /exire
 * | GET,POST /notae | POST /notae/:id/delere | GET /fila/:sigillum |
 * GET /salus. Omnis POST: Origin congruens; post sessionem etiam CSRF.
 * Textus usoris omnis per unum auxilium HTML evaditur. Fila: attachment
 * + nosniff, dominus solus (alienus = 404). */
#ifndef EXEMPLAR_NUCLEUS_H
#define EXEMPLAR_NUCLEUS_H

#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include "hospitium.h"

nomen structura {
    constans character* via_datorum;      /* --data (creatur 0700) */
    constans character* hospes;           /* NIHIL = 127.0.0.1 */
                   i32  portus;           /* 0 = auto */
    constans character* invitatio;        /* NIHIL = clausa */
                   b32  procurator_fidus; /* VERUM post nginx */
                   b32  acta_accessus;
                   i32  frenum_conatus;   /* 0 = V */
               Reactor* reactor;          /* NIHIL = proprius */
    /* basis publica: "https://exemplar.golden.earth"; NIHIL =
     * http://<hospes>:<portus> (mcp-remotus P10) */
    constans character* basis;
    /* sonda: acta petitionum /mcp, /.well-known/..., /oauth/... (capita
     * redacta); scriptor NIHIL = stderr */
                   b32 sonda;
                vacuum (*sonda_scriptor)(vacuum* datum, chorda linea);
                vacuum* sonda_datum;
} ExemplarConfiguratio;

nomen structura Exemplar Exemplar;

/* NIHIL si data, volumen, secretum aut servus deficiunt (causa ad
 * stderr) */
Exemplar*
exemplar_creare (
    constans ExemplarConfiguratio* configuratio,
                          Piscina* piscina);

Hospitium*
exemplar_hospitium (
    Exemplar* exemplar);

Reactor*
exemplar_reactor (
    Exemplar* exemplar);

/* servus destruitur, volumen (checkpoint) clauditur */
vacuum
exemplar_claudere (
    Exemplar* exemplar);

#endif /* EXEMPLAR_NUCLEUS_H */
