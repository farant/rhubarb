/* crusta_effectus.h - summarium effectuum scripti (effectus-spec;
 * planum effectus-plan.md T3)
 *
 * Quae scriptum - et omnia quae fontat aut exsequitur - legit,
 * scribit, exsequitur, fontat, enumerat, probat, et quas variabiles
 * ambitus legit, via cuiusque aestimata. Emittitur ut arbor STML
 * dialecti 'effectus' (effectus.canon): vocabularium LINGUA NEUTRA,
 * ut silva (C) idem olim emittat.
 *
 * AESTIMATOR olim in crusta_fontationes (retiratae T8). Idiomata
 * domus: cd && pwd,
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

/* Idem cum ARGV radicis declarata (effectus-plan-3 T5, A1): argumenta =
 * Xar de character* (verba post scriptum; vacua = nulla), NIHIL =
 * ignota ($N radicis 'argumentum' manet). Processus filii $N per arcus
 * (verba sedis exsecutionis) semper accipiunt. */
StmlNodus*
crusta_effectus_derivare_argumentis (
                Piscina*  piscina,
    InternamentumChorda*  intern,
     constans character*  radix,
     constans character*  scriptum,
              StmlNodus*  mandata,
                    Xar*  argumenta,
     constans character** causa_out);

/* ARGV RADICIS DECLARATA (effectus-plan-3 T5, A1): verba <argumenta>
 * ingressus 'effectus' scripti in aedificatio.stml subsystematum
 * (Xar de character*; vacua = nulla). NIHIL = non declarata, aut
 * declarationes discordes / ingressus sine <argumenta>. */
Xar*
crusta_effectus_argumenta_radicis (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* radix,
     constans character* scriptum);

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

/* SUBSUMPTIO (effectus-plan-3 T1; spec-3 par. VIII): situs NOVI quos
 * summarium VETUS non subsumit (Xar de StmlNodus*; vacuum = omnia
 * subsumpta; NIHIL = argumentum absens). Situs veteris eiusdem
 * plagulae, sedis et elementi novum tegit: via aequalis, globus
 * congruens aut continens, praefixum continens, aut vetus irresolutus
 * (resolutio nulla) - ignotum ab ignoto solo. Situs novus minus certus
 * quam vetus (praefixum ubi via erat) aut sedes nova = defectus:
 * probatio 'valor novus subcopia veteris' slice 3. */
Xar*
crusta_effectus_subsumptio (
       Piscina* piscina,
     StmlNodus* vetus,
     StmlNodus* novum);

/* CATENAE VERDICTI (planum T6; spec A4 - ex declarationibus fabricae
 * derivatae): viae scriptorum quae ingressus 'effectus' alicuius
 * actionis sunt, per fabrica.stml et
 * aedificatio.stml subsystematum. Xar de character*; vacuum = arbor
 * sine fabrica aut sine catenis; NIHIL = fabrica.stml fracta. */
Xar*
crusta_effectus_catenae (
                Piscina*  piscina,
    InternamentumChorda*  intern,
     constans character*  radix,
     constans character** causa_out);

/* Summarium plagulam 'via' (arbori relativam) tenet - ut radix
 * processus aut ut plagula situs alicuius? Processus custoditi
 * (aedificatores '[ -x P ] || S', attributum custodia) praetereuntur:
 * extra catenam verdicti sunt (P provenientia clavem tenet). */
b32
crusta_effectus_plagulam_tenet (
             StmlNodus* summarium,
    constans character* via);

#endif /* CRUSTA_EFFECTUS_H */
