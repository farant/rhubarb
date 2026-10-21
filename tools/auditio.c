/* auditio.c - praeparata sonitus audire (sonus spec S1)
 *
 * Sonitum praeparatum reddit in plagulam WAV et per afplay sonat -
 * instrumentum aurium, non probatio: probationes numeros iudicant,
 * hoc Franus et Claudius auribus iudicant.
 *
 *   auditio                        praeparata cum longitudinibus
 *   auditio radius [semen]         /tmp/auditio_radius.wav, deinde afplay
 *   auditio radius 7 -via x.wav    plagulam solam scribit (non sonat)
 *   auditio -omnes                 omnia ordine sonat
 *
 * Usus: ./bin/aedilis tools/auditio.c && bash
 *   build/aedilis/auditio/struere.sh &&
 *   ./build/aedilis/auditio/auditio radius
 *
 * Exitus: 0 bene, 1 praeparatum ignotum aut scriptio fracta, 2 usus.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "processus.h"
#include "sonus.h"
#include "sonitus.h"
#include "sonus_wav.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* afplay plus longitudine soni numquam eget; terminus durus contra
 * pendentiam */
#define AUDITIO_MORA_MS (XXX * M)

interior vacuum
_praeparata_monstrare (
    vacuum)
{
    i32 k;

    per (k = ZEPHYRUM; k < sonitus_praeparatorum_numerus(); k++)
    {
        constans character* titulus   = sonitus_praeparati_titulus(k);
        constans SonitusParametra* p  = sonitus_praeparatum(titulus);

        imprimere("  %-10s %6.3f s   varietas %.2f\n", titulus,
                  (duplex)sonitus_longitudo(p, I)
                      / (duplex)SONUS_FREQUENTIA,
                  (duplex)p->varietas);
    }
}

/* reddit, scribit, et (si sonare) per afplay sonat; FALSUM si deficit */
interior b32
_audire (
    constans character* titulus,
                   i64  semen,
    constans character* via,
                   b32  sonare,
               Piscina* piscina)
{
     constans SonitusParametra* p = sonitus_praeparatum(titulus);
                           i32  n;
                           f32* exempla;
                           i32  scripta;

    si (p == NIHIL)
    {
        fprintf(stderr, "auditio: praeparatum ignotum '%s'\n", titulus);
        redde FALSUM;
    }
    si (via == NIHIL)
    {
        redde FALSUM;
    }
    n = sonitus_longitudo(p, semen);
    exempla = (f32*)piscina_allocare(piscina,
                                     (memoriae_index)n * magnitudo(f32));
    si (exempla == NIHIL)
    {
        fprintf(stderr, "auditio: memoria deest\n");
        redde FALSUM;
    }
    scripta = sonitus_reddere(p, semen, exempla, n);
    si (!sonus_wav_scribere(via, exempla, scripta, I, SONUS_FREQUENTIA,
                            piscina))
    {
        fprintf(stderr, "auditio: scribi non potuit '%s'\n", via);
        redde FALSUM;
    }
    imprimere("%s (semen %lu): %.3f s -> %s\n", titulus,
              (longus insignatus)semen,
              (duplex)scripta / (duplex)SONUS_FREQUENTIA, via);
    si (sonare)
    {
        constans character* argumenta[III];
         ProcessusResultus  r;

        argumenta[ZEPHYRUM] = "afplay";
        argumenta[I] = via;
        argumenta[II] = NIHIL;
        r = processus_exsequi(argumenta, AUDITIO_MORA_MS, piscina);
        si (!r.successus || r.codex_exitus != ZEPHYRUM)
        {
            fprintf(stderr, "auditio: afplay fractum (exitus %u)\n",
                    r.codex_exitus);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* /tmp/auditio_<titulus>.wav in piscina */
interior constans character*
_via_temporaria (
    constans character* titulus,
               Piscina* piscina)
{
    memoriae_index  n    = strlen(titulus) + XXXII;
         character* via  = (character*)piscina_allocare(piscina, n);

    si (via == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(via, "/tmp/auditio_%s.wav", titulus);
    redde via;
}

integer
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
     constans character* titulus  = NIHIL;
     constans character* via      = NIHIL;
                    i64  semen    = I;
                    b32  omnes    = FALSUM;
                    b32  bene     = VERUM;
                integer  k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnes") == ZEPHYRUM)
        {
            omnes = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-via") == ZEPHYRUM
                     && k + I < argc)
        {
            via = argv[++k];
        }
        alioquin si (argv[k][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "auditio: vexillum ignotum '%s'\n",
                argv[k]);
            redde II;
        }
        alioquin si (titulus == NIHIL)
        {
            titulus = argv[k];
        }
        alioquin
        {
            semen = (i64)strtoul(argv[k], NIHIL, X);
        }
    }
    si (!omnes && titulus == NIHIL)
    {
        imprimere("auditio: praeparata sonitus (auditio TITULUS [semen] "
                  "[-via x.wav] | -omnes)\n");
        _praeparata_monstrare();
        redde ZEPHYRUM;
    }

    piscina = piscina_generare_dynamicum("auditio", M * M);
    si (piscina == NIHIL)
    {
        redde I;
    }
    si (omnes)
    {
        per (k = ZEPHYRUM; k
            < (integer)sonitus_praeparatorum_numerus(); k++)
        {
            constans character* t = sonitus_praeparati_titulus((i32)k);

            si (!_audire(t, semen, _via_temporaria(t, piscina), VERUM,
                         piscina))
            {
                bene = FALSUM;
            }
        }
    }
    alioquin
    {
        bene = _audire(titulus, semen,
                       via != NIHIL ? via
                                    : _via_temporaria(titulus, piscina),
                       via == NIHIL, piscina);
    }
    piscina_destruere(piscina);
    redde bene ? ZEPHYRUM : I;
}
