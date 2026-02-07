/* generare.c - Cursor Directivarum Generationis
 *
 * Instrumentum quod fila .c scrutat pro directivis generationis
 * et instrumenta execuit ante compilationem.
 *
 * USUS:
 *   ./bin/generare [--verbose] <via> [<via> ...]
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "argumenta.h"

#include <stdio.h>
#include <stdlib.h>


/* ========================================================================
 * CONSTANTIAE
 * ======================================================================== */

hic_manens i8 _praefixum_buf[] = {
    '/', '*', ' ', 'g', 'e', 'n', 'e', 'r', 'a', 'r', 'e', ':', ' '
};
#define GENERARE_PRAEFIXUM_MENSURA  XIII

hic_manens i8 _suffixum_buf[] = { '*', '/' };
#define GENERARE_SUFFIXUM_MENSURA   II

hic_manens i8 _c_suffixum_buf[] = { '.', 'c' };
#define C_SUFFIXUM_MENSURA          II

hic_manens i8 _c_glob_buf[] = { '*', '.', 'c' };
#define C_GLOB_MENSURA              III


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Directiva extracta ex filo */
nomen structura {
    chorda  instrumentum;
    Xar*    argumenta;
    s32     versus;         /* Numerus versus (1-based) */
} GenerareDirectiva;

/* Contextus pro ambulatore directoriorum */
nomen structura {
    Xar*     fila;
    Piscina* piscina;
} ColligereContextus;


/* ========================================================================
 * COLLIGERE FILA
 * ======================================================================== */

interior s32
_colligere_filum(
    chorda                         via_plena,
    constans DirectoriumIntroitus* introitus,
    vacuum*                        contextus)
{
    ColligereContextus* ctx;
    chorda*             via_ptr;
    chorda              suffixum;

    ctx = (ColligereContextus*)contextus;

    si (introitus->genus != INTROITUS_FILUM)
    {
        redde ZEPHYRUM;
    }

    suffixum = chorda_ex_buffer(_c_suffixum_buf, C_SUFFIXUM_MENSURA);
    si (!chorda_terminatur(introitus->titulus, suffixum))
    {
        redde ZEPHYRUM;
    }

    via_ptr = (chorda*)xar_addere(ctx->fila);
    si (via_ptr != NIHIL)
    {
        *via_ptr = chorda_transcribere(via_plena, ctx->piscina);
    }

    redde ZEPHYRUM;
}


/* ========================================================================
 * COMPUTARE VERSUM
 * ======================================================================== */

/* Numerare lineas novas in chorda usque ad positionem datam */
interior s32
_computare_lineas(
    chorda contentum,
    i32    usque_ad)
{
    s32 numerus;
    i32 i;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < usque_ad && i < contentum.mensura; i++)
    {
        si (contentum.datum[i] == '\n')
        {
            numerus++;
        }
    }

    redde numerus;
}


/* ========================================================================
 * EXTRAHERE DIRECTIVAS
 * ======================================================================== */

interior Xar*
_extrahere_directivas(
    chorda   contentum,
    Piscina* piscina)
{
    Xar*   directivae;
    chorda praefixum;
    chorda suffixum;
    s32    positio;
    chorda reliquum;
    s32    versus_currens;

    directivae = xar_creare(piscina, (i32)magnitudo(GenerareDirectiva));
    praefixum  = chorda_ex_buffer(_praefixum_buf, GENERARE_PRAEFIXUM_MENSURA);
    suffixum   = chorda_ex_buffer(_suffixum_buf, GENERARE_SUFFIXUM_MENSURA);

    reliquum        = contentum;
    versus_currens  = I;

    dum (reliquum.mensura > ZEPHYRUM)
    {
        chorda             inventum;
        s32                finis_idx;
        chorda             corpus;
        chorda             praecisa;
        GenerareDirectiva* dir;
        i32                idx;
        i32                signi_initium;
        s32                lineas_ante;

        positio = chorda_invenire_index(reliquum, praefixum);
        si (positio < ZEPHYRUM)
        {
            frange;
        }

        /* Computare lineas novas inter positionem currentem et directivam */
        lineas_ante = _computare_lineas(reliquum, (i32)positio);
        versus_currens += lineas_ante;

        inventum = chorda_sectio(reliquum,
                                 (i32)positio + GENERARE_PRAEFIXUM_MENSURA,
                                 reliquum.mensura);

        finis_idx = chorda_invenire_index(inventum, suffixum);
        si (finis_idx < ZEPHYRUM)
        {
            frange;
        }

        corpus = chorda_sectio(inventum, ZEPHYRUM, (i32)finis_idx);
        praecisa = chorda_praecidere(corpus);

        si (praecisa.mensura > ZEPHYRUM)
        {
            dir = (GenerareDirectiva*)xar_addere(directivae);
            si (dir != NIHIL)
            {
                dir->argumenta = xar_creare(piscina, (i32)magnitudo(chorda));
                dir->instrumentum.datum = NIHIL;
                dir->instrumentum.mensura = ZEPHYRUM;
                dir->versus = versus_currens;

                idx = ZEPHYRUM;
                dum (idx < praecisa.mensura)
                {
                    dum (idx < praecisa.mensura &&
                         (praecisa.datum[idx] == ' ' ||
                          praecisa.datum[idx] == '\t' ||
                          praecisa.datum[idx] == '\n' ||
                          praecisa.datum[idx] == '\r'))
                    {
                        idx++;
                    }

                    si (idx >= praecisa.mensura)
                    {
                        frange;
                    }

                    signi_initium = idx;
                    dum (idx < praecisa.mensura &&
                         praecisa.datum[idx] != ' ' &&
                         praecisa.datum[idx] != '\t' &&
                         praecisa.datum[idx] != '\n' &&
                         praecisa.datum[idx] != '\r')
                    {
                        idx++;
                    }

                    {
                        chorda signum;
                        signum = chorda_sectio(praecisa, signi_initium, idx);

                        si (dir->instrumentum.datum == NIHIL)
                        {
                            dir->instrumentum = chorda_transcribere(signum, piscina);
                        }
                        alioquin
                        {
                            chorda* arg;
                            arg = (chorda*)xar_addere(dir->argumenta);
                            si (arg != NIHIL)
                            {
                                *arg = chorda_transcribere(signum, piscina);
                            }
                        }
                    }
                }
            }
        }

        /* Computare lineas in directiva ipsa (praefixum + corpus + suffixum) */
        {
            i32 finis_directivae;
            finis_directivae = (i32)positio + GENERARE_PRAEFIXUM_MENSURA +
                               (i32)finis_idx + GENERARE_SUFFIXUM_MENSURA;
            versus_currens += _computare_lineas(
                chorda_sectio(reliquum, (i32)positio, finis_directivae),
                finis_directivae - (i32)positio);
            reliquum = chorda_sectio(reliquum, finis_directivae, reliquum.mensura);
        }
    }

    redde directivae;
}


/* ========================================================================
 * EXECUTARE DIRECTIVAM
 * ======================================================================== */

interior s32
_executare_directivam(
    constans GenerareDirectiva* directiva,
    chorda                      filum_via,
    b32                         loquax,
    Piscina*                    piscina)
{
    ChordaAedificator* aed;
    chorda              mandatum_chorda;
    character*          mandatum_cstr;
    s32                 codex;
    i32                 idx;

    aed = chorda_aedificator_creare(piscina, DXII);

    /* Praeponi variabiles ambitus: GENERARE_FILUM et GENERARE_VERSUS */
    chorda_aedificator_appendere_literis(aed, "GENERARE_FILUM=");
    chorda_aedificator_appendere_chorda(aed, filum_via);
    chorda_aedificator_appendere_literis(aed, " GENERARE_VERSUS=");
    chorda_aedificator_appendere_s32(aed, directiva->versus);
    chorda_aedificator_appendere_character(aed, ' ');

    chorda_aedificator_appendere_literis(aed, "bin/");
    chorda_aedificator_appendere_chorda(aed, directiva->instrumentum);

    per (idx = ZEPHYRUM; idx < (i32)xar_numerus(directiva->argumenta); idx++)
    {
        chorda* arg;
        arg = (chorda*)xar_obtinere(directiva->argumenta, (i32)idx);
        si (arg != NIHIL)
        {
            chorda_aedificator_appendere_character(aed, ' ');
            chorda_aedificator_appendere_chorda(aed, *arg);
        }
    }

    mandatum_chorda = chorda_aedificator_finire(aed);
    mandatum_cstr = chorda_ut_cstr(mandatum_chorda, piscina);

    printf("  $ %s\n", mandatum_cstr);

    codex = system(mandatum_cstr);

    si (codex != ZEPHYRUM)
    {
        fprintf(stderr, "ERRATUM: mandatum fracta (codex=%d): %s\n",
                codex, mandatum_cstr);
    }
    alioquin si (loquax)
    {
        printf("    -> successus\n");
    }

    redde codex;
}


/* ========================================================================
 * PROCESSARE FILUM
 * ======================================================================== */

interior s32
_processare_filum(
    chorda   via,
    b32      loquax,
    Piscina* piscina)
{
    character*  via_cstr;
    chorda      contentum;
    Xar*        directivae;
    i32         idx;
    i32         numerus;

    via_cstr = chorda_ut_cstr(via, piscina);
    contentum = filum_legere_totum(via_cstr, piscina);

    si (contentum.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    directivae = _extrahere_directivas(contentum, piscina);
    numerus = (i32)xar_numerus(directivae);

    si (numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    printf("%.*s: %d directiv%s\n",
           (i32)via.mensura, (character*)via.datum,
           (s32)numerus,
           numerus == I ? "a" : "ae");

    per (idx = ZEPHYRUM; idx < numerus; idx++)
    {
        GenerareDirectiva* dir;
        s32                codex;

        dir = (GenerareDirectiva*)xar_obtinere(directivae, (i32)idx);
        si (dir == NIHIL || dir->instrumentum.datum == NIHIL)
        {
            perge;
        }

        codex = _executare_directivam(dir, via, loquax, piscina);
        si (codex != ZEPHYRUM)
        {
            fprintf(stderr, "ERRATUM: terminatur propter fracta in %.*s\n",
                    (i32)via.mensura, (character*)via.datum);
            redde -I;
        }
    }

    redde (s32)numerus;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(integer argc, character** argv)
{
    Piscina*          piscina;
    ArgumentaParser*  parser;
    ArgumentaFructus* fructus;
    b32               loquax;
    i32               numerus_viarum;
    i32               idx;
    Xar*              fila;
    s32               totalis_directivae;
    s32               totalis_fila;

    totalis_directivae = ZEPHYRUM;
    totalis_fila       = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("generare", LXIV * M);
    si (!piscina)
    {
        fprintf(stderr, "ERRATUM: non possum creare piscinam\n");
        redde I;
    }

    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "generare - Cursor directivarum generationis codicis");
    argumenta_addere_vexillum(parser, "-v", "--verbose",
        "Modus loquax");
    argumenta_addere_exemplum(parser,
        "generare lib/");
    argumenta_addere_exemplum(parser,
        "generare --verbose lib/persona.c");

    fructus = argumenta_parsere(parser, (i32)argc,
        (constans character* constans*)argv);

    loquax = argumenta_habet_vexillum(fructus, "--verbose");

    numerus_viarum = argumenta_numerus_positionalium(fructus);
    si (numerus_viarum == ZEPHYRUM)
    {
        fprintf(stderr, "Usus: generare [--verbose] <via> [<via> ...]\n");
        piscina_destruere(piscina);
        redde I;
    }

    fila = xar_creare(piscina, (i32)magnitudo(chorda));

    per (idx = ZEPHYRUM; idx < numerus_viarum; idx++)
    {
        chorda     via;
        character* via_cstr;

        via = argumenta_obtinere_positionalem(fructus, (i32)idx, piscina);
        via_cstr = chorda_ut_cstr(via, piscina);

        si (filum_directorium_existit(via_cstr))
        {
            ColligereContextus ctx;
            DirectoriumFiltrum filtrum;
            IntroitusGenus     genus_filum;

            ctx.fila    = fila;
            ctx.piscina = piscina;

            genus_filum = INTROITUS_FILUM;

            filtrum = directorium_filtrum_omnia();
            filtrum.genera_accepta = &genus_filum;
            filtrum.genera_numerus = I;
            filtrum.exemplar = chorda_ex_buffer(_c_glob_buf, C_GLOB_MENSURA);

            directorium_ambulare(via_cstr, &filtrum, _colligere_filum,
                                 &ctx, piscina);
        }
        alioquin si (filum_existit(via_cstr))
        {
            chorda* via_ptr;
            via_ptr = (chorda*)xar_addere(fila);
            si (via_ptr != NIHIL)
            {
                *via_ptr = chorda_transcribere(via, piscina);
            }
        }
        alioquin
        {
            fprintf(stderr, "Admonitio: via non inventa: %s\n", via_cstr);
        }
    }

    per (idx = ZEPHYRUM; idx < (i32)xar_numerus(fila); idx++)
    {
        chorda* via;
        s32     fructus_fili;

        via = (chorda*)xar_obtinere(fila, (i32)idx);
        si (via == NIHIL)
        {
            perge;
        }

        fructus_fili = _processare_filum(*via, loquax, piscina);
        si (fructus_fili < ZEPHYRUM)
        {
            piscina_destruere(piscina);
            redde I;
        }

        si (fructus_fili > ZEPHYRUM)
        {
            totalis_directivae += fructus_fili;
            totalis_fila++;
        }
    }

    printf("\nGenerare: %d directiv%s executat%s ex %d fil%s\n",
           (s32)totalis_directivae,
           totalis_directivae == I ? "a" : "ae",
           totalis_directivae == I ? "a" : "ae",
           (s32)totalis_fila,
           totalis_fila == I ? "o" : "is");

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
