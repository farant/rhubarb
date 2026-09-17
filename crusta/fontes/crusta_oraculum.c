/* crusta_oraculum.c - Casus oraculi bash (vide .h) */

#include "crusta_oraculum.h"
#include "crusta_exempla.h"
#include <stdio.h>
#include <string.h>

#define PATHOLOGIAE "probationes/fixa/crusta/pathologiae.sh"

character*
crusta_oraculum_legere (
               Piscina* piscina,
    constans character* radix,
    constans character* via,
                   i32* mensura)
{
    character plena[4096];
         FILE* f;
       longus  longitudo;
    character* memoria;
       size_t  lecti;

    si (strlen(radix) + strlen(via) + II > magnitudo(plena))
    {
        redde NIHIL;
    }
    sprintf(plena, "%s/%s", radix, via);
    f = fopen(plena, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    fseek(f, 0L, SEEK_SET);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    memoria[lecti]  = '\0';
    *mensura        = (i32)lecti;
    redde memoria;
}

/* chorda ex litteris in piscina ('\0' post mensuram) */
interior chorda
_chorda_nova (
               Piscina* piscina,
    constans character* a,
                   i32  mensura_a,
    constans character* b,
                   i32  mensura_b)
{
       chorda  c;
    character* m = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_a + mensura_b) + I);

    c.mensura  = mensura_a + mensura_b;
    c.datum    = (i8*)m;
    si (m == NIHIL)
    {
        redde c;
    }
    memcpy(m, a, (size_t)mensura_a);
    memcpy(m + mensura_a, b, (size_t)mensura_b);
    m[mensura_a + mensura_b] = '\0';
    redde c;
}

interior b32
_addere (
                Piscina* piscina,
                    Xar* collectio,
     constans character* praefixum,
     constans character* titulus,
                    i32  mensura_tituli,
     constans character* datum,
                    i32  mensura_dati)
{
    CrustaCasusOraculi* c = (CrustaCasusOraculi*)xar_addere(collectio);

    si (c == NIHIL)
    {
        redde FALSUM;
    }
    c->clavis = _chorda_nova(piscina, praefixum, (i32)strlen(praefixum),
        titulus, mensura_tituli);
    c->datum  = _chorda_nova(piscina, datum, mensura_dati, "",
        ZEPHYRUM);
    redde c->clavis.datum != NIHIL && c->datum.datum != NIHIL;
}

Xar*
crusta_oraculum_casus (
                Piscina*  piscina,
     constans character*  radix,
     constans character** causa)
{
          Xar* collectio;
          Xar* exempla;
    character* fons;
          i32  mensura = ZEPHYRUM;
          i32  k;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    collectio = xar_creare(piscina, (i32)magnitudo(CrustaCasusOraculi));
    fons = crusta_oraculum_legere(piscina, radix, PATHOLOGIAE,
        &mensura);
    si (collectio == NIHIL || fons == NIHIL)
    {
        si (causa != NIHIL)
        {
            *causa = fons == NIHIL ? PATHOLOGIAE " absens" : "memoria";
        }
        redde NIHIL;
    }
    exempla = crusta_exempla_legere(piscina, fons, mensura);
    si (exempla == NIHIL)
    {
        si (causa != NIHIL)
        {
            *causa = "memoria";
        }
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(exempla); k++)
    {
        constans CrustaExemplum* e =
            (constans CrustaExemplum*)xar_obtinere(exempla, k);

        si (!_addere(piscina, collectio, "pathologiae:",
                (constans character*)e->titulus.datum,
                e->titulus.mensura,
                (constans character*)e->datum.datum, e->datum.mensura))
        {
            si (causa != NIHIL)
            {
                *causa = "memoria";
            }
            redde NIHIL;
        }
    }
    per (k = ZEPHYRUM; CRUSTA_FREEBSD_PLAGULAE[k] != NIHIL; k++)
    {
        constans character* via     = CRUSTA_FREEBSD_PLAGULAE[k];
        constans character* ultima  = strrchr(via, '/');

        fons = crusta_oraculum_legere(piscina, radix, via, &mensura);
        si (fons == NIHIL)
        {
            si (causa != NIHIL)
            {
                *causa = "plagula FreeBSD absens";
            }
            redde NIHIL;
        }
        ultima = ultima != NIHIL ? ultima + I : via;
        si (!_addere(piscina, collectio, "freebsd:", ultima,
                (i32)strlen(ultima), fons, mensura))
        {
            si (causa != NIHIL)
            {
                *causa = "memoria";
            }
            redde NIHIL;
        }
    }
    redde collectio;
}

chorda
crusta_oraculum_involvere (
    Piscina* piscina,
     chorda  datum)
{
    chorda primum;
       b32 linea = datum.mensura > ZEPHYRUM
           && datum.datum[datum.mensura - I] == '\n';

    primum = _chorda_nova(piscina, "f() {\n", VI,
        (constans character*)datum.datum, datum.mensura);
    si (primum.datum == NIHIL)
    {
        redde primum;
    }
    redde _chorda_nova(piscina, (constans character*)primum.datum,
        primum.mensura, linea ? "}\n" : "\n}\n", linea ? II : III);
}

chorda
crusta_oraculum_aurum (
          constans Xar* aurum,
                chorda  clavis,
    constans character* sectio)
{
    chorda nulla;
       i32 k;

    nulla.mensura  = ZEPHYRUM;
    nulla.datum    = NIHIL;
    si (aurum == NIHIL)
    {
        redde nulla;
    }
    per (k = ZEPHYRUM; k < xar_numerus(aurum); k++)
    {
        constans CrustaExemplum* e =
            (constans CrustaExemplum*)xar_obtinere(aurum, k);

        si (   e->titulus.mensura == clavis.mensura
            && memcmp(e->titulus.datum, clavis.datum,
                (size_t)clavis.mensura) == ZEPHYRUM)
        {
            redde crusta_exemplum_sectio(e, sectio);
        }
    }
    redde nulla;
}
