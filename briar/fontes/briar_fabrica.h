/* briar_fabrica.h - Arbor thistle -> proiectum silicis (fontes ad
 * extensiones unitatum silvae secta, '#line' in .thistle, caput
 * prototyporum genitum, unitas probationis separata, clausura, ordines)
 * SINE clang. Clavis = SHA-256(stampa corporis, vexilla, octeti).
 *
 * Regula principalis (spec par. 4.3): unitas cuius declarator post
 * expansionem 'main' est = programma planum; nulla + <fenestra/> =
 * app atrii (principale genitum); duae = recusatio; neutra =
 * recusatio. Regio munus="probatio" unitas propria, extra regulam.
 *
 * Partitio (plan 2): caput <t>_regiones.h = latina.h + directivae
 * omnium regionum C (#include, #define...) + unitates typorum (typedef,
 * tags, prototypi) + prototypi functionum definitarum (textus capitis
 * definitionis + ';'); <t>_regiones.c = obiecta (variabiles scopi
 * plagulae) + definitiones praeter principale. Commenta INTER unitates
 * cadunt (trivia extensionibus aliena); commenta intra unitates manent.
 */

#ifndef BRIAR_FABRICA_H
#define BRIAR_FABRICA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_nodus.h"
#include "silex.h"
#include "briar_nexus.h"

nomen enumeratio {
    BRIAR_FORMA_NULLA = 0,
    BRIAR_FORMA_PLANA,      /* principale in regione C */
    BRIAR_FORMA_VITREA      /* <fenestra/> + methodi */
} BriarForma;

/* plagula genita: via proiecto relativa + contentum */
nomen structura {
    chorda via;
    chorda contentum;
} BriarPlagula;

nomen structura {
                    b32  successus;
                 chorda  causa;           /* recusatio; cum linea */
                    i32  linea_causae;
             BriarForma  forma;
     constans character* titulus;         /* basis viae, sine suffixo */
              character  sigillum[17];    /* clavis: XVI hex + NUL */
                    Xar* genitae;         /* BriarPlagula genitae */
                    Xar* clausura;        /* SilexRes: bibliothecae */
                    i32  regiones_c;      /* regiones C non-probatio */
                    b32  probatio_adest;
} BriarFabricaFructus;

nomen structura {
    constans character* via_thistle;   /* in '#line' ut data */
    constans character* stampa;        /* corporis, in clavem */
    constans character* fons_titulus;  /* stampa ordinum vitreorum */
} BriarFabricaOptiones;

/* nexus post briar_silvam_texere; octeti = plagula tota (clavis).
 * Recusatio: successus FALSUM, causa + linea_causae; nihil genitum. */
BriarFabricaFructus
briar_fabricare (
                          Piscina* piscina,
            constans MateriaNodus* documentum,
                              Xar* nexus,
               constans SilexFons* fons,
    constans BriarFabricaOptiones* optiones,
                           chorda  octeti);

/* proiectum in directorium scribere (parentes creat); ordines
 * exsecutabiles (chmod 0755); FALSUM + causa si plagula non scripta */
b32
briar_fabricam_scribere (
                         Piscina* piscina,
    constans BriarFabricaFructus* fructus,
              constans character* radix,
                          chorda* causa);

/* directoria viae omnia creare ('a/b/c': a, a/b, a/b/c); VERUM si
 * omnia exsistunt post (filum_directorium_creare_si_necesse gradum
 * UNUM solum creat) */
b32
briar_directoria_creare (
               Piscina* piscina,
    constans character* via);

/* 'a/b/salve.thistle' -> 'salve'; characteres extra [A-Za-z0-9_] -> '_'
 * (symbolum C fit: capsula_<t>, custos capitis) */
constans character*
briar_fabrica_titulus (
               Piscina* piscina,
    constans character* via);

/* vexilla formae, chorda literalis EXACTA quam ordines ferunt */
constans character*
briar_fabrica_vexilla (
    BriarForma forma);

/* clavis: SHA-256(stampa '\n' vexilla '\n' octeti) -> XVI hex
 * minusculae */
vacuum
briar_fabrica_clavem_computare (
    constans character* stampa,
    constans character* vexilla,
                chorda  octeti,
             character* sigillum_xvii);

#endif /* BRIAR_FABRICA_H */
