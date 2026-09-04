/* quaere.c - Instrumentum quaesitionis Latinae (T8)
 *
 * Usus: quaere [-machina] forma...
 * Plane: pro forma quaque analyses ordine WORDS - stirps.finis, pars,
 * declinatio/varians, accidentia, lemma (forma prima), codices, sensus
 * (LX octeti); IGNOTUM si nulla. -machina: TSV cum capite.
 * Radix per RHUBARB_RADIX. Exitus: 0 (etiam ignota - inventa, non
 * vitia) | 2 usus/tabula absens
 */

#include "latina.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"

#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

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
    exitus->datum    = memoria;
    exitus->mensura  = (i32)longitudo;
    redde VERUM;
}

interior vacuum
_chordam (
    chorda c,
       i32 maxima)
{
    i32 n = c.mensura < maxima ? c.mensura : maxima;

    imprimere("%.*s", (integer)n, (constans character*)c.datum);
}

integer
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
     constans character* radix;
              character  via[1024];
                 chorda  tabula;
   OratioVocabulariumLa* voc;
OratioVocabulariumVitium vitium;
                    b32  machina = FALSUM;
                integer  i;
                    i32  formae = ZEPHYRUM;

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    piscina = piscina_generare_dynamicum("oratio_quaere", 268435456);
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    si (!_plagulam_legere(piscina, via, &tabula))
    {
        fprintf(stderr, "quaere: tabula absens: %s\n", via);
        redde II;
    }
        voc = oratio_vocabularium_la_onerare(piscina, tabula, &vitium);
    si (voc == NIHIL)
    {
        fprintf(stderr, "quaere: onus fractum: %s\n", vitium.causa);
        redde II;
    }
    /* glossarium domus, si adest: fons primus */
    sprintf(via, "%s/oratio/glossarium.stml", radix);
    {
        chorda fons_glossarii;

        si (_plagulam_legere(piscina, via, &fons_glossarii))
        {
            OratioGlossarium* gl = oratio_glossarium_legere(piscina,
                fons_glossarii, &vitium);

            si (gl == NIHIL)
            {
                fprintf(stderr,
                    "quaere: glossarium non legitur: %s:%d %s\n",
                    vitium.plagula, (integer)vitium.linea,
                    vitium.causa);
                redde II;
            }
            oratio_vocabularium_la_glossarium_ponere(voc, gl);
        }
    }

    per (i = I; i < argc; i++)
    {
        constans character* arg = argv[i];
                    chorda  forma;
                       Xar* analyses;
                       i32  a;

        si (strcmp(arg, "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
            si (i == I)
            {
                imprimere("# forma\tgenus\tstirps\tfinis\tpars\tdecl\tvar\taccidentia\tlemma\tcodices\tlinea\tsensus\n");
            }
            perge;
        }
        formae = formae + I;
        forma.datum = (i8*)argv[i];
        forma.mensura = (i32)strlen(arg);
        analyses = oratio_vocabularium_la_quaerere(piscina, voc, forma);
        si (analyses == NIHIL)
        {
            fprintf(stderr, "quaere: memoria\n");
            redde II;
        }
        si (!machina)
        {
            imprimere("%s: %d analyses\n", arg,
                (integer)xar_numerus(analyses));
        }
        si (xar_numerus(analyses) == ZEPHYRUM)
        {
            si (machina)
            {
                imprimere("%s\tIGNOTUM\n", arg);
            }
            alioquin
            {
                imprimere("  IGNOTUM\n");
            }
        }
        per (a = ZEPHYRUM; a < xar_numerus(analyses); a++)
        {
                        constans OratioAnalysis* an =
                            (constans OratioAnalysis*)xar_obtinere(analyses,
                            a);


            si (an->genus == ORATIO_ANALYSIS_STIRPS)
            {
                constans OratioStirps* st =
                    oratio_vocabularium_la_stirps(voc,
                    an->stirps);
                constans OratioFlexio* fl =
                    oratio_vocabularium_la_flexio(voc,
                    an->flexio);
                chorda lemma = oratio_vocabularium_la_lemma(piscina,
                    voc,
                    an->stirps);

                si (machina)
                {
                    imprimere("%s\tstirps\t", arg);
                    _chordam(st->stirpes[an->clavis - I], (i32)32);
                    putchar('\t');
                    _chordam(fl->finis, (i32)8);
                    imprimere("\t%s\t%d\t%d\t",
                        oratio_pars_titulus(st->pars),
                        (integer)st->declinatio, (integer)st->varians);
                    _chordam(fl->accidentia, (i32)64);
                    putchar('\t');
                    _chordam(lemma, (i32)32);
                    imprimere("\t%c%c%c%c%c\t%d\t", st->aetas, st->area,
                        st->geo, st->frequentia, st->fons,
                        (integer)st->linea);
                    _chordam(st->sensus, (i32)200);
                    putchar('\n');
                }
                alioquin
                {
                                        imprimere("  ");
                    _chordam(st->stirpes[an->clavis - I], (i32)32);

                    si (fl->finis.mensura > ZEPHYRUM)
                    {
                        putchar('.');
                        _chordam(fl->finis, (i32)8);
                    }
                    imprimere("  %s %d %d ",
                        oratio_pars_titulus(st->pars),
                        (integer)st->declinatio, (integer)st->varians);
                    _chordam(fl->accidentia, (i32)64);
                    imprimere("  [");
                    _chordam(lemma, (i32)32);
                    imprimere("]  %c%c%c%c%c  ", st->aetas, st->area,
                        st->geo,
                        st->frequentia, st->fons);
                    _chordam(st->sensus, (i32)60);
                    si (an->tackon >= ZEPHYRUM)
                    {
                        imprimere("  +");
                        _chordam(oratio_vocabularium_la_additamentum(voc,
                            an->tackon)->forma, (i32)8);
                    }
                    putchar('\n');
                }
            }
                        alioquin si (an->genus
                                     == ORATIO_ANALYSIS_GLOSSARIUM)
            {
                constans OratioGlossarium* gl =
                    oratio_vocabularium_la_glossarium(voc);
                constans OratioGlossariumForma* fg =
                    oratio_glossarium_forma(gl,
                    an->glossarium);
                constans OratioGlossariumEntrium* eg =
                    oratio_glossarium_entrium(
                    gl, fg->entrium);

                si (machina)
                {
                    imprimere("%s\tglossarium\t", arg);
                    _chordam(fg->textus, (i32)32);
                    imprimere("\t\t");
                    _chordam(eg->classis, (i32)32);
                    imprimere("\t\t\t");
                    _chordam(fg->persona, (i32)8); putchar(' ');
                    _chordam(fg->numerus, (i32)16); putchar(' ');
                    _chordam(fg->tempus, (i32)24); putchar(' ');
                    _chordam(fg->modus, (i32)16); putchar(' ');
                    _chordam(fg->vox, (i32)8);
                    putchar('\t');
                    _chordam(eg->lemma, (i32)32);
                    imprimere("\t\t%d\t", (integer)fg->linea);
                    _chordam(eg->sensus, (i32)200);
                    putchar('\n');
                }
                alioquin
                {
                    imprimere("  ");
                    _chordam(fg->textus, (i32)32);
                    imprimere("  GLOSSARIUM ");
                    _chordam(eg->classis, (i32)32);
                    putchar(' ');
                    _chordam(fg->persona, (i32)8); putchar(' ');
                    _chordam(fg->numerus, (i32)16); putchar(' ');
                    _chordam(fg->tempus, (i32)24); putchar(' ');
                    _chordam(fg->modus, (i32)16); putchar(' ');
                    _chordam(fg->vox, (i32)8);
                    imprimere("  [");
                    _chordam(eg->lemma, (i32)32);
                    imprimere("]  ");
                    _chordam(eg->sensus, (i32)60);
                    putchar('\n');
                }
            }
            alioquin si (an->genus == ORATIO_ANALYSIS_UNICUM)
            {
                constans OratioUnicum* u =
                    oratio_vocabularium_la_unicum(voc,
                    an->unicum);


                si (machina)
                {
                    imprimere("%s\tunicum\t", arg);
                    _chordam(u->forma, (i32)32);
                    imprimere("\t\t\t\t\t");
                    _chordam(u->descriptio, (i32)64);
                    imprimere("\t\t\t%d\t", (integer)u->linea);
                    _chordam(u->sensus, (i32)200);
                    putchar('\n');
                }
                alioquin
                {
                    imprimere("  ");
                    _chordam(u->forma, (i32)32);
                    imprimere("  UNICUM ");
                    _chordam(u->descriptio, (i32)64);
                    imprimere("  ");
                    _chordam(u->sensus, (i32)60);
                    putchar('\n');
                }
            }
            alioquin
            {
                constans OratioAdditamentum* t =
                    oratio_vocabularium_la_additamentum(voc,
                    an->tackon);

                si (machina)
                {
                    imprimere("%s\ttackon\t\t", arg);
                    _chordam(t->forma, (i32)8);
                    imprimere("\tTACKON\t\t\t\t\t\t%d\t",
                        (integer)t->linea);
                    _chordam(t->sensus, (i32)200);
                    putchar('\n');
                }
                alioquin
                {
                    imprimere("  -");
                    _chordam(t->forma, (i32)8);
                    imprimere("  TACKON  ");
                    _chordam(t->sensus, (i32)60);
                    putchar('\n');
                }
            }
        }
    }
    si (formae == ZEPHYRUM)
    {
        fprintf(stderr, "usus: quaere [-machina] forma...\n");
        redde II;
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
