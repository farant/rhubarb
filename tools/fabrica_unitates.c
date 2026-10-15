/* fabrica_unitates.c - bin/fabrica quid: QUID EST HAEC PLAGULA
 * (spec unitates-codicis par. VI septem-octavae b, Q7; nucleus
 * lib/unitates.c, caput include/unitates.h)
 *
 * Usus (ex radice repositorii):
 *   bin/fabrica quid [-machina] VIA...
 *       unitas intima (cum catena), cura, vita, munus, provenientia,
 *       lingua - quisque valor cum regula quae eum decrevit
 *   bin/fabrica quid -iudex TITULUS [-machina] VIA...
 *       iudicatne iudex (unitates.stml <iudex>) viam? causa si non
 *   bin/fabrica quid -omnes [-machina]
 *       unitates omnes (genus, titulus, via, parens, declaratio)
 *
 * -machina: lineae TAB separatae -
 *   quid:    via  proprietas  valor  causa
 *   -iudex:  via  IUDICAT|NON_IUDICAT  causa
 *   -omnes:  genus  titulus  via  parens  declaratio
 * Exitus: 0 responsum · 2 usus, iudex ignotus aut RECUSATIO
 * politicae/declarationum (causae omnes in stderr).
 *
 * Sutura vera: lib/unitates_arboris.c (git ls-files -co + filum),
 * communis cum oratio vocabula. */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "unitates.h"
#include "unitates_arboris.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <string.h>

interior vacuum
_scribere (
    chorda c)
{
    fwrite(c.datum, I, (size_t)c.mensura, stdout);
}

interior vacuum
_proprietatem_imprimere (
                chorda  via,
    constans character* titulus,
    UnitatesProprietas  p,
                   b32  machina)
{
    si (machina)
    {
        _scribere(via);
        printf("\t%s\t", titulus);
        _scribere(p.valor);
        putchar('\t');
        _scribere(p.causa);
        putchar('\n');
        redde;
    }
    printf("  %-13s ", titulus);
    si (p.valor.mensura > ZEPHYRUM)
    {
        _scribere(p.valor);
    }
    alioquin
    {
        fputs("(nullum)", stdout);
    }
    fputs("  <- ", stdout);
    _scribere(p.causa);
    putchar('\n');
}

interior vacuum
_responsum_imprimere (
    constans UnitatisResponsum* r,
                           b32  machina,
                       Piscina* p)
{
       constans Unitas* x;
    UnitatesProprietas  unitas;
    UnitatesProprietas  catena;
     ChordaAedificator* a;

    a = chorda_aedificator_creare(p, (memoriae_index)CXXVIII);
    chorda_aedificator_appendere_chorda(a, r->unitas->titulus);
    chorda_aedificator_appendere_literis(a, " (");
    chorda_aedificator_appendere_chorda(a, r->unitas->genus);
    chorda_aedificator_appendere_literis(a, ")");
    unitas.valor = chorda_aedificator_finire(a);
    unitas.causa = r->unitatis_causa;

    a = chorda_aedificator_creare(p, (memoriae_index)CXXVIII);
    per (x = r->unitas; x != NIHIL; x = x->parens)
    {
        si (x != r->unitas)
        {
            chorda_aedificator_appendere_literis(a, " < ");
        }
        chorda_aedificator_appendere_chorda(a, x->titulus);
    }
    catena.valor = chorda_aedificator_finire(a);
    catena.causa = chorda_ex_literis("parentes unitatum", p);

    si (!machina)
    {
        _scribere(r->via);
        putchar('\n');
    }
    _proprietatem_imprimere(r->via, "unitas", unitas, machina);
    _proprietatem_imprimere(r->via, "catena", catena, machina);
    _proprietatem_imprimere(r->via, "cura", r->cura, machina);
    _proprietatem_imprimere(r->via, "vita", r->vita, machina);
    _proprietatem_imprimere(r->via, "munus", r->munus, machina);
    _proprietatem_imprimere(r->via, "provenientia", r->provenientia,
        machina);
    _proprietatem_imprimere(r->via, "lingua", r->lingua, machina);
}

interior vacuum
_omnes_imprimere (
    constans Unitates* u,
                  b32  machina,
              Piscina* piscina)
{
    constans Xar* omnes = unitates_omnes(u);
             i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(omnes); i++)
    {
           constans Unitas* x    = *(Unitas**)xar_obtinere(omnes, i);
        constans character* sep  = machina ? "\t" : "  ";

        _scribere(x->genus);
        fputs(sep, stdout);
        _scribere(x->titulus);
        fputs(sep, stdout);
        _scribere(x->via.mensura > ZEPHYRUM ? x->via
            : chorda_ex_literis(".", piscina));
        fputs(sep, stdout);
        si (x->parens != NIHIL)
        {
            _scribere(x->parens->titulus);
        }
        alioquin
        {
            fputs("-", stdout);
        }
        fputs(sep, stdout);
        _scribere(x->declaratio);
        putchar('\n');
    }
    si (!machina)
    {
        printf("unitates: %ld\n", (longus)xar_numerus(omnes));
    }
}

s32
suturae_quid (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
        UnitatesSutura* s;
              Unitates* u;
                chorda  causa;
    constans character* iudex    = NIHIL;
                   b32  machina  = FALSUM;
                   b32  omnes    = FALSUM;
                   s32  i;
                   s32  primus;

    per (i = II; i < argc && argv[i][ZEPHYRUM] == '-'; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-omnes") == ZEPHYRUM)
        {
            omnes = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-iudex") == ZEPHYRUM
                     && i + I < argc)
        {
            iudex = argv[++i];
        }
        alioquin
        {
            fprintf(stderr, "quid: vexillum ignotum '%s'\n", argv[i]);
            redde II;
        }
    }
    primus = i;
    si (omnes == (primus < argc) || (omnes && iudex != NIHIL))
    {
        fprintf(stderr, "usus: bin/fabrica quid [-machina] VIA...\n"
            "      bin/fabrica quid -iudex TITULUS [-machina] VIA...\n"
            "      bin/fabrica quid -omnes [-machina]\n");
        redde II;
    }

    s = unitates_sutura_arboris(NIHIL, piscina);
    u = unitates_legere(s, piscina, &causa, NIHIL);
    si (u == NIHIL)
    {
        fputs("quid: RECUSATIO unitatum:\n", stderr);
        fwrite(causa.datum, I, (size_t)causa.mensura, stderr);
        fputc('\n', stderr);
        redde II;
    }
    si (omnes)
    {
        _omnes_imprimere(u, machina, piscina);
        redde ZEPHYRUM;
    }
    per (i = primus; i < argc; i++)
    {
        chorda via = chorda_ex_literis(argv[i], piscina);

        si (iudex != NIHIL)
        {
            UnitatesIudicium j = unitates_iudicat(u, chorda_ex_literis(
                iudex, piscina), via, piscina, &causa);

            si (j == UNITATES_IUDEX_IGNOTUS)
            {
                fputs("quid: ", stderr);
                fwrite(causa.datum, I, (size_t)causa.mensura, stderr);
                fputc('\n', stderr);
                redde II;
            }
            _scribere(via);
            fputs(machina ? "\t" : "  ", stdout);
            fputs(j == UNITATES_IUDICAT ? "IUDICAT" : "NON_IUDICAT",
                stdout);
            si (causa.mensura > ZEPHYRUM)
            {
                fputs(machina ? "\t" : "  <- ", stdout);
                _scribere(causa);
            }
            alioquin si (machina)
            {
                putchar('\t');
            }
            putchar('\n');
        }
        alioquin
        {
            UnitatisResponsum r;

            si (!unitates_quid(u, via, piscina, &r))
            {
                fprintf(stderr, "quid: via vacua aut absoluta '%s'\n",
                    argv[i]);
                redde II;
            }
            _responsum_imprimere(&r, machina, piscina);
        }
    }
    redde ZEPHYRUM;
}
