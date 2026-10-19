/* tools/oceanus.c - instrumentum DNS DigitalOcean (bin non
 * installatur: ./bin/aedilis tools/oceanus.c && bash
 * build/aedilis/oceanus/struere.sh -> build/aedilis/oceanus/oceanus)
 *
 *   oceanus [-clavis VIA] [-basis URL] registra <dominium>
 *   oceanus [-clavis VIA] [-basis URL] assecurare <dominium> <genus>
 *           <titulus> <data> [ttl]
 *
 * registra: "id genus titulus data ttl" per lineam (tabulatoribus).
 * assecurare: IDEM | CREATUM | MUTATUM; error = exitus I cum causa.
 * Clavis numquam imprimitur (oceanus.h). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "oceanus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior s32
_usus (vacuum)
{
    fprintf(stderr, "usus: oceanus [-clavis VIA] [-basis URL] registra "
        "<dominium>\n       oceanus [-clavis VIA] [-basis URL] "
        "assecurare <dominium> <genus> <titulus> <data> [ttl]\n");
    redde II;
}

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* p;
                Oceanus* o;
     constans character* clavis  = NIHIL;
     constans character* basis   = NIHIL;
                integer  k       = I;
                    s32  exitus  = ZEPHYRUM;

    dum (k + I < argc && argv[k][0] == '-')
    {
        si (strcmp(argv[k], "-clavis") == ZEPHYRUM)
        {
            clavis = argv[k + I];
        }
        alioquin si (strcmp(argv[k], "-basis") == ZEPHYRUM)
        {
            basis = argv[k + I];
        }
        alioquin
        {
            redde _usus();
        }
        k += II;
    }
    si (k >= argc)
    {
        redde _usus();
    }
    p = piscina_generare_dynamicum("oceanus", LXIV * MXXIV);
    o = p ? oceanus_creare(clavis, basis, p) : NIHIL;
    si (!o)
    {
        redde I;
    }
    si (strcmp(argv[k], "registra") == ZEPHYRUM && k + II == argc)
    {
        Xar* x = xar_creare(p, (i32)magnitudo(OceanusRegistrum));
        i32  i;

        si (!oceanus_registra(o, argv[k + I], x))
        {
            fprintf(stderr, "oceanus: %s\n", oceanus_error(o));
            exitus = I;
        }
        per (i = ZEPHYRUM; exitus == ZEPHYRUM
            && i < xar_numerus(x); i++)
        {
            OceanusRegistrum* r = (OceanusRegistrum*)xar_obtinere(x, i);

            printf("%lu\t%.*s\t%.*s\t%.*s\t%u\n",
                (insignatus longus)r->id,
                (int)r->genus.mensura,
                (constans character*)r->genus.datum,
                (int)r->titulus.mensura,
                (constans character*)r->titulus.datum,
                (int)r->data.mensura,
                (constans character*)r->data.datum,
                (insignatus integer)r->ttl);
        }
    }
    alioquin si (   strcmp(argv[k], "assecurare") == ZEPHYRUM
                 && (k + V == argc || k + VI == argc))
    {
                  i32 tempus_vitae = MMMDC;
        OceanusExitus r;

        si (k + VI == argc)
        {
            tempus_vitae = (i32)atoi(argv[k + V]);
        }
        r = oceanus_assecurare(o, argv[k + I], argv[k + II],
            argv[k + III], argv[k + IV], tempus_vitae);

        si (r == OCEANUS_ERROR)
        {
            fprintf(stderr, "oceanus: %s\n", oceanus_error(o));
            exitus = I;
        }
        alioquin
        {
            printf("%s %s.%s %s %s\n", r == OCEANUS_IDEM ? "IDEM"
                : r == OCEANUS_CREATUM ? "CREATUM" : "MUTATUM",
                argv[k + III], argv[k + I], argv[k + II], argv[k + IV]);
        }
    }
    alioquin
    {
        exitus = _usus();
    }
    oceanus_claudere(o);
    piscina_destruere(p);
    redde exitus;
}
