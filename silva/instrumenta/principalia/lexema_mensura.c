/* lexema_mensura.c - MENSURA LEXATORIS silvae: per plagulam octeti,
 * lexemata, et celeritas (MB/s, Mlex/s) duorum graduum -
 * silva_lexare_cruda (fluxus crudus) et silva_lexare (crudus +
 * attachmentum triviorum). Tempus = tempus PROCESSORIS (clock), optimum
 * ex repetitionibus (onus machinae alienum minus fallit quam horologium
 * murale); piscina recens per repetitionem (allocatio numeratur ut in
 * usu vero). Plagulae per filum leguntur (canalis lectionum).
 *
 * Usus: ./silva/lexema_mensura.sh [-n REPETITIONES] [-machina]
 *                                  PLAGULA...
 *   sine plagulis: include/latina.h, silva/fontes/silva_c89_semantica.c
 *   -machina: MENSURA <via> <octeti> <lexemata> <crudus MB/s>
 *             <attachatus MB/s> <crudus Mlex/s>; SUMMA idem pro omnibus
 *
 * Exitus: 0; 2 = usus aut plagula illegibilis. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "silva_lexema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* tempus processoris unius graduum (secundae), optimum ex 'n' */
interior f64
_gradum_metiri (
                   b32  cum_triviis,
    constans character* fons,
                   i32  mensura,
                   i32  n,
                   i32* lexemata_out)
{
    f64 optimum = -1.0;
    i32 r;

    per (r = ZEPHYRUM; r < n; r++)
    {
        Piscina* p = piscina_generare_dynamicum("lexema_mensura",
            (memoriae_index)mensura * (memoriae_index)XVI
            + (memoriae_index)65536);
        clock_t  initium;
        clock_t  finis;
            Xar* x;
            f64  t;

        si (p == NIHIL)
        {
            redde -1.0;
        }
        initium = clock();
        x = cum_triviis ? silva_lexare(p, fons, mensura, ZEPHYRUM)
            : silva_lexare_cruda(p, fons, mensura, ZEPHYRUM);
        finis = clock();
        si (x != NIHIL && lexemata_out != NIHIL)
        {
            *lexemata_out = xar_numerus(x);
        }
        t = (f64)(finis - initium) / (f64)CLOCKS_PER_SEC;
        si (optimum < 0.0 || t < optimum)
        {
            optimum = t;
        }
        piscina_destruere(p);
    }
    redde optimum;
}

interior f64
_celeritas (
    f64 octeti,
    f64 secundae)
{
    redde secundae > 0.0 ? octeti / (1048576.0 * secundae) : 0.0;
}

s32
principale (
          s32   argc,
    character** argv)
{
    hic_manens constans character* ordinariae[] = {
        "include/latina.h", "silva/fontes/silva_c89_semantica.c", NIHIL
    };
     constans character** viae;
                    i32   numerus_viarum   = ZEPHYRUM;
                    i32   n                = V;
                    b32   machina          = FALSUM;
                    f64   summa_octetorum  = 0.0;
                    f64   summa_crudi      = 0.0;
                    f64   summa_adiuncti   = 0.0;
                    f64   summa_lexematum  = 0.0;
                Piscina*  pl;
                    s32   a;
                    i32   i;

    viae = (constans character**)malloc(
        (memoriae_index)(argc + III) * magnitudo(character*));
    si (viae == NIHIL)
    {
        redde II;
    }
    per (a = I; a < argc; a++)
    {
        si (strcmp(argv[a], "-n") == ZEPHYRUM && a + I < argc)
        {
            n = (i32)atoi(argv[++a]);
        }
        alioquin si (strcmp(argv[a], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (argv[a][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "usus: lexema_mensura [-n N] [-machina]"
                " PLAGULA...\n");
            redde II;
        }
        alioquin
        {
            viae[numerus_viarum++] = argv[a];
        }
    }
    si (numerus_viarum == ZEPHYRUM)
    {
        per (i = ZEPHYRUM; ordinariae[i] != NIHIL; i++)
        {
            viae[numerus_viarum++] = ordinariae[i];
        }
    }
    si (n < I)
    {
        n = I;
    }
    pl = piscina_generare_dynamicum("lexema_mensura_plagulae", 1048576);
    si (pl == NIHIL)
    {
        redde II;
    }
    si (!machina)
    {
        imprimere("lexema_mensura: tempus processoris, optimum ex %d\n",
            (integer)n);
        imprimere("  %-44s %9s %9s %9s %9s %9s\n", "plagula", "KB",
            "lexemata", "crud MB/s", "attc MB/s", "crud Ml/s");
    }
    per (i = ZEPHYRUM; i < numerus_viarum; i++)
    {
        chorda textus    = filum_legere_totum(viae[i], pl);
           i32 lexemata  = ZEPHYRUM;
           f64 t_crudus;
           f64 t_adiunctus;

        si (textus.datum == NIHIL)
        {
            fprintf(stderr, "lexema_mensura: %s illegibilis\n",
                viae[i]);
            redde II;
        }
        t_crudus = _gradum_metiri(FALSUM,
            (constans character*)textus.datum, textus.mensura, n,
            &lexemata);
        t_adiunctus = _gradum_metiri(VERUM,
            (constans character*)textus.datum, textus.mensura, n,
            NIHIL);
        summa_octetorum  += (f64)textus.mensura;
        summa_crudi      += t_crudus;
        summa_adiuncti   += t_adiunctus;
        summa_lexematum  += (f64)lexemata;
        si (machina)
        {
            imprimere("MENSURA\t%s\t%d\t%d\t%.2f\t%.2f\t%.3f\n",
                viae[i],
                (integer)textus.mensura, (integer)lexemata,
                _celeritas((f64)textus.mensura, t_crudus),
                _celeritas((f64)textus.mensura, t_adiunctus),
                t_crudus > 0.0 ? (f64)lexemata
                    / (1e6 * t_crudus) : 0.0);
        }
        alioquin si (numerus_viarum <= XXIV)
        {
            imprimere("  %-44s %9.1f %9d %9.2f %9.2f %9.3f\n", viae[i],
                (f64)textus.mensura / 1024.0, (integer)lexemata,
                _celeritas((f64)textus.mensura, t_crudus),
                _celeritas((f64)textus.mensura, t_adiunctus),
                t_crudus > 0.0 ? (f64)lexemata
                    / (1e6 * t_crudus) : 0.0);
        }
    }
    si (machina)
    {
        imprimere("SUMMA\t%d\t%.0f\t%.0f\t%.2f\t%.2f\t%.3f\n",
            (integer)numerus_viarum, summa_octetorum, summa_lexematum,
            _celeritas(summa_octetorum, summa_crudi),
            _celeritas(summa_octetorum, summa_adiuncti),
            summa_crudi > 0.0 ? summa_lexematum / (1e6 * summa_crudi)
            : 0.0);
    }
    alioquin
    {
        imprimere("  %-44s %9.1f %9.0f %9.2f %9.2f %9.3f\n",
            numerus_viarum > I ? "SUMMA" : "(una)",
            summa_octetorum / 1024.0, summa_lexematum,
            _celeritas(summa_octetorum, summa_crudi),
            _celeritas(summa_octetorum, summa_adiuncti),
            summa_crudi > 0.0 ? summa_lexematum / (1e6 * summa_crudi)
            : 0.0);
    }
    piscina_destruere(pl);
    free((vacuum*)viae);
    redde ZEPHYRUM;
}
