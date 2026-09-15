/* reticulum.c - instrumentum reticuli partitionum (oratio/reticulum.sh,
 * T35 d; vide oratio_reticulum.h)
 *
 * Usus: reticulum <x.tsv> -genus K -aurum aurum-...
 *   (-sortes columna | -sortes-alternae N)
 *   [-ubi titulus=v1,v2]... [-columnae t1,t2 | -praeter t1,t2]
 *   [-gradus t1,t2] [-limen N] [-lucrum N] [-profunditas N] [-prima N]
 *   [-initium t1,t2] [-greges columna|catena|catena-libera|initium]
 *   [-exempla N] [-machina]
 * -ubi valores crudos confert (ante -gradus). Numeri decimales.
 * Exitus: 0 iudicatum; 2 nihil iudicatum (causa in stderr), usus,
 * plagula absens.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "oratio_reticulum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* fructus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo <= 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    fructus->datum    = (i8*)memoria;
    fructus->mensura  = (i32)longitudo;
    redde VERUM;
}

/* segmentum literarum [initium, initium+mensura) in chordam piscinae */
interior chorda
_chordam_facere (
               Piscina* piscina,
    constans character* initium,
                   i32  mensura)
{
        chorda  c;
     character* datum;

    datum = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    c.datum    = (i8*)datum;
    c.mensura  = datum != NIHIL ? mensura : ZEPHYRUM;
    si (datum != NIHIL && mensura > ZEPHYRUM)
    {
        memcpy(datum, initium, (size_t)mensura);
    }
    redde c;
}

/* "a,b,c" -> chordae in lista */
interior vacuum
_listam_addere (
               Piscina* piscina,
                   Xar* lista,
    constans character* literae)
{
    constans character* initium = literae;

    dum (VERUM)
    {
         constans character* finis   = strchr(initium, ',');
                        i32  mensura = finis != NIHIL
                            ? (i32)(finis
                                - initium) : (i32)strlen(initium);
                    chorda* c        = (chorda*)xar_addere(lista);

        si (c != NIHIL)
        {
            *c = _chordam_facere(piscina, initium, mensura);
        }
        si (finis == NIHIL)
        {
            frange;
        }
        initium = finis + I;
    }
}

/* "titulus=v1,v2" -> condicio */
interior b32
_ubi_addere (
               Piscina* piscina,
                   Xar* ubi,
    constans character* literae)
{
     constans character* signum = strchr(literae, '=');
     OratioReticulumUbi* u;
     constans character* initium;
                    i32  numerus  = I;
                    i32  k        = ZEPHYRUM;

    si (signum == NIHIL || signum == literae)
    {
        redde FALSUM;
    }
    u = (OratioReticulumUbi*)xar_addere(ubi);
    si (u == NIHIL)
    {
        redde FALSUM;
    }
    u->titulus = _chordam_facere(piscina, literae,
        (i32)(signum - literae));
    per (initium = signum + I; *initium != '\0'; initium++)
    {
        si (*initium == ',')
        {
            numerus = numerus + I;
        }
    }
    u->valores = (chorda*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(chorda));
    si (u->valores == NIHIL)
    {
        redde FALSUM;
    }
    initium = signum + I;
    dum (VERUM)
    {
         constans character* finis   = strchr(initium, ',');
                        i32  mensura = finis != NIHIL
                            ? (i32)(finis
                                - initium) : (i32)strlen(initium);

        u->valores[k]  = _chordam_facere(piscina, initium, mensura);
        k              = k + I;
        si (finis == NIHIL)
        {
            frange;
        }
        initium = finis + I;
    }
    u->numerus = k;
    redde VERUM;
}

interior b32
_numerum_legere (
    constans character* literae,
                   i32* fructus)
{
     constans character* p      = literae;
                    i32  valor  = ZEPHYRUM;

    si (*p == '\0')
    {
        redde FALSUM;
    }
    per (; *p != '\0'; p++)
    {
        si (*p < '0' || *p > '9')
        {
            redde FALSUM;
        }
        valor = valor * X + (i32)(*p - '0');
    }
    *fructus = valor;
    redde VERUM;
}

interior integer
_usus (vacuum)
{
    fprintf(stderr, "usus: reticulum <x.tsv> -genus K -aurum aurum-... "
        "(-sortes columna | -sortes-alternae N) "
        "[-ubi titulus=v1,v2]... [-columnae t1,t2 | -praeter t1,t2] "
        "[-gradus t1,t2] [-limen N] [-lucrum N] "
        "[-profunditas N] [-prima N] [-initium t1,t2] "
        "[-greges columna|catena|catena-libera|initium] [-exempla N] "
        "[-machina]\n"
        "  -ubi valores crudos confert (ante -gradus); numeri "
        "decimales\n");
    redde II;
}

integer
principale (
      integer   argc,
    character** argv)
{
                    Piscina* piscina;
    OratioReticulumOptiones  optiones;
      OratioReticulumExitus  exitus;
         constans character* via = NIHIL;
                     chorda  tsv;
                    integer  i;

    piscina =
        piscina_generare_dynamicum("oratio_reticulum_instrumentum",
        67108864);
    si (piscina == NIHIL)
    {
        redde II;
    }
    oratio_reticulum_optiones_initium(piscina, &optiones);
    per (i = I; i < argc; i++)
    {
        constans character* argumentum = argv[i];

        si (strcmp(argumentum, "-machina") == ZEPHYRUM)
        {
            optiones.machina = VERUM;
        }
        alioquin si (argumentum[ZEPHYRUM] != '-')
        {
            via = argumentum;
        }
        alioquin si (i + I < argc)
        {
            constans character* valor = argv[i + I];

            si (strcmp(argumentum, "-genus") == ZEPHYRUM)
            {
                optiones.genus = valor;
            }
            alioquin si (strcmp(argumentum, "-aurum") == ZEPHYRUM)
            {
                optiones.aurum = valor;
            }
            alioquin si (strcmp(argumentum, "-sortes") == ZEPHYRUM)
            {
                optiones.sortes = valor;
            }
            alioquin si (strcmp(argumentum, "-greges") == ZEPHYRUM)
            {
                optiones.greges = valor;
            }
            alioquin si (strcmp(argumentum, "-ubi") == ZEPHYRUM)
            {
                si (!_ubi_addere(piscina, optiones.ubi, valor))
                {
                    redde _usus();
                }
            }
            alioquin si (strcmp(argumentum, "-columnae") == ZEPHYRUM)
            {
                _listam_addere(piscina, optiones.columnae, valor);
            }
            alioquin si (strcmp(argumentum, "-praeter") == ZEPHYRUM)
            {
                _listam_addere(piscina, optiones.praeter, valor);
            }
            alioquin si (strcmp(argumentum, "-gradus") == ZEPHYRUM)
            {
                _listam_addere(piscina, optiones.gradus, valor);
            }
            alioquin si (strcmp(argumentum, "-limen") == ZEPHYRUM)
            {
                si (!_numerum_legere(valor, &optiones.limen))
                {
                    redde _usus();
                }
            }
            alioquin si (strcmp(argumentum, "-lucrum") == ZEPHYRUM)
            {
                si (!_numerum_legere(valor, &optiones.lucrum))
                {
                    redde _usus();
                }
            }
            alioquin si (strcmp(argumentum, "-profunditas") == ZEPHYRUM)
            {
                si (!_numerum_legere(valor, &optiones.profunditas))
                {
                    redde _usus();
                }
            }
            alioquin si (strcmp(argumentum, "-prima") == ZEPHYRUM)
            {
                si (!_numerum_legere(valor, &optiones.prima))
                {
                    redde _usus();
                }
            }
            alioquin si (strcmp(argumentum, "-initium") == ZEPHYRUM)
            {
                _listam_addere(piscina, optiones.initium, valor);
            }
            alioquin si (strcmp(argumentum, "-sortes-alternae")
                         == ZEPHYRUM)
            {
                si (!_numerum_legere(valor, &optiones.alternae))
                {
                    redde _usus();
                }
            }
            alioquin si (strcmp(argumentum, "-exempla") == ZEPHYRUM)
            {
                si (!_numerum_legere(valor, &optiones.exempla))
                {
                    redde _usus();
                }
            }
            alioquin
            {
                redde _usus();
            }
            i = i + I;
        }
        alioquin
        {
            redde _usus();
        }
    }
    si (   via            == NIHIL || optiones.genus == NIHIL
        || optiones.aurum == NIHIL
        || (optiones.sortes == NIHIL && optiones.alternae == ZEPHYRUM))
    {
        redde _usus();
    }
    si (!_plagulam_legere(piscina, via, &tsv))
    {
        fprintf(stderr, "reticulum: plagula absens: %s\n", via);
        piscina_destruere(piscina);
        redde II;
    }
    exitus = oratio_reticulum_currere(piscina, tsv, via, &optiones);
    si (!exitus.iudicatum)
    {
        fprintf(stderr, "%.*s\n", (integer)exitus.causa.mensura,
            exitus.causa.datum != NIHIL
            ? (constans character*)exitus.causa.datum : "");
        piscina_destruere(piscina);
        redde II;
    }
    (vacuum)fwrite(exitus.relatio.datum, I,
        (size_t)exitus.relatio.mensura,
        stdout);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
