/* census.c - census corporis markdown (md/census.sh)
 *
 * Usus: census -corpus <lista viarum> [-nota <textus>] [-machina | -citata]
 * Sine modo: tabulae markdown (md/CENSUS.md); -machina: TSV
 * 'clavis<tab>valor'; -citata: verbatim omnia 'via<tab>linea<tab>
 * contentum' (tabulae et lineae novae in contento spatia fiunt). Piscina
 * per plagulam (corpus XVII MB). Exitus: 0 sanum, 2 usus/lista absens.
 */

#include "latina.h"
#include "md_census.h"
#include "md_registrum.h"
#include "materia_registrum.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

interior integer
_linguas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans MdCensusLingua* la = (constans MdCensusLingua*)a;
    constans MdCensusLingua* lb = (constans MdCensusLingua*)b;

    si (la->numerus != lb->numerus)
    {
        redde (integer)lb->numerus - (integer)la->numerus;
    }
    redde strcmp(la->lingua, lb->lingua);
}

interior vacuum
_contentum_imprimere (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character ch = (character)c.datum[i];

        putchar((ch == '\t' || ch == '\n' || ch == '\r') ? ' ' : ch);
    }
}

interior vacuum
_ordinem (
    constans character* clavis,
                longus  valor,
                   b32  machina)
{
    si (machina)
    {
        imprimere("%s\t%ld\n", clavis, valor);
    }
    alioquin
    {
        imprimere("| %s | %ld |\n", clavis, valor);
    }
}

integer
principale (
      integer   argc,
    character** argv)
{
    constans character* lista_via  = NIHIL;
    constans character* nota       = "";
                   b32  machina    = FALSUM;
                   b32  citata     = FALSUM;
               Piscina* piscina;
              MdCensus  c;
                  FILE* lista;
             character  linea[DXII];
               integer  i;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-corpus") == ZEPHYRUM && i + I < argc)
        {
            lista_via = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-nota") == ZEPHYRUM
                     && i + I < argc)
        {
            nota = argv[++i];
        }
        alioquin si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-citata") == ZEPHYRUM)
        {
            citata = VERUM;
        }
        alioquin
        {
            fprintf(stderr, "usus: census -corpus <lista> [-nota <t>]"
                " [-machina | -citata]\n");
            redde II;
        }
    }
    si (lista_via == NIHIL)
    {
        fprintf(stderr, "census: -corpus <lista> deest\n");
        redde II;
    }
    lista = fopen(lista_via, "r");
    si (lista == NIHIL)
    {
        fprintf(stderr, "census: lista absens: %s\n", lista_via);
        redde II;
    }
    piscina = piscina_generare_dynamicum("md_census", 4194304);
    md_census_nudum(&c, piscina);
    dum (fgets(linea, (integer)magnitudo(linea), lista) != NIHIL)
    {
           Piscina* p;
         character* textus;
               i32  mensura  = ZEPHYRUM;
            size_t  l        = strlen(linea);

        dum (   l > ZEPHYRUM
             && (linea[l - I] == '\n' || linea[l - I] == '\r'))
        {
            linea[--l] = '\0';
        }
        si (l == ZEPHYRUM)
        {
            perge;
        }
        p = piscina_generare_dynamicum("md_census_plagula",
            16777216);
        textus = _plagulam_legere(p, linea, &mensura);
        si (textus == NIHIL)
        {
            fprintf(stderr, "census: plagula absens: %s\n", linea);
            piscina_destruere(p);
            perge;
        }
        si (citata)
        {
            Xar* xs  = md_census_citata(p, textus, mensura);
            i32  n   = xs != NIHIL ? xar_numerus(xs) : ZEPHYRUM;
            i32  k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                constans MdCitatum* x =
                    (constans MdCitatum*)xar_obtinere(xs, k);

                imprimere("%s\t%d\t", linea, (integer)x->linea);
                _contentum_imprimere(x->contentum);
                putchar('\n');
            }
        }
        alioquin
        {
            md_census_addere(&c, p, textus, mensura);
        }
        piscina_destruere(p);
    }
    fclose(lista);
    si (citata)
    {
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    si (!machina)
    {
        imprimere("# Census corporis markdown\n\n");
        imprimere("GENERATUM `./md/census.sh -scribere` (%s) - noli manu"
                  " emendare. Numeri ex ARBORE PARSATA (md_census), non ex"
                  " grep; corpus = `git ls-files '*.md'`.\n\n", nota);
        imprimere("## Summa\n\n| mensura | valor |\n|---|---|\n");
    }
    _ordinem("plagulae", (longus)c.plagulae, machina);
    _ordinem("fractae", (longus)c.fractae, machina);
    _ordinem("octeti", (longus)c.octeti, machina);
    _ordinem("lineae", (longus)c.lineae, machina);
    _ordinem("lexemata", (longus)c.lexemata, machina);
    _ordinem("derivata", (longus)c.derivata, machina);

    si (!machina)
    {
        imprimere("\n## Nodi per genus\n\n| genus | numerus |\n|---|---|\n");
    }
    per (i = ZEPHYRUM; i < (integer)MD_REGISTRUM.numerus_generum; i++)
    {
        character clavis[LXIV];

        sprintf(clavis, machina ? "genus.%s" : "%s",
            MD_REGISTRUM.genera[i].titulus);
        _ordinem(clavis, (longus)c.genera[i], machina);
    }

    si (!machina)
    {
        imprimere("\n## Capitula per gradum\n\n| gradus | numerus |\n|---|---|\n");
    }
    per (i = I; i <= VI; i++)
    {
        character clavis[XVI];

        sprintf(clavis, machina ? "capitulum.h%d" : "h%d", i);
        _ordinem(clavis, (longus)c.capitula_gradus[i], machina);
    }

    si (!machina)
    {
        imprimere("\n## Listae\n\n| mensura | valor |\n|---|---|\n");
    }
    _ordinem(machina ? "lista.punctatae" : "punctatae",
        (longus)c.listae_punctatae, machina);
    _ordinem(machina ? "lista.numeratae" : "numeratae",
        (longus)c.listae_numeratae, machina);
    _ordinem(machina ? "lista.laxae" : "laxae", (longus)c.listae_laxae,
        machina);
    _ordinem(machina ? "officium.aperta" : "officia aperta `- [ ]`",
        (longus)c.officia_aperta, machina);
    _ordinem(machina ? "officium.perfecta" : "officia perfecta `- [x]`",
        (longus)c.officia_perfecta, machina);

    si (!machina)
    {
        imprimere("\n## Saepta\n\n| mensura | valor |\n|---|---|\n");
    }
    _ordinem(machina ? "saeptum.saepta" : "saepta (fences)",
        (longus)c.saepta_saepta, machina);
    _ordinem(machina ? "saeptum.indentata" : "indentata",
        (longus)c.saepta_indentata, machina);

    si (!machina)
    {
        imprimere("\n## Linguae saeptorum\n\n| lingua | numerus |\n|---|---|\n");
    }
    {
                   i32  n = xar_numerus(c.linguae);
        MdCensusLingua* ordinatae =
            (MdCensusLingua*)piscina_allocare(piscina,
            (memoriae_index)(n
                > ZEPHYRUM ? n : I) * magnitudo(MdCensusLingua));
        i32 k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            ordinatae[k] = *(MdCensusLingua*)xar_obtinere(c.linguae, k);
        }
        qsort(ordinatae, (size_t)n, magnitudo(MdCensusLingua),
            _linguas_comparare);
        per (k = ZEPHYRUM; k < n; k++)
        {
            character clavis[LXIV];

            sprintf(clavis, machina ? "lingua.%s" : "%s",
                ordinatae[k].lingua);
            _ordinem(clavis, (longus)ordinatae[k].numerus, machina);
        }
    }

    si (!machina)
    {
        imprimere("\n## Nexus\n\n| forma | numerus |\n|---|---|\n");
    }
    _ordinem(machina ? "nexus.inlinei" : "inlinei `[a](/u)`",
        (longus)c.nexus_inlinei, machina);
    _ordinem(machina ? "nexus.referentes" : "referentes `[a][x]`",
        (longus)c.nexus_referentes, machina);
    _ordinem(machina ? "nexus.auto" : "auto `<http://…>`",
        (longus)c.nexus_auto, machina);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
