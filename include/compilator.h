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
 * ea imprimit aut servat) */
CompilatorResultus
compilator_compilare (
                      Compilator* compilator,
    constans character* constans* argumenta,
                              i32 numerus);

#endif /* COMPILATOR_H */
