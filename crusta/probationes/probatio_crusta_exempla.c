/* probatio_crusta_exempla.c - Lector casuum crustae (forma Oils)
 *
 * Contractus lectoris in plagula synthetica (sectio unius lineae,
 * plurium linearum, '## END', titulus detonsus, CRLF crudum in dato);
 * fixa domus (pathologiae.sh: XXXII casus; adversarius.sh: XVII casus
 * CRLF); manifestum FreeBSD (LXXXIX plagulae, XVIICCXCVII octeti -
 * probationes/fixa/crusta/FONTES.md) - plagula quaeque legibilis,
 * numeri pinnati.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_exempla.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior constans character*
_plagulam_legere (
               Piscina* piscina,
    constans character* radix,
    constans character* via,
                   i32* mensura)
{
    character plena[4096];
    FILE* f;
    longus longitudo;
    character* memoria;
    size_t lecti;

    sprintf(plena, "%s/%s", radix, via);
    f = fopen(plena, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    fseek(f, 0L, SEEK_SET);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    memoria[lecti]  = '\0';
    *mensura        = (i32)lecti;
    redde memoria;
}

interior b32
_chorda_est (
                chorda  c,
    constans character* litterae)
{
    redde c.mensura == (i32)strlen(litterae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, litterae, strlen(litterae)) == ZEPHYRUM);
}

interior constans CrustaExemplum*
_exemplum (
    Xar* exempla,
    i32  index)
{
    redde (constans CrustaExemplum*)xar_obtinere(exempla, index);
}

s32
principale (vacuum)
{
                        b32  praeteritus;
                    Piscina* piscina;
         constans character* radix;
                        Xar* exempla;
    constans CrustaExemplum* e;

    piscina = piscina_generare_dynamicum("probatio_crusta_exempla",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }


    /* ==================================================
     * PROBARE: contractus lectoris (plagula synthetica)
     * ================================================== */

    imprimere("\n--- Probans contractum lectoris ---\n");

    {
        constans character* fons =
            "praefatio\n"
            "#### primus  \n"
            "echo a\n"
            "\n"
            "## status: 1\n"
            "## STDOUT:\n"
            "out\n"
            "## not end\n"
            "## END\n"
            "## vacua\n"
            "####  secundus\r\n"
            "b\r\n"
            "#### tertius\n"
            "#### quartus";

        exempla = crusta_exempla_legere(piscina, fons,
            (i32)strlen(fons));
        CREDO_NON_NIHIL (exempla);
        CREDO_AEQUALIS_I32 (xar_numerus(exempla), (i32)IV);

        e = _exemplum(exempla, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (e->numerus, (i32)I);
        CREDO_AEQUALIS_I32 (e->linea, (i32)II);
        CREDO_VERUM (_chorda_est(e->titulus, "primus"));
        CREDO_VERUM (_chorda_est(e->datum, "echo a\n\n"));
        CREDO_AEQUALIS_I32 (xar_numerus(e->sectiones), (i32)III);
        CREDO_VERUM (_chorda_est(crusta_exemplum_sectio(e, "status"),
            "1"));
        CREDO_VERUM (_chorda_est(crusta_exemplum_sectio(e, "STDOUT"),
            "out\n## not end\n"));
        CREDO_VERUM (_chorda_est(crusta_exemplum_sectio(e, "vacua"),
            ""));
        CREDO_NON_NIHIL (crusta_exemplum_sectio(e, "vacua").datum);
        CREDO_NIHIL (crusta_exemplum_sectio(e, "absens").datum);
        CREDO_AEQUALIS_I32 (crusta_exemplum_sectio(e, "absens").mensura,
            ZEPHYRUM);

        /* CRLF: titulus detonsus, datum crudum cum '\r' */
        e = _exemplum(exempla, I);
        CREDO_VERUM (_chorda_est(e->titulus, "secundus"));
        CREDO_VERUM (_chorda_est(e->datum, "b\r\n"));
        CREDO_AEQUALIS_I32 (xar_numerus(e->sectiones), ZEPHYRUM);

        /* datum vacuum inter titulos; ultimus ad EOF sine linea nova */
        e = _exemplum(exempla, II);
        CREDO_VERUM (_chorda_est(e->titulus, "tertius"));
        CREDO_AEQUALIS_I32 (e->datum.mensura, ZEPHYRUM);
        e = _exemplum(exempla, III);
        CREDO_VERUM (_chorda_est(e->titulus, "quartus"));
        CREDO_AEQUALIS_I32 (e->datum.mensura, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (e->linea, (i32)XIV);
    }

    /* sectio plurium linearum aperta ad EOF: textus usque ad finem */
    {
        constans character* fons = "#### a\nx\n## OUT:\n1\n2\n";

        exempla = crusta_exempla_legere(piscina, fons,
            (i32)strlen(fons));
        e = _exemplum(exempla, ZEPHYRUM);
        CREDO_VERUM (_chorda_est(e->datum, "x\n"));
        CREDO_VERUM (_chorda_est(crusta_exemplum_sectio(e, "OUT"),
            "1\n2\n"));
    }

    /* plagula sine '####': nulla */
    {
        constans character* fons = "echo a\n## status: 0\n";

        exempla = crusta_exempla_legere(piscina, fons,
            (i32)strlen(fons));
        CREDO_NON_NIHIL (exempla);
        CREDO_AEQUALIS_I32 (xar_numerus(exempla), ZEPHYRUM);
    }


    /* ==================================================
     * PROBARE: fixa domus
     * ================================================== */

    imprimere("\n--- Probans fixa domus ---\n");

    {
                       i32  mensura;
        constans character* fons = _plagulam_legere(piscina, radix,
            "probationes/fixa/crusta/pathologiae.sh", &mensura);
        i32 i;
        i32 cum_linea = ZEPHYRUM;

        CREDO_NON_NIHIL (fons);
        exempla = crusta_exempla_legere(piscina, fons, mensura);
        CREDO_NON_NIHIL (exempla);
        CREDO_AEQUALIS_I32 (xar_numerus(exempla), (i32)XXXVI);
        CREDO_VERUM (_chorda_est(_exemplum(exempla, ZEPHYRUM)->titulus,
            "positio-verba"));
        CREDO_VERUM (_chorda_est(_exemplum(exempla, I)->datum,
            "cat <<A <<B | wc -l\none\nA\ntwo\nB\n"));
        CREDO_VERUM (_chorda_est(_exemplum(exempla, (i32)XXXV)->titulus,
            "arith-operandum-absens"));
        per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
        {
            e = _exemplum(exempla, i);
            si (   e->datum.mensura > ZEPHYRUM
                && e->datum.datum[e->datum.mensura - I] == '\n')
            {
                cum_linea++;
            }
        }
        CREDO_AEQUALIS_I32 (cum_linea, (i32)XXXVI);
    }

    {
                       i32  mensura;
        constans character* fons = _plagulam_legere(piscina, radix,
            "probationes/fixa/crusta/adversarius.sh", &mensura);
        i32 i;
        i32 cum_cr = ZEPHYRUM;

        CREDO_NON_NIHIL (fons);
        exempla = crusta_exempla_legere(piscina, fons, mensura);
        CREDO_AEQUALIS_I32 (xar_numerus(exempla), (i32)XVII);
        per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
        {
            e = _exemplum(exempla, i);
            si (   e->datum.mensura > ZEPHYRUM
                && memchr(e->datum.datum, '\r',
                (size_t)e->datum.mensura)
                    != NIHIL)
            {
                cum_cr++;
            }
        }
        /* omnes praeter ultimum ('echo a \' ad EOF) '\r' ferunt */
        CREDO_AEQUALIS_I32 (cum_cr, (i32)XVI);
        e = _exemplum(exempla, (i32)XVI);
        CREDO_VERUM (_chorda_est(e->titulus, "retroversum-ad-finem"));
        CREDO_VERUM (_chorda_est(e->datum, "echo a \\"));
        /* nidus CC: datum longum */
        e = _exemplum(exempla, (i32)XII);
        CREDO_VERUM (_chorda_est(e->titulus, "nidus-cc"));
        CREDO_MAIOR_I32 (e->datum.mensura, (i32)DC);
    }


    /* ==================================================
     * PROBARE: manifestum FreeBSD (FONTES.md pinnatum)
     * ================================================== */

    imprimere("\n--- Probans manifestum FreeBSD ---\n");

    {
        i32 plagulae  = ZEPHYRUM;
        i32 octeti    = ZEPHYRUM;
        i32 i;

        per (i = ZEPHYRUM; CRUSTA_FREEBSD_PLAGULAE[i] != NIHIL; i++)
        {
                           i32  mensura;
            constans character* fons = _plagulam_legere(piscina, radix,
                CRUSTA_FREEBSD_PLAGULAE[i], &mensura);

            si (fons == NIHIL)
            {
                imprimere("    absens: %s\n",
                    CRUSTA_FREEBSD_PLAGULAE[i]);
            }
            CREDO_NON_NIHIL (fons);
            si (fons == NIHIL)
            {
                perge;
            }
            plagulae++;
            octeti += mensura;
            CREDO_VERUM (mensura > ZEPHYRUM
                && fons[mensura - I] == '\n');
        }
        CREDO_AEQUALIS_I32 (plagulae, (i32)LXXXIX);
        CREDO_AEQUALIS_I32 (octeti, (i32)17297);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
