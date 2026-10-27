/* compilator.h - 'clang -c' per thesaurum contentorum, ut bibliotheca
 * (fabrica-6 T4; olim tools/compilator.c solum; caput Franus probavit).
 *
 * CLAVIS CAPITIS = sigillum(cwd ‖ identitas clang ‖
 * argumenta sine valore -o ‖ via et octeti fontis ‖ viae
 * radicum inclusionis). Sub ea: index capitum depfile cursus
 * ultimi. CLAVIS PLENA = caput ‖ per caput: via ‖ octeti ‖
 * obumbratio per nomen (fabrica plan 5 T2). Sub ea: obiectum.
 * Hit: obiectum copiatum (temporarium + rename; destinatio
 * identica non tangitur); miss: clang -MD -MF, conditum in
 * thesauro.
 */

#ifndef COMPILATOR_H
#define COMPILATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "sigillum.h"

nomen structura Compilator Compilator;

nomen structura {
       s32 codex;          /* exitus clang (0 bene); -I: non
                             * cacheabile aut clang currere nequit
                             * (erratum dicit) */
       b32 ex_thesauro;    /* hit */
    chorda effusio;        /* stdout clang (vacua in hit) */
    chorda erratum;        /* stderr clang (vacua in hit) */
    chorda via;            /* obiectum nectendum (codex 0): exitus '-o'
                            * (collocatum aut compilatum), aut - sine
                            * collocatione, in hit - blobus thesauri
                            * ipse; vacua aliter */
} CompilatorResultus;

/* thesaurum aperire; clang NIHIL = FABRICA_CLANG aut "clang".
 * NIHIL si thesaurus aperiri nequit. */
Compilator*
compilator_aperire (
               Piscina* piscina,
    constans character* radix_thesauri,
    constans character* clang);

/* argumenta cacheabilia? '-c', '-o OBJ', fons unus .c/.m; non
 * -E/-S/-M* (illa clang ipse currat) */
b32
compilator_cacheabile (
    constans character* constans* argumenta,
                              i32 numerus);

/* clavis capitis SINE compilatione (gradus 'nectere', fabrica-6 T6).
 * FALSUM: non cacheabile aut identitas clang ignota. */
b32
compilator_clavem (
                      Compilator* compilator,
    constans character* constans* argumenta,
                              i32 numerus,
                        Sigillum* clavis_out);

/* compilare per thesaurum; effusio et erratum clang in resultu (vocans
 * ea imprimit aut servat). Allocationes in piscina compilatoris: per
 * vocationem crescit (~I MB ex thesauro: fons, index, obiecta lecta) */
CompilatorResultus
compilator_compilare (
                      Compilator* compilator,
    constans character* constans* argumenta,
                              i32 numerus);

/* idem, sed allocationes VOCATIONIS (plagulae lectae, argumenta,
 * effusio, erratum) in 'piscina' vocantis - resultus in ea vivit; eam
 * post vocationem vacare licet. Thesaurus et memoria per processum in
 * piscinis suis manent (parcum …T77EH4: fabrica membrum quodque in
 * piscina temporaria agit) */
CompilatorResultus
compilator_compilare_in (
                      Compilator* compilator,
    constans character* constans* argumenta,
                              i32 numerus,
                         Piscina* piscina);

/* idem ac compilator_compilare_in, sed in hit obiectum NON collocatur
 * (nec legitur nec in exitum '-o' scribitur): resultus.via = blobus
 * thesauri, quem vocans directe nectit (clang plagulam sine suffixo
 * ut obiectum accipit). In miss compilatur in exitum ut olim (via =
 * exitus). Fabrica membra sic nectit - olim obiecta cuiusque membri in
 * area eius copiabantur (XII milia plagularum radicis, I.V GB; parcum
 * nexus ex thesauro). */
CompilatorResultus
compilator_compilare_sine_collocatione (
                      Compilator* compilator,
    constans character* constans* argumenta,
                              i32 numerus,
                         Piscina* piscina);

/* MEMORIA PER PROCESSUM (2026-10-23): capita indicis sigillata et
 * existentiae obumbrationis memorantur (via -> sigillum / praesens):
 * vocatio quaeque post primam capita communia nec legit nec quaerit.
 * Solum ubi capita intra processum non mutantur ET liber lectionum
 * (FABRICA_LECTIONES) per vocationem non postulatur - lectio memorata
 * liber non notat. Vocat 'bin/compilator --plures'; fabrica (membra
 * in processu uno, liber per membrum) NON. */
vacuum
compilator_memoriam_incipere (
    Compilator* compilator);

#endif /* COMPILATOR_H */
