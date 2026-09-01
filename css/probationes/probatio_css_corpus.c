/* probatio_css_corpus.c - Porta corporis (B5, spec par. IX.6)
 *
 * VI plagulae VERAE per circuitum octetorum: parsura -> emissio
 * DIRECTA (materia_scribere, oraculum separans) -> memcmp contra
 * discum. Corpus > fixturae, semper: fixtura praesumptiones meas
 * fert, plagula vera quod verum est.
 *
 * PORTA TEGUMENTUM SUUM METITUR: 'VI/VI mundae' mentitur si nulla
 * plagula umquam aperta est - lectae et octeti asseruntur, numquam
 * praesumuntur (lex domus: N/N mundum potest significare 'numquam
 * apparuit').
 */

#include "latina.h"
#include "credo.h"
#include "css_arbor.h"
#include "css_registrum.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* CORPUS[] = {
    "probationes/fixa/css/componentia_2026-08-18.css",
    "probationes/fixa/css/thema_2026-08-18.css",
    "probationes/fixa/css/adversarius.css",
    "probationes/fixa/css/adversarius_2.css",
    "lib/speculum_assets/speculum.css",
    "knotapel/atlas/web/style.css"
};

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

/* Numerus nodorum generis dati, recursivus. Oraculum octetorum
 * unum IGNOSCIT: parsator qui omnia in 'mala' congereret adhuc
 * octetim exactus esset. Ergo plagulae MUNDAE etiam structuram
 * asserunt: nullae malae, declarationes verae. */
interior i32
_genera_numerare (
    constans MateriaNodus* nodus,
                      s32  genus)
{
    i32 summa;
    i32 i;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    summa = (nodus->genus == genus) ? I : ZEPHYRUM;
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            summa = summa + _genera_numerare(v->datum.nodus, genus);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    summa = summa
                          + _genera_numerare(e->datum.nodus, genus);
                }
            }
        }
    }
    redde summa;
}

/* Oraculum separans: emissio directa ex parsura, sine STML. */
interior b32
_octetos_probare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    radix = css_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    materia_scriptura_consilium_nudum(&consilium, &CSS_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    {
        redde FALSUM;
    }
    si (emissa.textus.mensura != mensura)
    {
        imprimere("    (emissa %d contra fontem %d)\n",
            (integer)emissa.textus.mensura, (integer)mensura);
        redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(emissa.textus.datum, fons,
                       (size_t)mensura) == ZEPHYRUM);
}

integer
principale (vacuum)
{
               Piscina* piscina;
    constans character* radix_viae;
                   b32  praeteritus;
                   i32  numerus;
                   i32  i;
                   i32  plagulae_lectae;
                   s32  octeti_probati;

    piscina = piscina_generare_dynamicum("probatio_css_corpus",
        1048576);
    credo_aperire(piscina);

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }

    numerus          = (i32)(magnitudo(CORPUS) / magnitudo(CORPUS[0]));
    plagulae_lectae  = ZEPHYRUM;
    octeti_probati   = ZEPHYRUM;

    imprimere("\n--- PORTA CORPORIS: VI plagulae verae ---\n");

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;

        sprintf(via, "%s/%s", radix_viae, CORPUS[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", CORPUS[i]);
            perge;
        }
        plagulae_lectae  = plagulae_lectae + I;
        octeti_probati   = octeti_probati + (s32)mensura;
        imprimere("  %s (%d octeti)\n", CORPUS[i], (integer)mensura);
        CREDO_VERUM (_octetos_probare(piscina, textus, mensura));

        /* Plagulae MUNDAE (non adversariae, indices 2 et 3):
         * structura quoque asseritur, ne viridis vacua sit. */
        si (i != II && i != III)
        {
            MateriaNodus* radix = css_arbor_parsare(piscina, textus,
                mensura);

            CREDO_NON_NIHIL (radix);
            CREDO_AEQUALIS_I32 (_genera_numerare(radix,
                (s32)CSS_GENUS_REGULA_MALA), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_genera_numerare(radix,
                (s32)CSS_GENUS_DECLARATIO_MALA), ZEPHYRUM);
            CREDO_MAIOR_S32 ((s32)_genera_numerare(radix,
                (s32)CSS_GENUS_DECLARATIO), (s32)ZEPHYRUM);
        }
    }

    /* Tegumentum SUUM: plagula absens rubet, non tacet */
    CREDO_AEQUALIS_I32 (plagulae_lectae, VI);
    CREDO_MAIOR_S32 (octeti_probati, (s32)4000);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
