/* probatio_html_corpus.c - Porta corporis: plagulae reales octetim
 *
 * Omnis plagula corporis parsatur et per materia_scribere_nodum
 * emittitur; emissio == fons OCTETIM, aut causa nominatur cum offset
 * primo divergente. Corpus: fixa IV (snapshots + adversarii II) et
 * paginae domus X per viam (briar facies, villa, mensor, silex,
 * knotapel, speculum, referentia) - omnes cum <script>, plurimae cum
 * <style>: textus crudus non casus angularis est sed omnis pagina.
 *
 * SE METIENS: 'N/N mundae' de plagulis numquam apertis mentiri
 * potest - numerus lectarum et octeti probati asseruntur.
 */

#include "latina.h"
#include "credo.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


hic_manens constans character* CORPUS[] = {
    "probationes/fixa/html/adversarius.html",
    "probationes/fixa/html/adversarius_2.html",
    "probationes/fixa/html/laboratorium_index_2026-08-18.html",
    "probationes/fixa/html/villa_index_2026-08-18.html",
    "briar/facies/facies.html",
    "briar/probationes/fixa/facies/salve.html",
    "briar/probationes/fixa/facies/circulus.html",
    "briar/probationes/fixa/facies/fragmenta.html",
    "apps/villa/assets/index.html",
    "apps/mensor/assets/index.html",
    "tools/silex_assets/index.html",
    "knotapel/atlas/web/index.html",
    "lib/speculum_assets/probatio_visus.html",
    "reference/aquinas-dithering-tool.html"
};

#define NUMERUS_CORPORIS ((i32)(magnitudo(CORPUS)/magnitudo(CORPUS[0])))

/* Fructus casus unius - causa NOMINATA, non b32 mutum */
enumeratio {
    CORPUS_IDEM = 0,
    CORPUS_NIHIL,
    CORPUS_EMISSIO_FRACTA,
    CORPUS_MENSURA_DISPAR,
    CORPUS_OCTETUS_DISPAR
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "emissio fracta",
    "mensura emissionis dispar",
    "octetus dispar"
};

hic_manens character*
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
    { redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(f); redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    { fclose(f); redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    { redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

/* Parsare, emittere, conferre. Offset primi octeti divergentis in
 * 'divergens' (aut mensura emissa si mensurae differunt). */
hic_manens integer
_plagulam_probare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   i32* divergens)
{
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;
    i32 i;

    *divergens  = ZEPHYRUM;
    radix       = html_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    { redde CORPUS_NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &HTML_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    { redde CORPUS_EMISSIO_FRACTA;
    }
    per (i = ZEPHYRUM; i < mensura && i < emissa.textus.mensura; i++)
    {
        si ((character)emissa.textus.datum[i] != fons[i])
        {
            *divergens = i;
            redde CORPUS_OCTETUS_DISPAR;
        }
    }
    si (emissa.textus.mensura != mensura)
    {
        *divergens = emissa.textus.mensura;
        redde CORPUS_MENSURA_DISPAR;
    }
    redde CORPUS_IDEM;
}

integer
principale (vacuum)
{
                Piscina* piscina;
                    b32  praeteritus;
     constans character* radix;
                    i32  i;
                    i32  lectae  = ZEPHYRUM;
                    i32  idem    = ZEPHYRUM;
                    i32  octeti  = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("probatio_html_corpus", 65536);
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    { radix = ".";
    }

    imprimere("\n--- PORTA: corpus HTML octetim (%d plagulae) ---\n",
        (integer)NUMERUS_CORPORIS);

    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        character  via[DXII];
        character* textus;
              i32  mensura    = ZEPHYRUM;
              i32  divergens  = ZEPHYRUM;
          integer  causa;

        sprintf(via, "%s/%s", radix, CORPUS[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", CORPUS[i]);
            CREDO_CULPA ("plagula corporis abest");
            perge;
        }
        lectae = lectae + I;
        octeti = octeti + mensura;

        causa = _plagulam_probare(piscina, textus, mensura, &divergens);
        imprimere("  %-56s %7d octeti  %s", CORPUS[i], (integer)mensura,
            CAUSAE[causa]);
        si (   causa == CORPUS_OCTETUS_DISPAR
            || causa == CORPUS_MENSURA_DISPAR)
        {
            imprimere(" @%d", (integer)divergens);
        }
        imprimere("\n");
        CREDO_AEQUALIS_S32 ((s32)causa, (s32)CORPUS_IDEM);
        si (causa == CORPUS_IDEM)
        {
            idem = idem + I;
        }
    }

    /* Porta CORPUS SUUM metiatur */
    CREDO_AEQUALIS_I32 (lectae, NUMERUS_CORPORIS);
    CREDO_AEQUALIS_I32 (idem, NUMERUS_CORPORIS);
    CREDO_MAIOR_I32 (octeti, (i32)(C * M));
    imprimere("  summa: %d plagulae, %d octeti\n", (integer)lectae,
        (integer)octeti);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
