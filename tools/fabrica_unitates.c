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
 *   bin/fabrica quid -census [-machina]
 *       census (U5a): unitates cum lacunis (exspectationes politicae
 *       per genus non impletae), probationes radicis orphanae,
 *       directoria cum codice sine declaratione - relatio, exitus 0
 *   bin/fabrica quid -interrogationes [-machina] [UNITAS...]
 *       interrogationes (U5b): quaestiones typatae unitatum (omnium,
 *       aut nominatarum) cum responsis et ancoris, numeri per genus,
 *       vitia plagularum - exitus 1 si vitium aliquod
 *
 * -machina: lineae TAB separatae -
 *   quid:    via  proprietas  valor  causa
 *   -iudex:  via  IUDICAT|NON_IUDICAT  causa
 *   -omnes:  genus  titulus  via  parens  declaratio
 *   -interrogationes: INTERROGATIO  unitas  via  linea  genus  quaestio
 *            responsum  symbola  viae  imperium (indices spatiis);
 *            VITIUM  via  linea  causa; SUMMA  ...
 * Exitus: 0 responsum · 1 vitia interrogationum · 2 usus, iudex aut
 * unitas ignota, aut RECUSATIO politicae/declarationum (causae omnes
 * in stderr).
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
_numeros_imprimere (
    constans Xar* numeri)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(numeri); i++)
    {
        constans UnitatesNumerus* n = (constans UnitatesNumerus*)
            xar_obtinere(numeri, i);

        si (i > ZEPHYRUM)
        {
            fputs(", ", stdout);
        }
        _scribere(n->clavis);
        printf(" %ld", (longus)n->numerus);
    }
}

interior vacuum
_censum_imprimere (
    constans Unitates* u,
                  b32  machina,
              Piscina* piscina)
{
    UnitatesCensus* c          = unitates_census(u, piscina);
               i32  lacunosae  = ZEPHYRUM;
               i32  lacunae    = ZEPHYRUM;
               i32  i;
               i32  j;

    si (!machina)
    {
        puts("UNITATES CUM LACUNIS (exspectationes unitates.stml):");
    }
    per (i = ZEPHYRUM; i < xar_numerus(c->unitates); i++)
    {
        constans UnitatisCensus* summarium = (constans UnitatisCensus*)
            xar_obtinere(c->unitates, i);

        si (xar_numerus(summarium->desunt) == ZEPHYRUM)
        {
            perge;
        }
        lacunosae++;
        per (j = ZEPHYRUM; j < xar_numerus(summarium->desunt); j++)
        {
            lacunae++;
            fputs(machina ? "LACUNA\t" : "  ", stdout);
            _scribere(summarium->unitas->genus);
            fputs(machina ? "\t" : "  ", stdout);
            _scribere(summarium->unitas->titulus);
            fputs(machina ? "\t" : "  - ", stdout);
            _scribere(*(chorda*)xar_obtinere(summarium->desunt, j));
            putchar('\n');
        }
    }
    si (!machina)
    {
        printf("ORPHANI (probationes radicis sine unitate): %ld\n",
            (longus)xar_numerus(c->orphani));
    }
    per (i = ZEPHYRUM; i < xar_numerus(c->orphani); i++)
    {
        fputs(machina ? "ORPHANUS\t" : "  ", stdout);
        _scribere(*(chorda*)xar_obtinere(c->orphani, i));
        putchar('\n');
    }
    si (!machina)
    {
        printf("INDECLARATA (directoria cum codice quae radici cadunt):"
            " %ld\n", (longus)xar_numerus(c->indeclarata));
    }
    per (i = ZEPHYRUM; i < xar_numerus(c->indeclarata); i++)
    {
        constans UnitatesDirectorium* d =
            (constans UnitatesDirectorium*)
            xar_obtinere(c->indeclarata, i);

        fputs(machina ? "INDECLARATUM\t" : "  ", stdout);
        _scribere(d->via);
        fputs(machina ? "\t" : "  (", stdout);
        _numeros_imprimere(d->numeri);
        fputs(machina ? "\n" : ")\n", stdout);
    }
    printf(machina
        ? "SUMMA\tunitates\t%ld\tlacunosae\t%ld\tlacunae\t%ld"
          "\torphani\t%ld\tindeclarata\t%ld\n"
        : "summa: %ld unitates, %ld cum lacunis (%ld lacunae), %ld"
          " orphani, %ld directoria indeclarata\n",
        (longus)xar_numerus(c->unitates), (longus)lacunosae,
        (longus)lacunae, (longus)xar_numerus(c->orphani),
        (longus)xar_numerus(c->indeclarata));
}

/* clavis += 1 in Xar de UnitatesNumerus (ordine primi visi) */
interior vacuum
_numerum_augere (
       Xar* numeri,
    chorda  clavis)
{
    UnitatesNumerus* n;
                i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(numeri); i++)
    {
        n = (UnitatesNumerus*)xar_obtinere(numeri, i);
        si (chorda_aequalis(n->clavis, clavis))
        {
            n->numerus++;
            redde;
        }
    }
    n           = (UnitatesNumerus*)xar_addere(numeri);
    n->clavis   = clavis;
    n->numerus  = I;
}

interior vacuum
_indicem_scribere (
    constans Xar* verba)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(verba); i++)
    {
        si (i > ZEPHYRUM)
        {
            putchar(' ');
        }
        _scribere(*(chorda*)xar_obtinere(verba, i));
    }
}

interior b32
_unitas_eligitur (
    constans Unitas*  x,
                s32   argc,
          character** argv,
                s32   primus)
{
    s32 i;

    si (primus >= argc)
    {
        redde VERUM;
    }
    per (i = primus; i < argc; i++)
    {
        si (chorda_aequalis_literis(x->titulus, argv[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior s32
_interrogationes_imprimere (
    constans Unitates*  u,
                  s32   argc,
            character** argv,
                  s32   primus,
                  b32   machina,
              Piscina*  piscina)
{
    UnitatesInterrogationes* res = unitates_interrogationes(u, piscina);
                        Xar* genera = xar_creare(piscina,
                            (i32)magnitudo(UnitatesNumerus));
                         Xar* propria   = NIHIL;
             constans Unitas* prior     = NIHIL;
                         i32  numerus   = ZEPHYRUM;
                         i32  unitates  = ZEPHYRUM;
                         i32  vitia     = ZEPHYRUM;
                         s32  i;
                         i32  j;

    /* nomina eligenda notae esse debent: unitas ignota = usus */
    per (i = primus; i < argc; i++)
    {
        constans Xar* omnes  = unitates_omnes(u);
                 b32  nota   = FALSUM;

        per (j = ZEPHYRUM; j < xar_numerus(omnes) && !nota; j++)
        {
            nota = chorda_aequalis_literis(
                (*(Unitas**)xar_obtinere(omnes, j))->titulus, argv[i]);
        }
        si (!nota)
        {
            fprintf(stderr, "quid: unitas ignota '%s'\n", argv[i]);
            redde II;
        }
    }
    per (j = ZEPHYRUM; j <= xar_numerus(res->interrogationes); j++)
    {
        constans UnitatisInterrogatio* x = j < xar_numerus(
            res->interrogationes) ? (constans UnitatisInterrogatio*)
            xar_obtinere(res->interrogationes, j) : NIHIL;

        si (   x != NIHIL && !_unitas_eligitur(x->unitas, argc, argv,
                primus))
        {
            perge;
        }
        /* unitas nova (aut finis): numeri unitatis prioris */
        si (   !machina && prior != NIHIL
            && (x == NIHIL || x->unitas != prior))
        {
            fputs("  numeri: ", stdout);
            _numeros_imprimere(propria);
            fputs("\n\n", stdout);
        }
        si (x == NIHIL)
        {
            frange;
        }
        si (x->unitas != prior)
        {
            prior    = x->unitas;
            propria  = xar_creare(piscina,
                (i32)magnitudo(UnitatesNumerus));
            unitates++;
            si (!machina)
            {
                _scribere(x->unitas->titulus);
                fputs("  (", stdout);
                _scribere(x->via);
                fputs(")\n", stdout);
            }
        }
        numerus++;
        _numerum_augere(genera, x->genus);
        _numerum_augere(propria, x->genus);
        si (machina)
        {
            fputs("INTERROGATIO\t", stdout);
            _scribere(x->unitas->titulus);
            putchar('\t');
            _scribere(x->via);
            printf("\t%ld\t", (longus)x->linea);
            _scribere(x->genus);
            putchar('\t');
            _scribere(x->quaestio);
            putchar('\t');
            _scribere(x->responsum);
            putchar('\t');
            _indicem_scribere(x->symbola);
            putchar('\t');
            _indicem_scribere(x->viae);
            putchar('\t');
            _scribere(x->imperium);
            putchar('\n');
            perge;
        }
        printf("  %-9.*s ", (integer)x->genus.mensura,
            (constans character*)x->genus.datum);
        _scribere(x->quaestio);
        fputs("\n            -> ", stdout);
        _scribere(x->responsum);
        putchar('\n');
        si (xar_numerus(x->symbola) > ZEPHYRUM)
        {
            fputs("            symbolum: ", stdout);
            _indicem_scribere(x->symbola);
            putchar('\n');
        }
        si (xar_numerus(x->viae) > ZEPHYRUM)
        {
            fputs("            via: ", stdout);
            _indicem_scribere(x->viae);
            putchar('\n');
        }
        si (x->imperium.mensura > ZEPHYRUM)
        {
            fputs("            imperium: ", stdout);
            _scribere(x->imperium);
            putchar('\n');
        }
    }
    si (!machina && xar_numerus(res->recusationes) > ZEPHYRUM)
    {
        puts("VITIA:");
    }
    per (j = ZEPHYRUM; j < xar_numerus(res->recusationes); j++)
    {
        constans UnitatesRecusatio* r = (constans UnitatesRecusatio*)
            xar_obtinere(res->recusationes, j);

        vitia++;
        fputs(machina ? "VITIUM\t" : "  ", stdout);
        _scribere(r->via);
        printf(machina ? "\t%ld\t" : ":%ld: ", (longus)r->linea);
        _scribere(r->causa);
        putchar('\n');
    }
    fputs(machina ? "SUMMA\tinterrogationes\t" : "summa: ", stdout);
    printf(machina ? "%ld\tunitates\t%ld\tvitia\t%ld\tgenera\t"
        : "%ld interrogationes, %ld unitates, %ld vitia (",
        (longus)numerus, (longus)unitates, (longus)vitia);
    _numeros_imprimere(genera);
    fputs(machina ? "\n" : ")\n", stdout);
    redde vitia > ZEPHYRUM ? I : ZEPHYRUM;
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
                   b32  census   = FALSUM;
                   b32  rogatae  = FALSUM;
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
        alioquin si (strcmp(argv[i], "-census") == ZEPHYRUM)
        {
            census = VERUM;
        }
        alioquin si (strcmp(argv[i], "-interrogationes") == ZEPHYRUM)
        {
            rogatae = VERUM;
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
    si (   (!rogatae && (omnes || census) == (primus < argc))
        || (s32)omnes + (s32)census + (s32)rogatae > I
        || ((omnes || census || rogatae) && iudex != NIHIL))
    {
        fprintf(stderr, "usus: bin/fabrica quid [-machina] VIA...\n"
            "      bin/fabrica quid -iudex TITULUS [-machina] VIA...\n"
            "      bin/fabrica quid -omnes [-machina]\n"
            "      bin/fabrica quid -census [-machina]\n"
            "      bin/fabrica quid -interrogationes [-machina]"
            " [UNITAS...]\n");
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
    si (census)
    {
        _censum_imprimere(u, machina, piscina);
        redde ZEPHYRUM;
    }
    si (rogatae)
    {
        redde _interrogationes_imprimere(u, argc, argv, primus, machina,
            piscina);
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
