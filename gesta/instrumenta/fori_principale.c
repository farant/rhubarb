/* fori_principale.c - proxy MCP stdio<->TCP ad tabulariumd (F1
 * forum)
 *
 * Introitus .mcp.json "fori": Claudius per stdio loquitur, daemon
 * per TCP - proxy lineas VERBATIM fert (protocollum idem utrimque,
 * spec-v2 par IV). Connexio PER-PETITIONEM (contractus v1: daemon
 * sequentialis numquam a nobis obsessus). Classificatio per
 * tabellarius_nuntium_legere: PETITIO/PRAVUM = responsum unum
 * exspectatur; NUNTIATIO/RESPONSUM = mitte-et-obliviscere (fovea:
 * daemon nuntiationes tacite omittit - proxy exspectans penderet).
 * Start-if-absent HIC habitat: recusatio -> launcher genitus semel
 * + anquisitio; defectus finalis -> error curae in petitione.
 *
 * Usus: fori [-mcp] [-portus N]   (stdout protocollum PURUM) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabellarius.h"
#include "tcp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FORI_PORTUS_ORDINARIUS 8753
/* frustum receptionis (responsa fluunt - tectum nullum) */
#define FORI_FRUSTUM 8192

interior TcpConnexio*
_conectere_aut_gignere (
        i32  portus,
        b32  generatio_licita,
    Piscina* pn);

/* connexio per-petitionem; recusatio -> daemon genitus per
 * launcher portu eodem. Custos tempestatis: generatio UNA per
 * defectionem - connexio prospera vexillum RE-ARMAT (renovatio
 * daemonis exitum mundum petit, respawn sequens licere debet;
 * mensura viva 2026-07-29: 'semel per vitam' respawn post
 * renovare obstruxit). generatio_licita FALSUM
 * (-sine-generatione, probationes) = defectus statim. */
interior TcpConnexio*
_conectere_aut_gignere (
        i32  portus,
        b32  generatio_licita,
    Piscina* pn)
{
    hic_manens b32 genitus  = FALSUM;
       TcpResultus rc       = tcp_connectere("127.0.0.1", portus,
           pn);
           integer k;

    si (rc.successus)
    {
        genitus = FALSUM;   /* re-armatio: defectio nova generat */
        redde rc.connexio;
    }
    si (!generatio_licita)
    {
        redde NIHIL;
    }
    si (!genitus)
    {
        character imperium[CXXVIII];

        genitus = VERUM;
        fprintf(stderr, "fori: tabulariumd absens - gigno per"
            " launcher (portus %d)\n", (int)portus);
        sprintf(imperium, "./gesta/tabulariumd.sh -portus %d"
            " > /dev/null 2>&1 &", (int)portus);
        (vacuum)system(imperium);
    }
    per (k = ZEPHYRUM; k < XXV; k++)
    {
        (vacuum)system("sleep 0.2");
        rc = tcp_connectere("127.0.0.1", portus, pn);
        si (rc.successus)
        {
            genitus = FALSUM;   /* re-armatio */
            redde rc.connexio;
        }
    }
    redde NIHIL;
}

s32
principale (
      integer   argc,
    character** argv)
{
        i32 portus            = FORI_PORTUS_ORDINARIUS;
        b32 generatio_licita  = VERUM;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-portus") == ZEPHYRUM && k + I < argc)
        {
            portus = (i32)atoi(argv[k + I]);
            k++;
        }
        alioquin si (strcmp(argv[k], "-sine-generatione")
                     == ZEPHYRUM)
        {
            generatio_licita = FALSUM;
        }
        /* -mcp acceptum et omissum (modus unicus) */
    }

    per (;;)
    {
        Piscina* pn = piscina_generare_dynamicum("fori_nuntius",
            LXIV * 1024);
                       b32  finitus = FALSUM;
                    chorda  linea;
        TabellariusNuntius  nuntius;
                       b32  responsum_exspectatur;
               TcpConnexio* conn;

        si (pn == NIHIL)
        {
            redde I;
        }
        linea = tabellarius_lineam_legere(stdin, pn, &finitus);
        si (finitus)
        {
            piscina_destruere(pn);
            redde ZEPHYRUM;   /* cliens claudit = ordinatus */
        }
        nuntius = tabellarius_nuntium_legere(linea, pn);
        responsum_exspectatur =
            nuntius.genus == TABELLARIUS_PETITIO
            || nuntius.genus == TABELLARIUS_PRAVUM;

        conn = _conectere_aut_gignere(portus, generatio_licita,
            pn);
        si (conn == NIHIL)
        {
            si (responsum_exspectatur)
            {
                character cura[CXXVIII];

                sprintf(cura, "tabulariumd absens (portus %d) -"
                    " ./gesta/tabulariumd.sh incipe",
                    (int)portus);
                (vacuum)tabellarius_lineam_scribere(stdout,
                    tabellarius_errorem(pn, nuntius.id,
                        TABELLARIUS_ERROR_INTERNUS, cura));
            }
            piscina_destruere(pn);
            perge;
        }
        si (   tcp_mittere_omnia(conn, (constans i8*)linea.datum,
                linea.mensura)
            && tcp_mittere_omnia(conn, (constans i8*)"\n", I)
            && responsum_exspectatur)
        {
            /* frusta VERBATIM in stdout usque ad novam lineam -
             * responsum unum per petitionem (framing daemonis) */
            per (;;)
            {
                 i8 frustum[FORI_FRUSTUM];
                s32 n = tcp_recipere(conn, frustum,
                    (i32)FORI_FRUSTUM);
                s32 j;
                b32 completum = FALSUM;

                si (n <= ZEPHYRUM)
                {
                    frange;
                }
                fwrite(frustum, I, (memoriae_index)n, stdout);
                per (j = ZEPHYRUM; j < n; j++)
                {
                    si (frustum[j] == (i8)'\n')
                    {
                        completum = VERUM;
                    }
                }
                si (completum)
                {
                    frange;
                }
            }
            fflush(stdout);
        }
        tcp_claudere(conn);
        piscina_destruere(pn);
    }
}
