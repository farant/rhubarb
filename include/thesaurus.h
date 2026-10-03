/* thesaurus.h - THESAURUS contentorum (fabrica plan 2 T3, spec 2
 * par. III): blobi per sigillum contentorum, actiones per clavem,
 * generationes ad purgationem. Solum filum + sigillum: quodvis
 * instrumentum domus eo uti potest (nulla sqlite).
 *
 * Forma sub radice (build/aedilis/obiecta/ in arbore):
 *   blobi/<II hex>/<LXII hex>       octeti blobi
 *   actiones/<II hex>/<LXII hex>    sigilla exituum, unum per lineam
 *   generationes/<THESAURUS_GENERATIO>.lst  claves cursus unius
 *
 * SCRIPTURA ATOMICA: nomen temporarium (pid, numerus) deinde rename -
 * lector numquam blobum dimidium videt, scriptores duo eosdem octetos
 * sine discordia ponunt. Blobus iam praesens NON rescribitur; actio
 * eadem NON rescribitur (doctrina 'numquam rescribe immutatum').
 *
 * PURGATIO cum scriptore currente non est tuta (blobum ante actionem
 * positum deleret): vocans eam sub sera sua currit (bin/fabrica
 * purgare). */
#ifndef THESAURUS_H
#define THESAURUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"

nomen structura Thesaurus Thesaurus;

/* radix: directorium (creatur si absens). NIHIL si creari nequit. */
Thesaurus*
thesaurus_aperire (
    constans character* radix,
               Piscina* piscina);

/* VERIFICATIO lectionum (A2): ratio N = lectio una in N verificatur
 * (sigillum contentorum recomputatum); I = omnes (sub -plenus);
 * ZEPHYRUM = nulla. Ordinarium THESAURUS_VERIFICATIO_RATIO. */
#define THESAURUS_VERIFICATIO_RATIO XVI

vacuum
thesaurus_verificationem_ponere (
    Thesaurus* thesaurus,
          i32  ratio);

/* blob: octeti -> sigillum (SHA-256 contentorum). Scriptura atomica;
 * blob iam praesens non rescribitur. */
b32
thesaurus_ponere (
    Thesaurus* thesaurus,
       chorda  octeti,
     Sigillum* sigillum_out);

/* via plagulae blobi (legenda/nectenda); FALSUM si absens. Lectio
 * per specimen verificatur; verificatio fracta -> blob deletur,
 * FALSUM. */
b32
thesaurus_via (
             Thesaurus* thesaurus,
    constans  Sigillum* sigillum,
               Piscina* piscina,
                chorda* via_out);

/* actio: clavis -> sigilla exituum (plura, ordine). Xar elementorum
 * Sigillum. */
b32
thesaurus_actio_ponere (
             Thesaurus* thesaurus,
    constans  Sigillum* clavis,
    constans       Xar* sigilla_exituum);

/* FALSUM si clavis absens aut actio fracta (linea non LXIV hex). */
b32
thesaurus_actio_capere (
             Thesaurus*  thesaurus,
    constans  Sigillum*  clavis,
               Piscina*  piscina,
                   Xar** sigilla_out);

/* generatio: claves hoc cursu adhibitae (ad purgationem). GENERATIO
 * = cursus ordinans (bin/fabrica, porta), non processus: nomen ex
 * THESAURUS_GENERATIO (a tempore UTC incipiens; [A-Za-z0-9_-]). Absens:
 * nihil notatur (claves non servantur). */
vacuum
thesaurus_generationem_notare (
             Thesaurus* thesaurus,
    constans  Sigillum* clavis);

/* PURGARE: generationes ultimas servandas servat (indices et claves
 * eorum et blobos actionum eorum); cetera delet. verificare: blobi
 * servati omnes verificantur, fracti delentur. deleta_out = plagulae
 * deletae (blobi + actiones + indices). */
b32
thesaurus_purgare (
    Thesaurus* thesaurus,
          i32  generationes_servandae,
          b32  verificare,
          i32* deleta_out);

#endif /* THESAURUS_H */
