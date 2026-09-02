/* silva_lexicon.h - compositio lexici systematis per-plagulam
 *
 * MODULUS COMMUNIS instrumentorum (exemplar silva_amalgama): quod
 * superficies iudicii lexicon plagulae dare debet, hic UNO loco
 * computatur. Consumptores: examen (primus); percursus et legatus
 * (per praeparatorem) migrant tactu suo - "geminus tertius
 * oneratorum" ne quartum pareret.
 *
 * Duae functiones, duo fontes veritatis:
 *
 * I. DERIVATIO EX INCLUSIONIBUS (varians (a), parcum 01KYB3NNR4;
 *    decretum 01KYDMTS5N: POSIX solum, ISO semper integrum).
 *    Plagula dependentias suas IAM declarat - '#include
 *    <sys/select.h>' EST dictum "superficie select utor". Sectiones
 *    in systema_posix.h tagis notantur:
 *      <sectio caput="fcntl.h"/>            uni capiti
 *      <sectio caput="a.h,b.h"/>            pluribus
 *      <sectio semper="verum"/>             basis (typi communes)
 *      <sectio caput="x.h" poscit="y.h"/>   dependentia sectionum
 *    'poscit' necessarium est, non ornamentum: select() struct
 *    timeval ex sys/time.h poscit. Clausura transitiva computatur;
 *    sectiones ORDINE FONTIS emittuntur, ergo ordo declarationum
 *    per constructionem rectus.
 *
 * II. BLOCI externa (varians (b) eiusdem parci): .d.ts pro C.
 *    Superficies quas lexicon globalis NONDUM fert plagula ipsa in
 *    commentario declarat, tago 'externa caput="..."' aperto et
 *    tago clauso finito. EXEMPLAR VIVUM: lib/reactor.c (poll.h);
 *    lib/tcp_posix.c (quattuor bloci). Forma exacta hic CONSULTO
 *    non scribitur: exemplum plenum commentarium in commentario
 *    esset (-Wcomment sub -Werror in omni consumptore), et tagus
 *    apertus sine clauso excerptorem falleret si hoc caput umquam
 *    ipsum scrutabitur.
 *    Contentum C purum est - fons lexici, in systema_posix.h
 *    verbatim promovibilis. 'caput=' REQUIRITUR (clavis
 *    promotionis: nominat sectionem globalem pro qua blocus
 *    vicarius stat).
 *
 * ORDO PORTANS: utraque functio TEXTUM CRUDUM scrutatur, non
 * arborem - lexicon typos definit quibus parsatio ipsa eget, ergo
 * arbor quae hinc penderet nondum exsistit cum opus est.
 *
 * ASYMMETRIA PERICULI (cur declarationes locales licitae sint):
 * declaratio errans efficere potest SOLUM ut iudex accipiat quod
 * clang postea reiciat - monitio amissa, numquam mendum missum.
 * Exclusio contra omnia silet et plagulam CAECAM relinquit.
 */

#ifndef SILVA_LEXICON_H
#define SILVA_LEXICON_H

#include "latina.h"
#include "piscina.h"

/* Sectiones POSIX plagulae debitas excerpere: lineae '#include
 * <...>' plagulae (fons_pl) contra tagos <sectio> lexici (fons_px)
 * congruuntur, clausura 'poscit' completur, sectiones petitae
 * ordine fontis concatenantur. NIHIL = nulla sectio petita (plagula
 * sine capitibus POSIX - ISO purum). */
character*
silva_lexicon_posix_derivare (
    constans character* fons_px,
                   i32  mensura_px,
    constans character* fons_pl,
                   i32  mensura_pl,
               Piscina* piscina,
                   i32* mensura_out);

/* Blocos externa (tago aperto cum caput="...", tago clauso finito
 * - forma exacta CONSULTO non scripta, vide CAVE in .c: sequentia
 * aperiens in plagula scrutata tagum EVOCAT, et haec plagula ipsa
 * a percursu scrutatur) plagulae excerpere, praefixo ' * '
 * commentarii dempto, concatenatos reddere.
 *
 * 'fractum' ab 'absente' DISCERNITUR: NIHIL sine fracto = nullus
 * blocus (casus communis, tacitus); NIHIL cum fracto = annotatio
 * prava (sine caput=, sine clausura) quae iudicium SISTERE debet -
 * vocator verdictum mundum ex annotatione fracta reddere NON debet
 * (INFRA, non ACCIPE). Nuntius erroris iam in stderr scriptus est. */
character*
silva_lexicon_externa_excerpere (
    constans character* fons,
                   i32  mensura,
               Piscina* piscina,
                   i32* mensura_out,
    constans character* via,
                   b32* fractum);

/* Compositio PLENA textus systematis pro plagula una - functiones
 * I et II vocatae UNO loco (geminus tertius oneratorum hic moritur:
 * examen, percursus, praeparator omnes hanc vocant, compositionem
 * nemo iterum scribit):
 *
 *   ISO integrum
 *   + POSIX (totum sub 'totum_posix' - escape '-posix' vetus;
 *     alioquin sectiones ex inclusionibus plagulae derivatae)
 *   + bloci externa plagulae ipsius
 *
 * Reddit allocationem NOVAM semper, etiam ISO purum sine additis -
 * CONSULTO: aequalitas CONTENTI = clavis cache consumptorum
 * (plagulae sine POSIX ad eundem textum componunt ergo eandem
 * praeparationem communicant, sine casu speciali). Plagula NIHIL
 * licet = ISO purum.
 *
 * 'fractum' contractus ut supra (II): VERUM = annotatio prava,
 * vocator iudicium SISTERE debet (INFRA, numquam ACCIPE; nuntius
 * iam in stderr). NIHIL sine fracto = apparatus (allocatio), non
 * absentia. */
character*
silva_lexicon_componere (
    constans character* fons_iso,
                   i32  mensura_iso,
    constans character* fons_px,
                   i32  mensura_px,
    constans character* fons_pl,
                   i32  mensura_pl,
                   b32  totum_posix,
               Piscina* piscina,
                   i32* mensura_out,
    constans character* via,
                   b32* fractum);

#endif /* SILVA_LEXICON_H */
