/* formae.c - Instrumentum: forma paragraphi cuiusque cum indiciis (T6b)
 *
 * Usus: formae <plagula.txt> [-machina]
 * Plane: linea, forma, regula tenens, linea prima paragraphi (LX octeti).
 * -machina: TSV cum capite - indicia omnia (ut casus speciales numerentur
 * et regulae ex DATIS emendentur).
 * Exitus: 0 sanum | 1 nihil paragraphorum | 2 usus/plagula absens
 */

#include "latina.h"
#include "oratio_arbor.h"
#include "oratio_forma.h"
#include "oratio_registrum.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

interior vacuum
_lineam_primam_imprimere (
    constans character* textus,
                   i32  mensura,
                   s32  ab)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = (i32)ab; k < mensura && n < (i32)60; k++)
    {
        character c = textus[k];

        si (c == '\n' || c == '\r')
        {
            frange;
        }
        putchar(c == '\t' ? ' ' : c);
        n = n + I;
    }
}

integer
principale (
      integer   argc,
    character** argv)
{
             Piscina* piscina;
           character* textus;
                 i32  mensura = ZEPHYRUM;
        MateriaNodus* radix;
                 b32  machina = FALSUM;
  constans character* via     = NIHIL;
              integer i;
                 i32  np;
                 i32  p;
                 s32  cursor  = ZEPHYRUM;
                 i32  linea   = I;
constans MateriaValor* paragraphi;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin
        {
            via = argv[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: formae <plagula.txt> [-machina]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("oratio_formae_instrumentum",
        33554432);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "formae: plagula absens: %s\n", via);
        redde II;
    }
    radix = oratio_arbor_parsare(piscina, textus, mensura);
    si (radix == NIHIL)
    {
        fprintf(stderr, "formae: parsura fracta\n");
        redde II;
    }
    paragraphi = &radix->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    np = paragraphi->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*paragraphi) : ZEPHYRUM;
    si (machina)
    {
        imprimere("# linea\tforma\tregula\tlineae\telementa\tlatitudo\tmedia"
                  "\tvoluntariae-pct\tterminales-pct\tinterpunctae-pct"
                  "\tcapitales-pct\tcapitales-omnes-pct\tnotatae-pct"
                  "\tcolumnatae-pct\ttextus\n");
    }
    per (p = ZEPHYRUM; p < np; p++)
    {
        constans MateriaNodus* par =
            materia_valor_lista_obtinere(*paragraphi, p)->datum.nodus;
          constans OratioRegulaFormae* regula = NIHIL;
                        OratioIndicia  indicia;
                                  Xar* elementa;
                                  s32  initium;
                          OratioForma  forma;
                                  s32  k;

        elementa  = oratio_forma_elementa(piscina, par);
        initium   = oratio_forma_initium_lineae(par);
        oratio_forma_indicia(elementa, initium, &indicia);
        forma = oratio_forma_iudicare(&indicia, &regula);
        per (k = cursor; k < initium; k++)
        {
            si (textus[k] == '\n')
            {
                linea = linea + I;
            }
        }
        cursor = initium;
        si (machina)
        {
            imprimere("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",
                (integer)linea, oratio_forma_titulus(forma),
                regula != NIHIL ? regula->titulus : "-",
                (integer)indicia.lineae, (integer)indicia.elementa,
                (integer)indicia.latitudo,
                (integer)indicia.longitudo_media,
                (integer)indicia.voluntariae_pct,
                (integer)indicia.terminales_pct,
                (integer)indicia.interpunctae_pct,
                (integer)indicia.capitales_pct,
                (integer)indicia.capitales_omnes_pct,
                (integer)indicia.notatae_pct,
                (integer)indicia.columnatae_pct);
        }
        alioquin
        {
            imprimere("%5d %-8s %-24s ", (integer)linea,
                oratio_forma_titulus(forma),
                regula != NIHIL ? regula->titulus : "-");
        }
        si (forma != oratio_forma_paragraphi(par))
        {
            imprimere("[arbor: %s] ",
                oratio_forma_titulus(oratio_forma_paragraphi(par)));
        }
        _lineam_primam_imprimere(textus, mensura, initium);
        putchar('\n');
    }
    piscina_destruere(piscina);
    redde np > ZEPHYRUM ? ZEPHYRUM : I;
}
