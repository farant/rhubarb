/* silva_arbor.h - vocabularium dialecti 'arbor' (STML canonicum)
 *
 * Arbor proicit arbores parsurae silvae in STML canonicum et eas
 * relegit. HOC caput stratum VOCABULARII solum fert (T2): sigillum
 * registri, quaesitiones nominum, tabulam orthographiae, mangulationem
 * tagorum. Scriptor (T3), comparator (T4), lector (T5) sequuntur.
 *
 * Consilium: project-specs/arbor-stml-spec-v2.md + arbor-stml-plan.md.
 *
 * CHARTA VOCABULARII (spec §2, contra registrum coctum mensurata):
 *   - genus nodi   -> tag elementi VERBATIM ex SilvaTabGenus.titulus
 *   - locus        -> tag elementi VERBATIM ex SilvaTabLocus.titulus
 *                     (nomina locorum NON globaliter unica sunt: LXII
 *                      nomina per CLXXVI ordines - locus significat
 *                      solum relative ad genus parentis)
 *   - genus lexematis -> tag PRAEFIXATUM 'lex-'
 *
 * CUR PRAEFIXUM: spatia nominum NON disiuncta sunt. 'assignatio' est
 * et genus nodi et SILVA_LEX_ASSIGNATIO; 'corpus' est et genus et
 * nomen loci. Praeterea genera lexematum in vocabulario communi
 * NUMQUAM fuerunt - machina quaestionis genera lexematum attingere
 * nequit (valorem TEXTUS comparat, non genus), et NOMINA_GENERUM
 * tabula separata est numquam in SilvaRegistrumCoctum nexa. Praefixum
 * ergo limitem spatii nominum DICIT, non celat.
 */

#ifndef SILVA_ARBOR_H
#define SILVA_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_tabulae.h"

/* Praefixum tagorum lexematum (vide caput pro ratione) */
#define SILVA_ARBOR_PRAEFIXUM "lex-"

/* Capacitas buffer cuivis tago lexematis sufficiens: praefixum (IV)
 * + nomen longissimum ('SINISTRORSUM_ASSIGNATIO', XXIII) + terminator.
 * Rotundatum ad LXIV - probatio limitem verum custodit. */
#define SILVA_ARBOR_TAG_CAPACITAS 64

/* Longitudo sigilli in characteribus (nibbles hexadecimales) */
#define SILVA_ARBOR_SIGILLI_LONGITUDO 8


/* ==================================================
 * Sigillum registri
 * ================================================== */

/* Sigillum registri grammatici: friatio FNV-1a super genera + locos,
 * reddita ut chorda hexadecimalis minuscula VIII characterum
 * (semper VIII - zephyris ante-implita, ut sigilla oculo conferri
 * possint).
 *
 * TOTALE, non exemplar: SilvaRegistrumCoctum quattuor campos exacte
 * fert (duae series + duo numeri), ergo genera[] et loci[] ambulata
 * TOTAM structuram tegunt - nullus est status occultus quem sigillum
 * praeterire possit. Nullus campus versionis usquam exstat: CONTENTUM
 * tabularum solum signum versionis est, quod est prorsus quod
 * friandum volumus.
 *
 * Munus: documentum arboris sigillum fert; lector CONFERT et REICIT
 * si divergit. Arbor iudicata vocabulario falso mendacium est.
 *
 * XXXII bita sufficiunt quia comparatio UNA fit (sigillum documenti
 * contra sigillum registri currentis), ergo acceptio falsa 2^-32
 * constat - non terminus natalicius, qui comparationi binis inter
 * copiam applicaretur.
 *
 * Piscina/tabularium NIHIL -> chorda vacua (mensura ZEPHYRUM). */
chorda
silva_arbor_sigillum (
                           Piscina* piscina,
     constans SilvaRegistrumCoctum* tabularium);


/* ==================================================
 * Quaesitiones nominum registri
 * ================================================== */

/* Nomen generis nodi -> index in tabularium->genera; -I si ignotum.
 * Titulus NON terminatur nullo (mensura data) - chordae documenti
 * ita veniunt. */
s32
silva_arbor_genus_index (
     constans SilvaRegistrumCoctum* tabularium,
                constans character* titulus,
                               i32  mensura);

/* Nomen loci INTRA genus datum -> index ABSOLUTUS in seriem planam
 * tabularium->loci; -I si ignotum aut genus_index extra fines.
 *
 * Scopus generis essentialis est: nomina locorum non globaliter
 * unica sunt (vide caput). Index absolutus redditur ut vocans
 * speciem statim legere possit: tabularium->loci[index].species. */
s32
silva_arbor_locus_index (
     constans SilvaRegistrumCoctum* tabularium,
                               s32  genus_index,
                constans character* titulus,
                               i32  mensura);


/* ==================================================
 * Orthographia lexematum
 * ================================================== */

/* Orthographia FIXA generis lexematis ('auto', '[', '->', ...);
 * NIHIL si genus orthographiam VARIAM fert (identificator, litterae,
 * trivia, genera robustitatis) aut si genus extra fines est.
 *
 * ARTIFICIUM NOVUM, PORTA OBLIGATA. Silva tabulam INVERSAM
 * (genus -> littera) nusquam habet: verba clausa tabulam ANTRORSUM
 * solam ferunt (VERBA_CLAUSA, silva_lexema.c) et interpunctiones
 * cascadam si-alioquin recognoscentem solam (_legere_interpunctionem).
 * Haec ergo fons veritatis SECUNDUS est, qui a lexatore divergere
 * potest. Porta ad nativitatem: probatio orthographiam CUIUSQUE
 * introitus lexat et genus redditum exspectat.
 *
 * Silva trigraphos NON implet, ergo orthographiae hodie vere I:I
 * sunt. Si trigraphi umquam addantur tabula I:I esse desinit et
 * valor portandus fit - dependentia nominata. */
constans character*
silva_arbor_orthographia (
    SilvaLexemaGenus genus);

/* An valor lexematis in documento PORTANDUS sit (orthographia varia).
 * FALSUM pro generibus orthographiae fixae ET pro EOF, cuius valor
 * semper vacuus est. */
b32
silva_arbor_valor_portandus (
    SilvaLexemaGenus genus);


/* ==================================================
 * Mangulatio tagorum lexematum
 * ================================================== */

/* Tag lexematis in buffer datum scribere: praefixum 'lex-' + nomen
 * generis minusculum, '_' in '-' mutatis. Nullo terminatur.
 * Reddit longitudinem SCRIPTAM (sine terminatore), aut ZEPHYRUM si
 * genus extra fines est aut capacitas non sufficit.
 *
 * Nomen ex silva_lexema_genus_nomen sumitur - NON tabula propria.
 * Tabula propria quartus fons veritatis esset. */
i32
silva_arbor_lexema_tag (
    SilvaLexemaGenus  genus,
           character* buffer,
                 i32  capacitas);

/* Tag ('lex-...') -> genus lexematis; SILVA_LEX_NUMERUS_GENERUM si
 * ignotum. Titulus NON terminatur nullo (mensura data).
 *
 * Implementum per mangulationem ANTRORSAM cuiusque generis et
 * comparationem - ergo directiones duae DIVERGERE NON POSSUNT. */
SilvaLexemaGenus
silva_arbor_lexema_ex_tag (
     constans character* tag,
                    i32  mensura);

#endif /* SILVA_ARBOR_H */
