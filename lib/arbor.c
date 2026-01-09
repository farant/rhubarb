#include "arbor.h"
#include "filum.h"
#include "arbor_lexema.h"
#include "xar.h"
#include <stdio.h>

/* DEBUG flag - set to 1 to enable debug output */
#define ARBOR_DEBUG 0

/* ==================================================
 * Default Options
 * ================================================== */

interior constans character* _include_via_default = "include";

ArborOptiones
arbor_optiones_default(vacuum)
{
    ArborOptiones opt;

    opt.pp_modus        = ARBOR_PP_MODUS_HYBRID;
    opt.include_viae    = &_include_via_default;
    opt.include_numerus = I;

    redde opt;
}

/* ==================================================
 * Parsere Fontem
 * ================================================== */

ArborResultus
arbor_parsere_fontem(
    constans character*   fons,
    i32                   longitudo,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborOptiones*        optiones)
{
    ArborResultus res;
    ArborOptiones opt_default;
    ArborLexator* lexator;
    ArborPraeparator* pp;
    ArborSyntaxis* syn;
    Xar* lexemata;
    Xar* processata;
    ArborSyntaxisResultus parse_res;
    i32 i;

    /* Initializare resultum vacuum */
    res.successus = FALSUM;
    res.radix     = NIHIL;
    res.errores   = NIHIL;
    res.lexemata  = NIHIL;

    /* Verificare argumenta */
    si (fons == NIHIL || piscina == NIHIL || intern == NIHIL)
    {
        redde res;
    }

    /* Uti optiones default si NIHIL */
    si (optiones == NIHIL)
    {
        opt_default = arbor_optiones_default();
        optiones = &opt_default;
    }

    /* Lexere */
#if ARBOR_DEBUG
    printf("  [arbor] lexator_creare...\n"); fflush(stdout);
#endif
    lexator = arbor_lexator_creare(piscina, intern, fons, longitudo);
    si (lexator == NIHIL)
    {
        redde res;
    }

#if ARBOR_DEBUG
    printf("  [arbor] lexema_omnia...\n"); fflush(stdout);
#endif
    lexemata = arbor_lexema_omnia(lexator);
    si (lexemata == NIHIL)
    {
        redde res;
    }
#if ARBOR_DEBUG
    printf("  [arbor] lexemata count: %d\n", xar_numerus(lexemata)); fflush(stdout);
#endif

    /* Praeparare */
#if ARBOR_DEBUG
    printf("  [arbor] praeparator_creare...\n"); fflush(stdout);
#endif
    pp = arbor_praeparator_creare(piscina, intern);
    si (pp == NIHIL)
    {
        redde res;
    }

#if ARBOR_DEBUG
    printf("  [arbor] ponere_modum: %d\n", optiones->pp_modus); fflush(stdout);
#endif
    arbor_praeparator_ponere_modum(pp, optiones->pp_modus);

    per (i = ZEPHYRUM; i < optiones->include_numerus; i++)
    {
        arbor_praeparator_addere_via(pp, optiones->include_viae[i]);
    }

#if ARBOR_DEBUG
    printf("  [arbor] processare_lexemata...\n"); fflush(stdout);
#endif
    processata = arbor_praeparator_processare_lexemata(pp, lexemata, NIHIL);
#if ARBOR_DEBUG
    printf("  [arbor] processare_lexemata returned\n"); fflush(stdout);
#endif
    si (processata == NIHIL)
    {
        redde res;
    }
#if ARBOR_DEBUG
    printf("  [arbor] processata count: %d\n", xar_numerus(processata)); fflush(stdout);
#endif

    /* Parsere */
#if ARBOR_DEBUG
    printf("  [arbor] syntaxis_creare...\n"); fflush(stdout);
#endif
    syn = arbor_syntaxis_creare(piscina, intern);
    si (syn == NIHIL)
    {
        redde res;
    }

    arbor_syntaxis_ponere_keyword_macros(
        syn,
        arbor_praeparator_obtinere_keyword_macros(pp));

    arbor_syntaxis_ponere_typedef_nomina(
        syn,
        arbor_praeparator_obtinere_typedef_nomina(pp));

    parse_res = arbor_syntaxis_parsere(syn, processata);

    /* Transferre resulta */
    res.successus = parse_res.successus;
    res.radix     = parse_res.radix;
    res.errores   = parse_res.errores;
    res.lexemata  = processata;

    redde res;
}

/* ==================================================
 * Parsere Filum
 * ================================================== */

ArborResultus
arbor_parsere_filum(
    constans character*   via,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborOptiones*        optiones)
{
    ArborResultus res;
    chorda contentum;

    /* Initializare resultum vacuum */
    res.successus = FALSUM;
    res.radix     = NIHIL;
    res.errores   = NIHIL;
    res.lexemata  = NIHIL;

    /* Verificare argumenta */
    si (via == NIHIL || piscina == NIHIL || intern == NIHIL)
    {
        redde res;
    }

    /* Legere filum */
    contentum = filum_legere_totum(via, piscina);
    si (contentum.mensura == ZEPHYRUM)
    {
        redde res;
    }

    /* Delegare ad arbor_parsere_fontem */
    redde arbor_parsere_fontem(
        (constans character*)contentum.datum,
        contentum.mensura,
        piscina,
        intern,
        optiones);
}
