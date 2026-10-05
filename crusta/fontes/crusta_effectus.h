/* crusta_effectus.h - summarium effectuum scripti (effectus-spec;
 * planum effectus-plan.md T3)
 *
 * Quae scriptum - et omnia quae fontat aut exsequitur - legit,
 * scribit, exsequitur, fontat, enumerat, probat, et quas variabiles
 * ambitus legit, via cuiusque aestimata. Emittitur ut arbor STML
 * dialecti 'effectus' (effectus.canon): vocabularium LINGUA NEUTRA,
 * ut silva (C) idem olim emittat.
 *
 * AESTIMATOR idem ac crusta_fontationes (idiomata domus: cd && pwd,
 * dirname, basename, readlink -f; $0, BASH_SOURCE; 'local' in
 * functione; definitiones discordes = irresolutum). SITUS (spec par.
 * IV.2): redirectiones; source/.; '[' 'test' '[[ ]]'; globi in
 * argumentis et 'for'; tituli imperii per TABULAM MANDATORUM
 * (crusta/effectus_mandata.stml) - functio ambitus nihil facit (corpus
 * eius ambulatur), aedificium nihil nisi eval (ignotum), externum
 * extra tabulam = situs ignotum (causa 'mandatum ignotum'); variabiles
 * lectae quas ambitus non assignat = ambitus_lectio.
 *
 * SINE ORDINE (interview Q4): facta per processum; scripta_in_ambitu
 * = 'etiam scribitur', non 'ante scripta'. Lintrum quietat, clavem
 * numquam minuit (regula soliditatis, spec par. I).
 *
 * cwd situs: 'cd W' ultimum in catena eadem; aliter 'cd W' ultimum
 * in summo gradu plagulae ANTE situm (ordine fontis, non fluxus);
 * aliter radix. cd ignotum = resolutio nulla, causa 'cwd ignotum'.
 *
 * SEDES et OCTETI = materia_tractus_nodi nodi qui viam NOMINAT
 * (verbum, redirectio, pars variabilis; imperium pro ignoto). */

#ifndef CRUSTA_EFFECTUS_H
#define CRUSTA_EFFECTUS_H

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "stml.h"
#include "chorda.h"
#include "xar.h"

/* Summarium derivare. radix: via ABSOLUTA arboris (sine '/' finali);
 * scriptum: arbori relativa aut absoluta; mandata: radix tabulae IAM
 * LECTAE (elementum 'mandata'), NIHIL = legitur ex
 * radix/crusta/effectus_mandata.stml. Reddit elementum 'effectus';
 * NIHIL = memoria deficit, scriptum absens aut tabula illegibilis
 * (causa_out). TOTALIS: plagula fontata illegibilis aut parsura non
 * sana = situs ignotum cum causa, numquam NIHIL. */
StmlNodus*
crusta_effectus_derivare (
                Piscina*  piscina,
    InternamentumChorda*  intern,
     constans character*  radix,
     constans character*  scriptum,
              StmlNodus*  mandata,
     constans character** causa_out);

/* ORACULUM (planum T5): liber interpositionis (interpositio_macos.c)
 * -> summarium observatum eiusdem dialecti (per="observatum").
 * Processus soli bash; viae extra radicem et quaesitiones PATH
 * processus SUI omittuntur; argv mandatorum quos bash exsequitur per
 * tabulam eandem interpretatur (SIP: lectio intra /bin/cat aliter
 * invisibilis). ante_scripta (NIHIL licet): Xar de character* - viae
 * lectae ANTE scripturam in eodem processu (ordo quem analysis
 * statica non habet; semen slice 2). */
StmlNodus*
crusta_effectus_observata (
                Piscina*  piscina,
    InternamentumChorda*  intern,
     constans character*  radix,
     constans character*  scriptum,
                 chorda   liber,
              StmlNodus*  mandata,
                    Xar*  ante_scripta,
     constans character** causa_out);

/* Situs observati quos summarium staticum NON tegit (Xar de
 * StmlNodus*; vacuum = omnia tecta; NIHIL = argumentum absens).
 * Tegit: genus compatibile (lectio <- lectio/fontatio/exsecutio,
 * exsecutio <- exsecutio/fontatio, probatio <- quodvis) et via
 * aequalis,
 * globo congruens, aut praefixo contenta; radices processuum
 * statici (scripta ipsa) lectiones et probationes tegunt.
 * explicata (NIHIL licet): observata quae situs staticus IRRESOLUTUS
 * generis et mandati eiusdem explicat - clavis ibi iam IGNOTUM est,
 * ergo non errores analysis sed ignota nominata (classes T5: viae ex
 * ambitu '${X:-d}', ex argumentis functionum). */
Xar*
crusta_effectus_non_tecta (
       Piscina* piscina,
     StmlNodus* staticum,
     StmlNodus* observatum,
           Xar* explicata);

#endif /* CRUSTA_EFFECTUS_H */
