/* probatio_crusta_corpus.c - Corpus crustae octetim: plagulae domus
 * (.sh tractae, lista build/crusta_corpus.lst a cursore scripta),
 * fixa (pathologiae, adversarius), casus FreeBSD (manifestum
 * CRUSTA_FREEBSD_PLAGULAE). Quaeque: parsare -> materia_scribere_nodum
 * -> memcmp (idem), et tegumentum lectoris: lexemata quae aedificator
 * accepit concatenata = fons. Causae numeratae (idem / NIHIL / emissio
 * fracta / dispar / tegumentum), offset primum divergens nominatum.
 * Mensura sui: plagulae >= CCXXX, octeti > DCCC milia. Mala corporis
 * domus per plagulam relata (pinnanda P11).
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_exempla.h"
#include "crusta_registrum.h"
#include "materia_scribere.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
    i32 plagulae;
    i32 octeti;
    i32 idem;
    i32 nihil;
    i32 emissio_fracta;
    i32 dispar;
    i32 tegumentum_fractum;
    i32 insanae;
    i32 mala;
} Summa;

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

/* offset primum divergens; -I si aequales */
interior s32
_divergentia (
    constans character* a,
    constans character* b,
                   i32  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (a[i] != b[i])
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

/* lexemata concatenata contra fontem */
interior b32
_tegumentum (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   Xar* lexemata,
                   s32* offset)
{
    character* area = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    i32 scripti = ZEPHYRUM;
    i32 i;

    si (area == NIHIL)
    {
        *offset = ZEPHYRUM;
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(lexemata); i++)
    {
        constans MateriaToken* t =
            *(MateriaToken**)xar_obtinere(lexemata,
            i);

        si (scripti + t->valor.mensura > mensura)
        {
            *offset = (s32)scripti;
            redde FALSUM;
        }
        memcpy(area + scripti, t->valor.datum,
            (size_t)t->valor.mensura);
        scripti += t->valor.mensura;
    }
    si (scripti != mensura)
    {
        *offset = (s32)scripti;
        redde FALSUM;
    }
    *offset = _divergentia(area, fons, mensura);
    redde *offset < ZEPHYRUM;
}

/* plagulam unam per portam ducere; piscina propria */
interior vacuum
_plagula (
    constans character* radix,
    constans character* via,
                 Summa* summa,
                   b32  domus)
{
    Piscina* piscina = piscina_generare_dynamicum("crusta_corpus",
        4194304);
                   i32  mensura;
    constans character* fons;
    MateriaNodus* radix_arboris;
    CrustaParsura relatio;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;
    Xar* lexemata;
    s32 offset;

    si (piscina == NIHIL)
    {
        imprimere("    piscina deficit: %s\n", via);
        summa->nihil++;
        redde;
    }
    fons = _plagulam_legere(piscina, radix, via, &mensura);
    si (fons == NIHIL)
    {
        imprimere("    illegibilis: %s\n", via);
        summa->nihil++;
        piscina_destruere(piscina);
        redde;
    }
    summa->plagulae++;
    summa->octeti  += mensura;
    lexemata       = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    radix_arboris = crusta_arbor_parsare_cum_lexematis(piscina, fons,
        mensura, &CRUSTA_BASH, &relatio, lexemata);
    si (radix_arboris == NIHIL)
    {
        imprimere("    NIHIL: %s\n", via);
        summa->nihil++;
        piscina_destruere(piscina);
        redde;
    }
    materia_scriptura_consilium_nudum(&consilium, &CRUSTA_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix_arboris, &consilium);
    si (!emissa.successus)
    {
        imprimere("    emissio fracta: %s\n", via);
        summa->emissio_fracta++;
    }
    alioquin si (emissa.textus.mensura != mensura)
    {
        imprimere("    dispar (mensura %d pro %d): %s\n",
            (integer)emissa.textus.mensura, (integer)mensura, via);
        summa->dispar++;
    }
    alioquin
    {
        offset = _divergentia((constans character*)emissa.textus.datum,
            fons, mensura);
        si (offset >= ZEPHYRUM)
        {
            imprimere("    dispar (offset %ld): %s\n", (longus)offset,
                via);
            summa->dispar++;
        }
        alioquin
        {
            summa->idem++;
        }
    }
    si (!_tegumentum(piscina, fons, mensura, lexemata, &offset))
    {
        imprimere("    tegumentum fractum (offset %ld): %s\n",
            (longus)offset, via);
        summa->tegumentum_fractum++;
    }
    si (domus && !relatio.sana)
    {
        imprimere("    insana (mala %d, clausurae absentes %d): %s\n",
            (integer)relatio.mala, (integer)relatio.clausurae_absentes,
            via);
        summa->insanae++;
        summa->mala += relatio.mala;
    }
    piscina_destruere(piscina);
}

interior b32
_eadem_via (
    constans character* a,
    constans character* b)
{
    redde strcmp(a, b) == ZEPHYRUM;
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix;
    constans character* lista;
                   i32  mensura;
                 Summa  summa;
                 Summa  fixa;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_crusta_corpus",
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
    memset(&summa, ZEPHYRUM, magnitudo(summa));
    memset(&fixa, ZEPHYRUM, magnitudo(fixa));


    /* ==================================================
     * PROBARE: corpus domus (lista a cursore scripta)
     * ================================================== */

    imprimere("\n--- Probans corpus domus ---\n");

    lista = _plagulam_legere(piscina, radix, "build/crusta_corpus.lst",
        &mensura);
    si (lista == NIHIL)
    {
        CREDO_CULPA ("build/crusta_corpus.lst absens - cursor eam "
            "scribit (git ls-files '*.sh')");
    }
    alioquin
    {
        i32 ab = ZEPHYRUM;

        dum (ab < mensura)
        {
                  i32 ad = ab;
            character via[4096];

            dum (ad < mensura && lista[ad] != '\n')
            {
                ad++;
            }
            si (ad > ab && ad - ab < (i32)4096)
            {
                memcpy(via, lista + ab, (size_t)(ad - ab));
                via[ad - ab] = '\0';
                si (   !_eadem_via(via,
                        "probationes/fixa/crusta/pathologiae.sh")
                    && !_eadem_via(via,
                        "probationes/fixa/crusta/adversarius.sh"))
                {
                    _plagula(radix, via, &summa, VERUM);
                }
            }
            ab = ad + I;
        }
    }
    imprimere("    plagulae %d, octeti %d: idem %d, NIHIL %d, emissio "
        "fracta %d, dispar %d, tegumentum fractum %d; insanae %d "
        "(mala %d)\n",
        (integer)summa.plagulae, (integer)summa.octeti,
        (integer)summa.idem,
        (integer)summa.nihil, (integer)summa.emissio_fracta,
        (integer)summa.dispar, (integer)summa.tegumentum_fractum,
        (integer)summa.insanae, (integer)summa.mala);
    CREDO_VERUM (summa.plagulae >= (i32)CCXXX);
    CREDO_VERUM (summa.octeti > (i32)800000);
    CREDO_AEQUALIS_I32 (summa.nihil, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summa.emissio_fracta, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summa.dispar, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summa.tegumentum_fractum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summa.idem, summa.plagulae);


    /* ==================================================
     * PROBARE: fixa et FreeBSD
     * ================================================== */

    imprimere("\n--- Probans fixa et casus FreeBSD ---\n");

    _plagula(radix, "probationes/fixa/crusta/pathologiae.sh", &fixa,
        FALSUM);
    _plagula(radix, "probationes/fixa/crusta/adversarius.sh", &fixa,
        FALSUM);
    per (i = ZEPHYRUM; CRUSTA_FREEBSD_PLAGULAE[i] != NIHIL; i++)
    {
        _plagula(radix, CRUSTA_FREEBSD_PLAGULAE[i], &fixa, FALSUM);
    }
    imprimere("    plagulae %d, octeti %d: idem %d, NIHIL %d, emissio "
        "fracta %d, dispar %d, tegumentum fractum %d\n",
        (integer)fixa.plagulae, (integer)fixa.octeti,
        (integer)fixa.idem,
        (integer)fixa.nihil, (integer)fixa.emissio_fracta,
        (integer)fixa.dispar, (integer)fixa.tegumentum_fractum);
    CREDO_AEQUALIS_I32 (fixa.plagulae, (i32)XCI);
    CREDO_AEQUALIS_I32 (fixa.nihil, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fixa.emissio_fracta, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fixa.dispar, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fixa.tegumentum_fractum, ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
