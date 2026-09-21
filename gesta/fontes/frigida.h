/* frigida.h - via scripturae FRIGIDA: linea imperii in machinam
 * tabularii (2026-09-21, parcum K4.5 01KY57P2WD)
 *
 * CUR EXSTAT: uncus commissi et pythonica MCP loqui NON possunt.
 * 'Commissum opus claudit' et 'porta progressum scribit' viam
 * scripturae ex crusta poscunt. Haec unitas eam dat.
 *
 * IANUA UNA: verba nova (-status, -mutatio, -nexus) per MACHINAM
 * TABULARII ipsam eunt (tabularium_tractare, vocatio 'gerere') -
 * non per gesta_scribere directe. Ergo regulae eaedem (verba
 * canonica, praeiudicium vinculi), proiectiones eaedem (tabula.md,
 * entitates), recusationes eaedem. Via circum regulas NON exstat,
 * quia ianua altera non est.
 *
 * Formae veteres (nota, -crea) in instrumenta/nota_frigida.c
 * CONSULTO directae manent: ianua tumultuaria muta et robusta sit
 * cum machina ipsa aegrotat.
 *
 * LEGES RECUSATIONUM (decretum 01M32X21NR): causae OMNES simul,
 * et IMPERIUM VALIDUM ostensum - scriptura valida machinae in
 * lineam imperii versa, valoribus vocantis impleta.
 *
 * Divisio machina/principale (exemplar legati et tabularii): haec
 * unitas probabilis est (configuratio data, effusiones FILE* datae);
 * nota_frigida.c involucrum tenue manet. */

#ifndef FRIGIDA_H
#define FRIGIDA_H

#include "latina.h"
#include "tabularium.h"
#include <stdio.h>

/* codices exitus (contractus domus: II = NIHIL temptatum) */
#define FRIGIDA_EXITUS_SCRIPTUM   0
#define FRIGIDA_EXITUS_RECUSATUM  1
#define FRIGIDA_EXITUS_USUS       2

/* estne argv[I] verbum quod haec unitas tractat? (principale
 * formas veteres ipsum servat) */
b32
frigida_verbum_novit (
    constans character* vexillum);

/* Usus (argv[0] praeteritur):
 *   [-actor A] [-origo O] -status  <res> <novus>
 *   [-actor A] [-origo O] -mutatio <res> <clavis> <valor>
 *   [-actor A] [-origo O] -nexus   <res> <verbum> <alterum>
 *   -res <res>            (LECTIO: breviarium; nihil scribit)
 * actor ordinarius "fran", origo ordinaria "frigida".
 * effusio = responsum machinae (successus); errores = recusationes
 * et usus. Reddit FRIGIDA_EXITUS_*. */
integer
frigida_currere (
    constans TabulariumConfiguratio*  cfg,
                            integer   argc,
                          character** argv,
                               FILE*  effusio,
                               FILE*  errores);

#endif /* FRIGIDA_H */
