/* oraculum.c - Instrumentum oraculi treebank (T13, oratio/oraculum.sh)
 *
 * Usus: oraculum [-machina] [-exempla] <plagula.conllu>...
 *   sine plagulis: fixturae venditae oratio/probationes/fixa/ud
 *   -machina   TSV (plagula, classis, verba, tecta, primaria, lemmata,
 *              ignota, inalignata)
 *   -exempla   verba non tecta prima V per classem
 * Vocabularia (la.bin + glossarium + Moby) ex RHUBARB_RADIX. Exitus:
 * 0 relatio scripta | 2 usus/tabula/plagula absens
 */

#include "latina.h"
#include "oratio_conllu.h"
#include "oratio_oraculum.h"
#include "oratio_registrum.h"
#include "oratio_vocabularia.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

interior duplex
_pars (
    i32 a,
    i32 b)
{
    redde b > ZEPHYRUM ? 100.0 * (duplex)a / (duplex)b : 0.0;
}

interior vacuum
_tabulam_imprimere (
    constans OratioOraculumCensus* c,
               constans character* titulus,
                              b32  exempla)
{
    i32 i;

    imprimere("--- %s: sententiae %d (fractae %d)  verba %d  rangae %d ---\n",
        titulus, (integer)c->sententiae, (integer)c->sententiae_fractae,
        (integer)c->verba, (integer)c->rangae);
    imprimere("  TECTA %.1f%%  primaria %.1f%%  lemmata %.1f%%  ignota %.1f%%  inalignata %d\n",
        _pars(c->tecta, c->verba), _pars(c->primaria, c->verba),
        _pars(c->lemmata, c->verba), _pars(c->ignota, c->verba),
        (integer)c->inalignata);
    imprimere("  %-24s %6s %7s %8s %8s %7s\n", "classis aurea", "verba",
        "tecta", "primaria", "lemmata", "ignota");
    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans OratioOraculumClassis* k = &c->classes[i];
        constans character* t = i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
            ? oratio_classis_titulus((OratioClassis)i) : "(extra)";
        i32 j;

        si (k->verba == ZEPHYRUM)
        {
            perge;
        }
        imprimere("  %-24s %6d %6.1f%% %7.1f%% %7.1f%% %6.1f%%\n", t,
            (integer)k->verba, _pars(k->tecta, k->verba),
            _pars(k->primaria, k->verba), _pars(k->lemmata, k->verba),
            _pars(k->ignota, k->verba));
        si (exempla)
        {
            per (j = ZEPHYRUM; j < k->numerus_exemplorum; j++)
            {
                constans OratioOraculumExemplum* e = &k->exempla[j];

                imprimere("      %-20.*s  nostrum: %-40.*s  lemma aureum: %.*s\n",
                    (integer)e->forma.mensura,
                    (constans character*)e->forma.datum,
                    (integer)e->classes.mensura,
                    (constans character*)e->classes.datum,
                    (integer)e->lemma.mensura,
                    (constans character*)e->lemma.datum);
            }
        }
    }
}

interior vacuum
_machinam_imprimere (
    constans OratioOraculumCensus* c,
               constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans OratioOraculumClassis* k = &c->classes[i];

        si (k->verba == ZEPHYRUM)
        {
            perge;
        }
        imprimere("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", titulus,
            i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                ? oratio_classis_titulus((OratioClassis)i) : "extra",
            (integer)k->verba, (integer)k->tecta, (integer)k->primaria,
            (integer)k->lemmata, (integer)k->ignota,
            (integer)k->inalignata);
    }
    imprimere("%s\tSUMMA\t%d\t%d\t%d\t%d\t%d\t%d\n", titulus,
        (integer)c->verba, (integer)c->tecta, (integer)c->primaria,
        (integer)c->lemmata, (integer)c->ignota,
        (integer)c->inalignata);
}

integer
principale (
      integer   argc,
    character** argv)
{
                                Piscina* piscina;
                     constans character* radix;
                              character  via[1024];
                      OratioVocabularia  vocabularia;
               OratioVocabulariumVitium  vitium;
                                    b32  machina = FALSUM;
                                    b32  exempla = FALSUM;
                                integer  i;
                                    i32  plagulae = ZEPHYRUM;
    hic_manens constans character* constans venditae[] = {
        "oratio/probationes/fixa/ud/la_circse-ud-test.conllu",
        "oratio/probationes/fixa/ud/la_llct-ud-dev.conllu",
        "oratio/probationes/fixa/ud/la_llct-ud-test.conllu",
        "oratio/probationes/fixa/ud/en_ewt-ud-dev.conllu",
        "oratio/probationes/fixa/ud/en_ewt-ud-test.conllu"
    };

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    piscina = piscina_generare_dynamicum("oratio_oraculum", 536870912);
    si (!oratio_vocabularia_onerare(piscina, radix, &vocabularia,
        &vitium))
    {
        fprintf(stderr, "oraculum: vocabularia non onerata: %s:%d %s\n",
            vitium.plagula ? vitium.plagula : "?",
            (integer)vitium.linea,
            vitium.causa ? vitium.causa : "-");
        redde II;
    }
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-exempla") == ZEPHYRUM)
        {
            exempla = VERUM;
        }
    }
    si (machina)
    {
        imprimere("# plagula\tclassis\tverba\ttecta\tprimaria\tlemmata\tignota\tinalignata\n");
    }
    {
        i32 k;
        i32 numerus_argumentorum = ZEPHYRUM;

        per (i = I; i < argc; i++)
        {
            si (argv[i][ZEPHYRUM] != '-')
            {
                numerus_argumentorum = numerus_argumentorum + I;
            }
        }
        per (k = ZEPHYRUM;
             k < (numerus_argumentorum
                 > ZEPHYRUM ? (i32)argc
                 : (i32)(magnitudo(venditae)
                     / magnitudo(venditae[ZEPHYRUM]))
                     + I);
             k++)
        {
              constans character* plagula;
                          chorda  fons;
                         Piscina* p;
                             Xar* sententiae;
            OratioOraculumCensus  census;
                         clock_t  ante;

            si (numerus_argumentorum > ZEPHYRUM)
            {
                si (k == ZEPHYRUM || argv[k][ZEPHYRUM] == '-')
                {
                    perge;
                }
                plagula = argv[k];
                si (plagula[ZEPHYRUM] == '/')
                {
                    strcpy(via, plagula);
                }
                alioquin
                {
                    sprintf(via, "%s/%s", radix, plagula);
                }
            }
            alioquin
            {
                si (k == ZEPHYRUM)
                {
                    perge;
                }
                plagula = venditae[k - I];
                sprintf(via, "%s/%s", radix, plagula);
            }
            p = piscina_generare_dynamicum("oraculum_plagula",
                268435456);
            si (!_plagulam_legere(p, via, &fons))
            {
                fprintf(stderr, "oraculum: plagula absens: %s\n", via);
                redde II;
            }
            sententiae = oratio_conllu_legere(p, fons, &vitium);
            si (sententiae == NIHIL)
            {
                fprintf(stderr, "oraculum: %s: linea %d: %s\n", plagula,
                    (integer)vitium.linea,
                    vitium.causa ? vitium.causa : "?");
                redde II;
            }
            oratio_oraculum_census_vacare(&census);
            ante = clock();
            si (!oratio_oraculum_iudicare(p, &vocabularia, sententiae,
                    &census))
            {
                fprintf(stderr, "oraculum: iudicium fractum: %s\n",
                    plagula);
                redde II;
            }
            si (machina)
            {
                _machinam_imprimere(&census, plagula);
            }
            alioquin
            {
                _tabulam_imprimere(&census, plagula, exempla);
                imprimere("  %.0f ms\n\n", 1000.0 * (duplex)(clock()
                    - ante)
                    / (duplex)CLOCKS_PER_SEC);
            }
            plagulae = plagulae + I;
            piscina_destruere(p);
        }
    }
    (vacuum)plagulae;
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
