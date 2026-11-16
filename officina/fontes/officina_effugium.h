/* officina_effugium.h - introitus in codicem nativum et effugium ex eo
 * (M5 T6; spec C7, D10, D12)
 *
 * Machinula functionem NATIVAM per trampolinum universale vocat:
 * argumenta AAPCS64 in registra et stivam ponuntur, functio vocatur,
 * x0:x1 et d0-d3 redduntur. Codex nativus qui pergere nequit
 * (divisio per zephyrum, fines) officina_effugium_salire vocat:
 * vocatio pendens statim cum EFFUGIUM_SALTUS redit, stiva nativa
 * relicta. Signum hardware in codice nativo (lectio per monstratorem
 * nullum, instructio illicita) idem facit cum EFFUGIUM_VITIUM;
 * processus supervivit. Area codicis una, ad inscriptionem fixam
 * (codex inter se per 'bl' vocat).
 *
 * Status processus globalis: parare semel, piscina propria permanens.
 */

#ifndef OFFICINA_EFFUGIUM_H
#define OFFICINA_EFFUGIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "memoria_exsecutabilis.h"

#define OFFICINA_NATIVUM_LOCUS   0x2FFFF0000000UL  /* F19 */
#define OFFICINA_NATIVUM_MENSURA 0x10000000UL

/* trampolinum [0, OFFICINA_EFFUGIUM_TRAMPOLINUM) areae occupat;
 * codex vocantium post id scribitur */
#define OFFICINA_EFFUGIUM_TRAMPOLINUM 0x1000UL

nomen enumeratio {
    EFFUGIUM_REDDITUM = 0,  /* functio rediit */
    EFFUGIUM_SALTUS,        /* officina_effugium_salire vocata */
    EFFUGIUM_VITIUM         /* signum hardware in codice nativo */
} EffugiumGenus;

nomen structura {
               s32 genus;
               s32 signum_systematis;  /* SIGSEGV... (VITIUM) */
               s64 codex;              /* SALTUS: causa */
    memoriae_index pc;                 /* VITIUM/SALTUS: positio */
    memoriae_index fp;
    memoriae_index lr;                 /* VITIUM: x30 (vocans, si
                                        * vitium ante tabulatum) */
    memoriae_index inscriptio_vitii;   /* VITIUM: si_addr */
               b32 stiva_exhausta;     /* VITIUM: inscriptio vitii
                                        * iuxta sp - stiva hospitis
                                        * exhausta */
               i64 x[II];              /* x0:x1 reditus */
               f64 d[IV];              /* d0-d3 reditus (HFA) */
} EffugiumFructus;

nomen structura {                      /* argumenta in AAPCS64 */
    i64  x[VIII];
    s32  numerus_x;
    f64  d[VIII];
    s32  numerus_d;
    i64* stiva;                        /* verba, iam ordinata */
    s32  numerus_stivae;
    i64  x8;                           /* sret > XVI octeti */
} EffugiumArgumenta;

/* aream reservat, trampolinum scribit et tractatorem signorum
 * (SIGSEGV, SIGBUS, SIGILL; stiva alterna) installat, semel (vocatio
 * altera nihil agit, VERUM); FALSUM + causa si quid dari nequit.
 * Signum extra aream aut sine vocatione pendente ad actionem priorem
 * it (catena). */
b32
officina_effugium_parare (
    Piscina* piscina,
     chorda* causa_out);

/* area codicis; NIHIL ante parare */
MemoriaExsecutabilis*
officina_effugium_area (vacuum);

/* quoties tractator signorum installatus est (I post parare, quoties
 * cumque vocatum) */
s32
officina_effugium_installationes (vacuum);

/* functionem ad introitum vocat; genus REDDITUM, SALTUS aut VITIUM */
EffugiumFructus
officina_effugium_vocare (
                        vacuum* introitus,
    constans EffugiumArgumenta* argumenta);

/* a codice nativo vocatur; numquam redit. Extra vocationem pendentem:
 * abortus processus (nullus locus ad quem redeat) */
vacuum
officina_effugium_salire (
               s64 codex,
    memoriae_index pc,
    memoriae_index fp);

#endif /* OFFICINA_EFFUGIUM_H */
