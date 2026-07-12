/* colloquium.c - superficies linearis sessionis (M4b chunk C)
 *
 * Ansa fgets (modus coctus - functionat fistulatum; cursor #! =
 * chunk D). Continuatio per sessio_initus_completus (librae).
 * Imperia: :monstra (documentum), :effusio <k> (acta turni),
 * :exi. Formatio per sessio_relatum_formare (machina - transcripta
 * probabilia byte-exacte).
 *
 * Usus: colloquium [-plagulae a,b,c] [-posix] [-radix via]
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "sessio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITUS_MAXIMUS 65536

interior s32 _plagulas_findere (character* argumentum,
    constans character** plagulae, s32 maximum);

interior s32
_plagulas_findere (character* argumentum,
    constans character** plagulae, s32 maximum)
{
    s32 numerus = ZEPHYRUM;
    character* p = argumentum;

    dum (numerus < maximum && p != NIHIL && *p != '\0')
    {
        character* virgula = strchr(p, ',');

        plagulae[numerus] = p;
        numerus++;
        si (virgula == NIHIL)
        {
            frange;
        }
        *virgula = '\0';
        p = virgula + I;
    }
    redde numerus;
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina;
    Sessio* sessio;
    SessioConfiguratio cfg;
    constans character* plagulae[XXXII];
    character initus[INITUS_MAXIMUS];
    integer k;

    piscina = piscina_generare_dynamicum("colloquium", 268435456);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "colloquium: piscina deest\n");
        redde I;
    }
    memset(&cfg, ZEPHYRUM, magnitudo(SessioConfiguratio));
    cfg.radix = ".";

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-posix") == ZEPHYRUM)
        {
            cfg.cum_posix = VERUM;
        }
        alioquin si (strcmp(argv[k], "-plagulae") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            cfg.plagulae = plagulae;
            cfg.plagulae_numerus = _plagulas_findere(argv[k],
                plagulae, (s32)XXXII);
        }
        alioquin si (strcmp(argv[k], "-radix") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            cfg.radix = argv[k];
        }
        alioquin
        {
            fprintf(stderr, "usus: colloquium [-plagulae a,b,c]"
                " [-posix] [-radix via]\n");
            redde I;
        }
    }

    sessio = sessio_creare(piscina, &cfg);
    si (sessio == NIHIL)
    {
        fprintf(stderr, "colloquium: sessio deest (curre ex radice"
            " repositorii aut da -radix)\n");
        redde I;
    }

    imprimere("colloquium (M4b) - :exi exit, :monstra documentum\n");
    per (;;)
    {
        i32 mensura = ZEPHYRUM;
        chorda textus;

        imprimere("> ");
        fflush(stdout);
        per (;;)
        {
            character* linea = initus + mensura;
            i32 spatium = (i32)INITUS_MAXIMUS - mensura - I;

            si (spatium <= I
                || fgets(linea, (int)spatium, stdin) == NIHIL)
            {
                imprimere("\n");
                redde ZEPHYRUM;   /* EOF = exitus */
            }
            mensura += (i32)strlen(linea);
            textus = chorda_ex_buffer((i8*)initus, mensura);
            si (sessio_initus_completus(sessio, textus))
            {
                frange;
            }
            imprimere(".. ");
            fflush(stdout);
        }

        /* linea vacua */
        {
            b32 vacua = VERUM;
            i32 j;

            per (j = ZEPHYRUM; j < mensura; j++)
            {
                si (initus[j] != ' ' && initus[j] != '\t'
                    && initus[j] != '\n' && initus[j] != '\r')
                {
                    vacua = FALSUM;
                    frange;
                }
            }
            si (vacua)
            {
                perge;
            }
        }

        /* imperia `:` */
        si (initus[ZEPHYRUM] == ':')
        {
            si (strncmp(initus, ":exi", IV) == ZEPHYRUM)
            {
                frange;
            }
            alioquin si (strncmp(initus, ":monstra", VIII)
                == ZEPHYRUM)
            {
                i32 n = sessio_turni_numerus(sessio);
                i32 j;

                per (j = ZEPHYRUM; j < n; j++)
                {
                    chorda t = sessio_turnus_textus(sessio, j);

                    imprimere("[%d] %.*s", (int)j, (int)t.mensura,
                        (constans character*)t.datum);
                }
            }
            alioquin si (strncmp(initus, ":effusio", VIII)
                == ZEPHYRUM)
            {
                integer index = atoi(initus + VIII);
                chorda e = sessio_turnus_effusio(sessio,
                    (i32)index);

                si (e.mensura > ZEPHYRUM)
                {
                    imprimere("%.*s", (int)e.mensura,
                        (constans character*)e.datum);
                }
            }
            alioquin
            {
                imprimere("imperia: :monstra :effusio <k> :exi\n");
            }
            perge;
        }

        /* turnus */
        {
            SessioRelatum r = sessio_turnum_offerre(sessio, textus);
            chorda formatum = sessio_relatum_formare(sessio, &r,
                piscina);

            si (formatum.mensura > ZEPHYRUM)
            {
                imprimere("%.*s", (int)formatum.mensura,
                    (constans character*)formatum.datum);
            }
        }
    }

    sessio_destruere(sessio);
    redde ZEPHYRUM;
}
