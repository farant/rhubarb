/* colloquium.c - superficies sessionis (M4b chunki C+D)
 *
 * INTERACTIVUS: ansa fgets (modus coctus - functionat fistulatum);
 * continuatio per sessio_initus_completus (librae); imperia
 * :monstra / :effusio <k> / :serva [-strictum] <via> / :aperi <via>
 * / :exi. Formatio per sessio_relatum_formare.
 *
 * BATCH (cursor #!, chunk D): colloquium <scriptum.c> - linea #!
 * obliterata, iudicium ADEST, recusationes DESUNT (scriptum =
 * programma verum: scripturae/tempus/stdin licent), effusio sola
 * imprimitur; principale (si definitum) post turnos curritur;
 * exitus processus = codex eius.
 *
 * Usus: colloquium [scriptum.c] [-plagulae a,b,c] [-posix]
 *       [-radix via]
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

interior chorda
_plagulam_totam_legere (Piscina* piscina, constans character* via)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    chorda fructus;
    i8* datum;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    si (pl == NIHIL)
    {
        redde fructus;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l <= 0L)
    {
        fclose(pl);
        redde fructus;
    }
    datum = piscina_allocare(piscina, (memoriae_index)mensura_l);
    si (datum == NIHIL
        || fread(datum, I, (memoriae_index)mensura_l, pl)
            != (memoriae_index)mensura_l)
    {
        fclose(pl);
        redde fructus;
    }
    fclose(pl);
    fructus.datum = datum;
    fructus.mensura = (i32)mensura_l;
    redde fructus;
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina;
    Sessio* sessio;
    SessioConfiguratio cfg;
    constans character* plagulae[XXXII];
    constans character* via_scripti = NIHIL;
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
        alioquin si (argv[k][ZEPHYRUM] != '-'
            && via_scripti == NIHIL)
        {
            via_scripti = argv[k];
        }
        alioquin
        {
            fprintf(stderr, "usus: colloquium [scriptum.c]"
                " [-plagulae a,b,c] [-posix] [-radix via]\n");
            redde I;
        }
    }

    si (via_scripti != NIHIL)
    {
        /* modus batch (C12): recusationes desunt, iudicium adest */
        cfg.sine_recusationibus = VERUM;
        cfg.cum_posix = VERUM;   /* scripta bibliothecas POSIX agunt */
    }

    sessio = sessio_creare(piscina, &cfg);
    si (sessio == NIHIL)
    {
        fprintf(stderr, "colloquium: sessio deest (curre ex radice"
            " repositorii aut da -radix)\n");
        redde I;
    }

    si (via_scripti != NIHIL)
    {
        chorda scriptum = _plagulam_totam_legere(piscina,
            via_scripti);
        SessioRelatum r;
        s32 recepti;
        s64 codex = ZEPHYRUM;
        b32 principale_definitum = FALSUM;

        si (scriptum.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "colloquium: scriptum illegibile: %s\n",
                via_scripti);
            redde I;
        }
        recepti = sessio_scriptum_offerre(sessio, scriptum, &r);
        si (recepti < ZEPHYRUM
            || r.verdictum != SESSIO_ACCEPTUM)
        {
            chorda f = sessio_relatum_formare(sessio, &r, piscina);

            fprintf(stderr, "%s: turnus %d reiectus\n", via_scripti,
                (int)(recepti + I));
            si (f.mensura > ZEPHYRUM)
            {
                fprintf(stderr, "%.*s", (int)f.mensura,
                    (constans character*)f.datum);
            }
            redde I;
        }
        /* effusio turnorum (capta) ad stdout */
        per (k = ZEPHYRUM; k < (integer)sessio_turni_numerus(sessio);
            k++)
        {
            chorda e = sessio_turnus_effusio(sessio, (i32)k);

            si (e.mensura > ZEPHYRUM)
            {
                imprimere("%.*s", (int)e.mensura,
                    (constans character*)e.datum);
            }
            /* principale = macro latinae (-> main): valor lexematis
             * = textus EXPANSUS (lex emitte) - symbolum verum "main" */
            si (sessio_turnus_genus(sessio, (i32)k)
                    == SESSIO_TURNUS_DEFINITIO
                && (chorda_aequalis_literis(
                        sessio_turnus_nomen(sessio, (i32)k), "main")
                    || chorda_aequalis_literis(
                        sessio_turnus_nomen(sessio, (i32)k),
                        "principale")))
            {
                principale_definitum = VERUM;
            }
        }
        fflush(stdout);
        si (principale_definitum)
        {
            si (!sessio_functionem_currere(sessio, "main", &codex))
            {
                fprintf(stderr, "colloquium: principale fractum\n");
                redde I;
            }
        }
        sessio_destruere(sessio);
        redde (s32)codex;
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
            alioquin si (strncmp(initus, ":serva", VI) == ZEPHYRUM)
            {
                character* via = initus + VI;
                b32 strictum = FALSUM;
                chorda textus_servandus;
                FILE* pl;

                dum (*via == ' ')
                {
                    via++;
                }
                si (strncmp(via, "-strictum", IX) == ZEPHYRUM)
                {
                    strictum = VERUM;
                    via += IX;
                    dum (*via == ' ')
                    {
                        via++;
                    }
                }
                via[strcspn(via, "\n")] = '\0';
                si (*via == '\0')
                {
                    imprimere("usus: :serva [-strictum] <via>\n");
                    perge;
                }
                textus_servandus = strictum
                    ? sessio_documentum_strictum(sessio, piscina)
                    : sessio_documentum(sessio, piscina);
                pl = fopen(via, "wb");
                si (pl == NIHIL)
                {
                    imprimere("serva: %s non scribitur\n", via);
                    perge;
                }
                si (textus_servandus.mensura > ZEPHYRUM)
                {
                    fwrite(textus_servandus.datum, I,
                        (memoriae_index)textus_servandus.mensura,
                        pl);
                }
                fclose(pl);
                imprimere("servatum: %s (%d octeti)\n", via,
                    (int)textus_servandus.mensura);
            }
            alioquin si (strncmp(initus, ":aperi", VI) == ZEPHYRUM)
            {
                character* via = initus + VI;
                chorda scriptum;
                SessioRelatum r_scripti;
                s32 recepti;

                dum (*via == ' ')
                {
                    via++;
                }
                via[strcspn(via, "\n")] = '\0';
                scriptum = _plagulam_totam_legere(piscina, via);
                si (scriptum.mensura == ZEPHYRUM)
                {
                    imprimere("aperi: %s illegibile\n", via);
                    perge;
                }
                recepti = sessio_scriptum_offerre(sessio, scriptum,
                    &r_scripti);
                imprimere("aperti: %d turni\n", (int)recepti);
                si (r_scripti.verdictum != SESSIO_ACCEPTUM)
                {
                    chorda f = sessio_relatum_formare(sessio,
                        &r_scripti, piscina);

                    si (f.mensura > ZEPHYRUM)
                    {
                        imprimere("%.*s", (int)f.mensura,
                            (constans character*)f.datum);
                    }
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
